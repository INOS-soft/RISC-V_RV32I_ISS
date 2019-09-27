#ifndef MEMORY_H
#define MEMORY_H

/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2019

   Class for memory

**************************************************************** */

#include <vector>
#include <unordered_map>

using namespace std;

class memory {

 private:

  // Use an un-ordered map. Perfect data structure for this.
  // Dynamically allocated automatically, super fast, perfect.
  unordered_map<unsigned int, uint32_t> mem;
  bool verbose_output;
  uint32_t word_aligned_address;
  uint32_t memory_word;
  uint32_t existing_word;

 public:

  // Constructor
  memory(bool verbose);
  	 
  // Read a word of data from a word-aligned address.
  // If the address is not a multiple of 4, it is rounded down to a multiple of 4.
  uint32_t read_word (uint32_t address);

  // Write a word of data to a word-aligned address.
  // If the address is not a multiple of 4, it is rounded down to a multiple of 4.
  // The mask contains 1s for bytes to be updated and 0s for bytes that are to be unchanged.
  void write_word (uint32_t address, uint32_t data, uint32_t mask);

  // Load a hex image file and provide the start address for execution from the file in start_address.
  // Return true if the file was read without error, or false otherwise.
  bool load_file(string file_name, uint32_t &start_address);

};

#endif
