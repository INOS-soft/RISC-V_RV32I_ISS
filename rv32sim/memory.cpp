/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2019

   Class members for memory

**************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <cstdio>
#include <unordered_map>
#include "memory.h"

using namespace std;

// Constructor
memory::memory(bool verbose)
{
    verbose_output = verbose;
}

// Read a word of data from a word-aligned address.
// If the address is not a multiple of 4, it is rounded down to a multiple of 4.
uint32_t memory::read_word (uint32_t address)
{
    word_aligned_address = address - (address % 4);

    memory_word = mem[word_aligned_address];
    if(verbose_output)
    {
        cout << "Memory read word: address = " << hex << setw(8) << setfill('0') << address
            << ", data = " << hex << setw(8) << setfill('0') << memory_word << endl;
    }
    
    return mem[word_aligned_address];
}

// Write a word of data to a word-aligned address.
// If the address is not a multiple of 4, it is rounded down to a multipl of 4.
// The mask contains 1s for bytes to be updated and 0s for bytes that are to be unchanged.
void memory::write_word (uint32_t address, uint32_t data, uint32_t mask)
{
    if(verbose_output)
    {
        cout << "Memory write word: address = " << hex << setw(8) << setfill('0') << address
            << ", data = " << hex << setw(8) << setfill('0') << data
            << ", mask = " << hex << setw(8) << setfill('0') << mask << endl;
    }
    // word align the memory address
    word_aligned_address = address - (address % 4);
    existing_word = mem[word_aligned_address];

    // make sure data matches mask
    data = data & mask;
    existing_word = existing_word & (~mask);
    mem[word_aligned_address] = existing_word | data;
}

// Load a hex image file and provide the start address for execution from the file in start_address.
// Return true if the file was read without error, or false otherwise.
bool memory::load_file(string file_name, uint32_t &start_address)
{
    ifstream input_file(file_name);
    string input;
    unsigned int line_count = 0;
    unsigned int byte_count = 0;
    char record_start;
    char byte_string[3];
    char halfword_string[5];
    unsigned int record_length;
    unsigned int record_address;
    unsigned int record_type;
    unsigned int record_data;
    unsigned int record_checksum;
    bool end_of_file_record = false;
    uint32_t load_address;
    uint32_t load_data;
    uint32_t load_mask;
    uint32_t load_base_address = 0x00000000UL;
    start_address = 0x00000000UL;
    
    if (input_file.is_open())
    {
        while (true)
        {
            line_count++;
            input_file >> record_start;
            
            if (record_start != ':')
            {
                cout << "Input line " << dec << line_count << " does not start with colon character" << endl;
                return false;
            }

            // first 3 hex digits are length of record
            input_file.get(byte_string, 3);
            sscanf(byte_string, "%x", &record_length);
            // next 5 digits are address of record in hex
            input_file.get(halfword_string, 5);
            sscanf(halfword_string, "%x", &record_address);
            // next 3 are the type of record it is
            input_file.get(byte_string, 3);
            sscanf(byte_string, "%x", &record_type);
            
            switch (record_type)
            {
                case 0x00:  // Data record
                    //cerr << "entered case 0x00" << endl;
                    for (unsigned int i = 0; i < record_length; i++)
                    {
                        // read a byte
                        input_file.get(byte_string, 3);
                        sscanf(byte_string, "%x", &record_data);
                        // byte address of word
                        load_address = (load_base_address | (uint32_t)(record_address)) + i;
                        // shift byte to appropriate spot in 32 bit word
                        load_data = (uint32_t)(record_data) << ((load_address % 4) * 8);
                        // create a mask so write_word can write to the correct byte
                        load_mask = 0x000000ffUL << ((load_address % 4) * 8);
                        

                        //cerr << "calling write_word with address: " << load_address
                        //    << " and data: " << load_data << " and mask: " << load_mask << endl;
                        
                        // load_address & 0xfffffffc rounds down to word address
                        write_word(load_address & 0xfffffffcUL, load_data, load_mask);

                        //cerr << "write done, reading: " << read_word(load_address) << endl; 

                        byte_count++;
                    }
                    break;

                case 0x01:  // End of file
                    end_of_file_record = true;
                    break;

                case 0x02:  // Extended segment address (set bits 19:4 of load base address)
                    load_base_address = 0x00000000UL;
                    for (unsigned int i = 0; i < record_length; i++)
                    {
                        input_file.get(byte_string, 3);
                        sscanf(byte_string, "%x", &record_data);
                        load_base_address = (load_base_address << 8) | (record_data << 4);
                    }
                    break;

                case 0x03:  // Start segment address (ignored)
                    for (unsigned int i = 0; i < record_length; i++)
                    {
                        input_file.get(byte_string, 3);
                        sscanf(byte_string, "%x", &record_data);
                    }
                    break;

                case 0x04:  // Extended linear address (set upper halfword of load base address)
                    load_base_address = 0x00000000UL;
                    for (unsigned int i = 0; i < record_length; i++)
                    {
                        input_file.get(byte_string, 3);
                        sscanf(byte_string, "%x", &record_data);
                        load_base_address = (load_base_address << 8) | (record_data << 16);
                    }
                    break;

                case 0x05:  // Start linear address (set execution start address)
                    start_address = 0x00000000UL;
                    for (unsigned int i = 0; i < record_length; i++)
                    {
                        input_file.get(byte_string, 3);
                        sscanf(byte_string, "%x", &record_data);
                        start_address = (start_address << 8) | record_data;
                    }
                    break;
            }

            input_file.get(byte_string, 3);
            sscanf(byte_string, "%x", &record_checksum);
            input_file.ignore();

            if (end_of_file_record)
                break;
        }

        input_file.close();
        cout << dec << byte_count << " bytes loaded, start address = "
        << setw(8) << setfill('0') << hex << start_address << endl;
        return true;
    }
    
    else
    {
        cout << "Failed to open file" << endl;
        return false;
    }
}
