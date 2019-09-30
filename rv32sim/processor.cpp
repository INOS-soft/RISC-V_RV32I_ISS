/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2019

   Class for processor
   
   Written by:
   Nathanael Whitrow
   For feedback, comments and questions please contact me at:
   nathanael.whitrow@gmail.com

**************************************************************** */

#include <iostream>
#include <iomanip>

#include "processor.h"

using namespace std;



// Consructor
processor::processor(memory* main_memory, bool verbose, bool stage2)
{
	mem = main_memory;
	verbose_output = verbose;
	stage2_check = stage2;
}

// Display PC value
void processor::show_pc()
{
	cout << setw(8) << setfill('0') << hex << pc << endl;
}

// Set PC to new value
void processor::set_pc(uint32_t new_pc)
{
	pc = new_pc;
}

// Display register value
void processor::show_reg(unsigned int reg_num)
{
	cout << setw(8) << setfill('0') << hex << x[reg_num] << endl;
}

// Set register to new value
void processor::set_reg(unsigned int reg_num, uint32_t new_value)
{
	if(reg_num != 0)
	{
		x[reg_num] = new_value;
	}
}

// Execute a number of instructions
void processor::execute(unsigned int num, bool breakpoint_check)
{
	for(unsigned int i = 0; i < num; i++)
	{
		// check for breakpoint
		if(pc == breakpoint && breakpoint_check)
		{
			cout << "breakpoint reached at " << setw(8) << setfill('0') << hex << pc << endl;
			return;
		}

		if(pending_interrupt())
			break;


		current_instruction = mem->read_word(pc);

		if(verbose_output)
		{
			cout << "Fetch: pc = " << hex << setw(8) << setfill('0') << pc
				<< ", ir = " << hex << setw(8) << setfill('0') << current_instruction << endl;
		}
		

		// get current instruction, increment PC, and get opcode. 
		pc += 4;
		cpu_cycles++;	// all instructions take at least one cycle
		op_code = current_instruction & opcode_bits;


		// check for pc misaligned error
		if ((pc % 4) != 0)
		{
			//throw exception error 0 - instruction address misaligned
			exception(0);
			break;
		}


		// start decoding
		if(op_code == r_type_a)
		{
			// r_type_a
			tmp_funct3 = ((current_instruction & funct3_bits) >> 12);

			tmp_rd = ((current_instruction & rd_bits) >> 7);
			tmp_rs1 = ((current_instruction & rs1_bits) >> 15);
			tmp_rs2 = ((current_instruction & rs2_bits) >> 20);

			if (tmp_funct3 == 0)
			{
				// add or sub
				tmp_funct7 = (current_instruction & funct7_bits) >> 25;
				if (tmp_funct7 == 0)
				{
					// add instruction
					set_reg(tmp_rd, (x[tmp_rs1] + x[tmp_rs2]));
				}

				else if (tmp_funct7 == 0x20)
				{
					// sub instruction
					if(verbose_output)
					{
						cout << "sub: rd = " << tmp_rd << ", rs1 = " << tmp_rs1 << ", rs2 = " << tmp_rs2 << endl;
					}
					set_reg(tmp_rd, (x[tmp_rs1] - x[tmp_rs2]));
				}
				else
				{
					// EXCEPTION cause 2 Illegal instruction
					exception(2);
					cout << "Not a valid opcode." << endl;
				}
			}

			else if (tmp_funct3 == 1)
			{
				// sll instruction
				tmp_imm_32 = x[tmp_rs2] & 0x0000001fUL;
				set_reg(tmp_rd, (x[tmp_rs1] << tmp_imm_32));
			}

			else if (tmp_funct3 == 2)
			{
				// slt instruction
				if ((int32_t)x[tmp_rs1] < (int32_t)x[tmp_rs2])
					set_reg(tmp_rd, 1);
				else
					set_reg(tmp_rd, 0);
			}

			else if (tmp_funct3 == 3)
			{
				// sltu instruction
				if (x[tmp_rs1] < x[tmp_rs2])
					set_reg(tmp_rd, 1);
				else
					set_reg(tmp_rd, 0);
			}

			else if (tmp_funct3 == 4)
			{
				// xor instruction
				set_reg(tmp_rd, (x[tmp_rs1] ^ x[tmp_rs2]));
			}

			else if (tmp_funct3 == 5)
			{
				// srl or sra instruction
				tmp_funct7 = (current_instruction & funct7_bits) >> 25;
				tmp_imm_32 = x[tmp_rs2] & 0x0000001fUL;
				if (tmp_funct7 == 0)
				{
					// srl instruction
					set_reg(tmp_rd, x[tmp_rs1] >> tmp_imm_32);
				}

				else if (tmp_funct7 == 0x20)
				{
					// sra instruction
					set_reg(tmp_rd, ((int32_t)x[tmp_rs1]) >> tmp_imm_32);
				}
			}

			else if (tmp_funct3 == 6)
			{
				// or instruction
				set_reg(tmp_rd, (x[tmp_rs1] | x[tmp_rs2]));
			}

			else if (tmp_funct3 == 7)
			{
				// and instruction
				set_reg(tmp_rd, (x[tmp_rs1] & x[tmp_rs2]));
			}
		}

		else if(op_code == i_type_l)
		{
			// load type instructions

			cpu_cycles = cpu_cycles + 2;

			tmp_funct3 = ((current_instruction & funct3_bits) >> 12);
			tmp_rs1 = (current_instruction & rs1_bits) >> 15;
			tmp_rd = (current_instruction & rd_bits) >> 7;
			
			immed_I = (uint32_t)(((int32_t)(current_instruction & i_type_imm_bits)) >> 20);

			if(verbose_output)
			{
				cout << "lw: rd = " << dec << tmp_rd << ", rs1 = " << dec << tmp_rs1
					<< ", immed_I = " << hex << setw(8) << setfill('0') << immed_I << endl;
			}
			
			mem_addr = immed_I + x[tmp_rs1]; // effective address

			offset = mem_addr % 4;
			mem_addr = mem_addr - (mem_addr % 4);	// word aligned address

			
			tmp_imm_32 = mem->read_word(mem_addr);	// word at memory address

			if(tmp_funct3 == 0)
			{
				// lb instruction
				// create mask based on offset
				if(offset == 0)
				{
					mask = 0x000000ffUL;
					tmp_imm_32 = tmp_imm_32 & mask;
				}
				else if (offset == 1)
				{
					mask = 0x0000ff00UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 8;
				}
				else if (offset == 2)
				{
					mask = 0x00ff0000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 16;
				}
				else if (offset == 3)
				{
					mask = 0xff000000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 24;
				}
				else
					cout << "ERROR in load, offset out of bounds" << endl;

				// sign extend the result
				if((tmp_imm_32 & 0x00000080UL) == 0x00000080UL)
					tmp_imm_32 = tmp_imm_32 | 0xffffff00UL;

				set_reg(tmp_rd, tmp_imm_32);
			}

			else if(tmp_funct3 == 1)
			{
				// lh instruction

				if (offset == 0)
				{
					// hw is in least significant part of memory word
					mask = 0x0000ffffUL;
					tmp_imm_32 = tmp_imm_32 & mask;
					// sign extend
					if((tmp_imm_32 & 0x00008000UL) == 0x00008000UL)
						tmp_imm_32 = tmp_imm_32 | 0xffff0000UL;

					set_reg(tmp_rd, tmp_imm_32);
				}

				else if (offset == 1)
				{
					// lh exception 4 - address not multiple of 2
					cpu_cycles-=2;
					exception(4);
					//return;
					//mask = 0x00ffff00UL;
					//tmp_imm_32 = (tmp_imm_32 & mask) >> 8;
				}
				else if (offset == 2)
				{
					mask = 0xffff0000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 16;
					// sign extend
					if((tmp_imm_32 & 0x00008000UL) == 0x00008000UL)
						tmp_imm_32 = tmp_imm_32 | 0xffff0000UL;

					set_reg(tmp_rd, tmp_imm_32);
				}

				else if (offset == 3)
				{
					// lh exception 4 - address not multiple of 2
					cpu_cycles-=2;
					exception(4);
					//return;
					/*
					cpu_cycles = cpu_cycles + 2;
					// first byte of word
					mask = 0xff000000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 24;
					// second byte of half word
					mem_addr = mem_addr + 4;
					tmp_imm_32_2 = mem->read_word(mem_addr);
					mask = 0x000000ffUL;
					tmp_imm_32_2 = (tmp_imm_32_2 & mask) << 8;
					// final result:
					tmp_imm_32 = (tmp_imm_32 | tmp_imm_32_2);
					*/
				}
			}

			else if(tmp_funct3 == 2)
			{
				// lw instruction
				// lw exception 4 - address not multiple of 4
				if (offset != 0)
				{
					exception(4);
					//return;
				}
				else
					set_reg(tmp_rd, tmp_imm_32);
			}

			else if(tmp_funct3 == 4)
			{
				// lbu instruction
				// lb instruction
				// create mask based on offset
				if(offset == 0)
				{
					mask = 0x000000ffUL;
					tmp_imm_32 = tmp_imm_32 & mask;
				}
				else if (offset == 1)
				{
					mask = 0x0000ff00UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 8;
				}
				else if (offset == 2)
				{
					mask = 0x00ff0000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 16;
				}
				else if (offset == 3)
				{
					mask = 0xff000000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 24;
				}
				else
					cout << "ERROR in load, offset out of bounds" << endl;

				set_reg(tmp_rd, tmp_imm_32);
			}

			else if(tmp_funct3 == 5)
			{
				// lhu instruction

				if (offset == 0)
				{
					// hw is in least significant part of memory word
					mask = 0x0000ffffUL;
					tmp_imm_32 = tmp_imm_32 & mask;
					set_reg(tmp_rd, tmp_imm_32);
				}

				else if (offset == 1)
				{
					// lhu exception 4 - address not multiple of 2
					cpu_cycles-=2;
					exception(4);
					//return;
					//mask = 0x00ffff00UL;
					//tmp_imm_32 = (tmp_imm_32 & mask) >> 8;
				}
				else if (offset == 2)
				{
					mask = 0xffff0000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 16;
					set_reg(tmp_rd, tmp_imm_32);
				}

				else if (offset == 3)
				{
					// lhu exception 4 - address not multiple of 2
					cpu_cycles-=2;
					exception(4);
					//return;
					/*
					cpu_cycles = cpu_cycles + 2;
					// first byte of word
					mask = 0xff000000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 24;
					// second byte of half word
					mem_addr = mem_addr + 4;
					tmp_imm_32_2 = mem->read_word(mem_addr);
					mask = 0x000000ffUL;
					tmp_imm_32_2 = (tmp_imm_32_2 & mask) << 8;
					// final result:
					tmp_imm_32 = (tmp_imm_32 | tmp_imm_32_2);
					*/
				}
			}

			else
			{
				// EXCEPTION cause 2 Illegal instruction
				exception(2);
			}
		}

		else if(op_code == i_type_j)
		{
			// jalr

			// get immediates
			tmp_rs1 = (current_instruction & rs1_bits) >> 15;
			tmp_rd = (current_instruction & rd_bits) >> 7;
			tmp_imm_32 = (current_instruction & i_type_imm_bits) >> 20;
			
			// sign extend the immediate
			if((current_instruction & 0x80000000UL) == 0x80000000UL)
				tmp_imm_32 = (tmp_imm_32 | 0xfffff000UL);
			tmp_imm_32 = (tmp_imm_32 + x[tmp_rs1]) & 0xfffffffeUL;

			// do the thing
			set_reg(tmp_rd, pc);
			set_pc(tmp_imm_32);
			
			// admin stuff
			cpu_cycles ++;
			if(verbose_output)
			{
				cout << "jalr: rd = " << dec << tmp_rd << ", rs1 = " << tmp_rs1
					<< ", immed_J = " << hex << setw(8) << setfill('0') << tmp_imm_32 << endl;
			}
		}

		else if(op_code == i_type_a)
		{
			////cerr << "i_type_a found." << endl;
			// calculate funct3
			tmp_funct3 = ((current_instruction & funct3_bits) >> 12);
			
			// get rd, rs1, and sign extend immediate
			tmp_rd = ((current_instruction & rd_bits) >> 7);
			tmp_rs1 = ((current_instruction & rs1_bits) >> 15);
			tmp_imm_32 = ((current_instruction & i_type_imm_bits) >> 20);
			if((current_instruction & 0x80000000UL) == 0x80000000UL)
				tmp_imm_32 = (tmp_imm_32 | 0xfffff000UL);

			if(tmp_funct3 == 0)
			{
				// addi instruction
				if(verbose_output)
				{
					cout << "addi: rd = " << dec << tmp_rd << ", rs1 = " << tmp_rs1
						<< ", immed_I = " << hex << setw(8) << setfill('0') << tmp_imm_32 << endl;
				}
				tmp_imm_32 += x[tmp_rs1];
				set_reg(tmp_rd, tmp_imm_32);
			}

			else if(tmp_funct3 == 2)
			{
				// slti instruction
				if( (int32_t)x[tmp_rs1] < (int32_t)tmp_imm_32)
					set_reg(tmp_rd, 1);
				else
					set_reg(tmp_rd, 0);
			}

			else if(tmp_funct3 == 3)
			{
				// sltiu instruction
				if( x[tmp_rs1] < tmp_imm_32)
					set_reg(tmp_rd, 1);
				else
					set_reg(tmp_rd, 0);
			}

			else if(tmp_funct3 == 4)
			{
				// xori instruction
				set_reg(tmp_rd, (x[tmp_rs1] ^ tmp_imm_32));
			}

			else if(tmp_funct3 == 6)
			{
				// ori instruction
				set_reg(tmp_rd, (x[tmp_rs1] | tmp_imm_32));
			}

			else if(tmp_funct3 == 7)
			{
				// andi instruction
				set_reg(tmp_rd, (x[tmp_rs1] & tmp_imm_32));
			}

			else if (tmp_funct3 == 1)
			{
				// slli instruction
				tmp_imm_32 = (current_instruction & i_type_imm_4_0) >> 20;
				set_reg(tmp_rd, (x[tmp_rs1] << tmp_imm_32));
			}

			else if (tmp_funct3 == 5)
			{
				// srli or srai instruction
				tmp_funct7 = (current_instruction & funct7_bits) >> 25;
				tmp_imm_32 = (current_instruction & i_type_imm_4_0) >> 20; // shift ammount

				if (tmp_funct7 == 0)
				{
					// srli instruction
					set_reg(tmp_rd, (x[tmp_rs1] >> tmp_imm_32));
				}

				else if (tmp_funct7 == 0x20)
				{
					// srai instruction
					// logical shift right
					set_reg(tmp_rd, ((int32_t)x[tmp_rs1]) >> tmp_imm_32);
					// 
				}

				else
				{
					cout << "instruction not recognised: " << setw(8)
						<< setfill('0') << hex << current_instruction << endl;
					break;
				}
			}

		}

		else if(op_code == s_type)
		{
			// store instruction
			cpu_cycles++;
			tmp_funct3 = ((current_instruction & funct3_bits) >> 12);
			tmp_rs1 = ((current_instruction & rs1_bits) >> 15);
			tmp_rs2 = ((current_instruction & rs2_bits) >> 20);

			// build immediate
			bits_4_0 = (current_instruction & s_imm_4_to_0) >> 7;
			bits_11_5 = (current_instruction & s_imm_11_to_5) >> 20;
			immed_S = bits_11_5 | bits_4_0;
			if((immed_S & 0x00000800UL) == 0x00000800UL)
				immed_S = (immed_S | 0xffffff00UL);

			mem_addr = immed_S + x[tmp_rs1];	// effective address

			// exception 6 store address misaligned if offset != 0
			offset = mem_addr % 4;
			mem_addr = mem_addr - (mem_addr % 4);	// word aligned address

			if(tmp_funct3 == 0)
			{
				// sb instruction
				tmp_imm_32 = x[tmp_rs2];
				tmp_imm_32 = tmp_imm_32 & 0x000000ffUL;

				if(verbose_output)
				{
					cout << "sb: rs1 = " << dec << tmp_rs1 << ", rs2 = " << tmp_rs2
						<< ", immed_S = " << hex << setw(8) << setfill('0') << immed_S << endl;
				}

				if (offset == 0)
					mem->write_word(mem_addr, tmp_imm_32, 0x000000ffUL);

				else if (offset == 1)
				{
					tmp_imm_32 = (tmp_imm_32 << 8);
					mem->write_word(mem_addr, tmp_imm_32, 0x0000ff00UL);
				}

				else if (offset == 2)
				{
					tmp_imm_32 = (tmp_imm_32 << 16);
					mem->write_word(mem_addr, tmp_imm_32, 0x00ff0000UL);
				}

				else if (offset == 3)
				{
					tmp_imm_32 = (tmp_imm_32 << 24);
					mem->write_word(mem_addr, tmp_imm_32, 0xff000000UL);	
				}

				else
					cout << "Error in save: offset out of bounds." << endl;

			}

			else if(tmp_funct3 == 1)
			{
				// sh instruction
				tmp_imm_32 = x[tmp_rs2];
				tmp_imm_32 = tmp_imm_32 & 0x0000ffffUL;

				if(verbose_output)
				{
					cout << "sb: rs1 = " << dec << tmp_rs1 << ", rs2 = " << tmp_rs2
						<< ", immed_S = " << hex << setw(8) << setfill('0') << immed_S << endl;
				}

				if (offset == 0)
					mem->write_word(mem_addr, tmp_imm_32, 0x0000ffffUL);

				else if (offset == 1)
				{
					// sh exception 6 address not multiple of 2
					cpu_cycles--;
					exception(6);
					//return;
					//tmp_imm_32 = (tmp_imm_32 << 8);
					//mem->write_word(mem_addr, tmp_imm_32, 0x00ffff00UL);
				}

				else if (offset == 2)
				{
					tmp_imm_32 = (tmp_imm_32 << 16);
					mem->write_word(mem_addr, tmp_imm_32, 0xffff0000UL);
				}

				else if (offset == 3)
				{
					// sh exception 6 address not multiple of 2
					cpu_cycles--;
					exception(6);
					//return;
					/*
					cpu_cycles += 2;

					tmp_imm_32_2 = (tmp_imm_32 & 0x0000ff00UL) >> 8;
					tmp_imm_32 = (tmp_imm_32 << 24);
					mem->write_word(mem_addr, tmp_imm_32, 0xff000000UL);

					mem_addr = mem_addr + 4;
					mem->write_word(mem_addr, tmp_imm_32_2, 0x000000ffUL);
					*/
				}

				else
					cout << "Error in save: offset out of bounds." << endl;
			}

			else if(tmp_funct3 == 2)
			{
				// sw instruction
				// sw exception 6 address not multiple of 4
				if(verbose_output)
				{
					cout << "sw: rs1 = " << dec << tmp_rs1 << ", rs2 = " << tmp_rs2
						<< ", immed_S = " << hex << setw(8) << setfill('0') << immed_S << endl;
				}

				if (offset != 0)
				{
					cpu_cycles--;
					exception(6);
					//break;
				}
				else
				{
					tmp_imm_32 = x[tmp_rs2];
					mem->write_word(mem_addr, tmp_imm_32, 0xffffffffUL);
				}
			}

			else
			{
				// EXCEPTION cause 2 Illegal instruction
				exception(2);
			}
		}

		else if(op_code == b_type)
		{
			// bits for determining type of branch
			tmp_funct3 = ((current_instruction & funct3_bits) >> 12);

			// build rs1 and rs2
			tmp_rs1 = ((current_instruction & rs1_bits) >> 15);
			tmp_rs2 = ((current_instruction & rs2_bits) >> 20);
			// build immediate - careful because Branch instructions are multiples of 2
			bits_4_1 = ((current_instruction & b_type_imm_4_1) >> 7);
			bits_10_5 = ((current_instruction & b_type_imm_10_5) >> 20);
			bits_11 = ((current_instruction & b_type_imm_11) << 4);
			bits_12 = ((current_instruction & b_type_imm_12) >> 19);
			tmp_imm_32 = bits_12 | bits_11 | bits_10_5 | bits_4_1;
			if((current_instruction & 0x80000000UL) == 0x80000000UL)
				tmp_imm_32 = (tmp_imm_32 | 0xfffff000UL);

			if(tmp_funct3 == 0)
			{
				// BEQ
				if((int32_t)x[tmp_rs1] == (int32_t)x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
			}

			else if(tmp_funct3 == 1)
			{
				// BNE
				if(verbose_output)
				{
					cout << "bne: rs1 = " << dec << tmp_rs1 << ", rs2 = " << tmp_rs2
						<< ", immed_B = "<< hex << setw(8) << setfill('0') << tmp_imm_32 << endl;
				}
				if((int32_t)x[tmp_rs1] != (int32_t)x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
			}

			else if(tmp_funct3 == 4)
			{
				// BLT
				if((int32_t)x[tmp_rs1] < (int32_t)x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
			}

			else if(tmp_funct3 == 5)
			{
				// BGE
				if((int32_t)x[tmp_rs1] >= (int32_t)x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
			}

			else if(tmp_funct3 == 6)
			{
				// BLTU
				if(x[tmp_rs1] < x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
			}

			else if(tmp_funct3 == 7)
			{
				// BGEU
				if(x[tmp_rs1] >= x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
			}

			else
			{
				// EXCEPTION cause 2 Illegal instruction
				exception(2);
			}
		}

		else if(op_code == lui_type)
		{
			// lui instruction
			tmp_rd = (current_instruction & rd_bits) >> 7;
			tmp_imm_32 = current_instruction & u_type_imm_bits;
			set_reg(tmp_rd, tmp_imm_32);
			if(verbose_output)
			{
				cout << "lui: rd = " << dec << tmp_rd << ", immed_U = " 
					<< hex << setw(8) << setfill('0') << tmp_imm_32 << endl;
			}
		}

		else if(op_code == auipc_type)
		{
			// auipc instruction
			tmp_rd = (current_instruction & rd_bits) >> 7;
			tmp_imm_32 = current_instruction & u_type_imm_bits;
			if(verbose_output)
			{
				cout << "auipc: rd = " << dec << tmp_rd
					<< ", immed_U = " << hex << setw(8) << setfill('0') << tmp_imm_32 << endl;
			}
			tmp_imm_32 = tmp_imm_32 + pc - 4;
			set_reg(tmp_rd, tmp_imm_32);
		}

		else if(op_code == jal_type)
		{
			// jal instruction
			// get rd and save current PC
			tmp_rd = (current_instruction & rd_bits) >> 7;
			set_reg(tmp_rd, pc);
			// build immediate. Sign extend.
			bits_10_1 = 	(current_instruction & j_type_imm_10_1) >> 20;
			bits_11 = 		(current_instruction & j_type_imm_11) >> 9;
			bits_19_12 = 	(current_instruction & j_type_imm_19_12);
			bits_20 = 		(current_instruction & j_type_imm_20) >> 11;
			tmp_imm_32 = bits_20 | bits_19_12 | bits_11 | bits_10_1;
			if((current_instruction & 0x80000000UL) == 0x80000000UL)
				tmp_imm_32 = (tmp_imm_32 | 0xffe00000UL);
			
			if(verbose_output)
			{
				cout << "jal: rd = " << tmp_rd << ", immed_J = "
					<< hex << setw(8) << setfill('0') << tmp_imm_32 << endl;
			}
			
			// add to PC to form target address
			tmp_imm_32 = tmp_imm_32 + pc - 4;
			set_pc(tmp_imm_32);
			cpu_cycles++;

		}

		else if(op_code == fence_type)
		{
			// fence instruction
		}

		else if(op_code == system)
		{
			// SYSTEM call

			// tmp numbers for decoding
			tmp_funct3 = ((current_instruction & funct3_bits) >> 12);
			tmp_rd = ((current_instruction & rd_bits) >> 7);
			tmp_rs1 = ((current_instruction & rs1_bits) >> 15);
			immed_I = (uint32_t)(((int32_t)(current_instruction & i_type_imm_bits)) >> 20);
			// DON"T sign extend this immediate
			immed_I = (immed_I & 0x00000fffUL);
			

			if(current_instruction == ecall_type)
			{
				// exception when called from user or machine mode
				if(prv == 0)
					exception(8);
				else if (prv == 3)
					exception(11);
			}

			else if (current_instruction == ebreak_type)
			{
				exception(3);
			}

			// mret instruction
			else if (current_instruction == mret_type)
			{
				if(prv < 3)
				{
					// EXCEPTION cause 2 Illegal instruction
					exception(2);
				}

				else
				{
					// get MPP and restore privilege level
					set_prv(mstatus >> 11);
					// set MIE to MPIE
					mstatus = (mstatus >> 4);
					// set MPIE bit to 1
					mstatus = (mstatus | 0x00000080UL);
					// set MPP bits to zero
					mstatus = (mstatus & 0x00000088UL);
					// set pc to value stored in mepc register
					set_pc(mepc);
				}
			}

			// check for illegal CSR number
			else if(	immed_I != 0xf11 &&
						immed_I != 0xf12 &&
						immed_I != 0xf13 &&
						immed_I != 0xf14 &&
						immed_I != 0x300 &&
						immed_I != 0x301 &&
						immed_I != 0x304 &&
						immed_I != 0x305 &&
						immed_I != 0x340 &&
						immed_I != 0x341 &&
						immed_I != 0x342 &&
						immed_I != 0x343 &&
						immed_I != 0x344	)
			{
				exception(2);
			}



			// CSRRW instruction - put CSR register into rd then put rs1 into CSR
			else if (tmp_funct3 == 1)
			{
				if ((prv < 3) || is_read_only(immed_I) )
					exception(2);
				else
				{
					if (tmp_rd != 0)
						set_reg(tmp_rd, get_csr(immed_I));
					if(immed_I == 0x344)
						manipulate_mip(x[tmp_rs1]);
					else
						set_csr(immed_I, x[tmp_rs1]);
				}
			}

			// CSRRS instruction
			else if (tmp_funct3 == 2)
			{
				if ((prv < 3) || (is_read_only(immed_I) && (tmp_rs1 != 0)) )
					exception(2);
				else
				{
					set_reg(tmp_rd, get_csr(immed_I));
					if (tmp_rs1 != 0)
					{
						if(immed_I == 0x344)
							manipulate_mip(x[tmp_rs1] | get_csr(immed_I));
						else
							set_csr(immed_I, (x[tmp_rs1] | get_csr(immed_I)));
					}
				}
			}

			else if (tmp_funct3 == 3)
			{
				if ((prv < 3) || (is_read_only(immed_I) && (tmp_rs1 != 0)) )
					exception(2);
				else
				{
					// CSRRC instruction
					set_reg(tmp_rd, get_csr(immed_I));
					if (tmp_rs1 != 0)
					{
						if(immed_I == 0x344)
							manipulate_mip(~x[tmp_rs1] & get_csr(immed_I));
						else
							set_csr(immed_I, (~x[tmp_rs1]) & get_csr(immed_I) );
					}
				}
			}

			// use rs1 as immediate
			else if (tmp_funct3 == 5)
			{
				if ((prv < 3) || (is_read_only(immed_I)))
					exception(2);
				else
				{
					// CSRRWI instruciton
					set_reg(tmp_rd, get_csr(immed_I));
					if (tmp_rs1 != 0)
					{
						if(immed_I == 0x344)
							manipulate_mip((uint32_t)tmp_rs1);
						else
							set_csr(immed_I, (uint32_t)tmp_rs1);
					}
				}
			}

			else if (tmp_funct3 == 6)
			{
				if ((prv < 3) || (is_read_only(immed_I) && (tmp_rs1 != 0)) )
					exception(2);
				else
				{
					// CSRRSI instruction
					set_reg(tmp_rd, get_csr(immed_I));
					if (tmp_rs1 != 0)
					{
						if(immed_I == 0x344)
							manipulate_mip(((uint32_t)tmp_rs1) | get_csr(immed_I));
						else
							set_csr(immed_I, ( ((uint32_t)tmp_rs1) | get_csr(immed_I)));
					}
				}
			}

			else if (tmp_funct3 == 7)
			{
				if ((prv < 3) || (is_read_only(immed_I) && (tmp_rs1 != 0)) )
					exception(2);
				else
				{
					// CSRRCI instruction
					set_reg(tmp_rd, get_csr(immed_I));
					if (tmp_rs1 != 0)
					{
						if(immed_I == 0x344)
							manipulate_mip(((uint32_t)(~tmp_rs1)) & get_csr(immed_I));
						else
							set_csr(immed_I, ((uint32_t)(~tmp_rs1)) & get_csr(immed_I));
					}
				}
			}


			else
			{
				// EXCEPTION cause 2 Illegal instruction
				tmp_funct3 = (current_instruction & funct3_bits) >> 12;
				if(tmp_funct3 != 0)
					cout << "Error: illegal instruction" << endl;
			}
		}

		else
		{
			// EXCEPTION cause 2 Illegal instruction
			//cerr << "OP-Code " << hex << op_code << " not recognized." << endl;
			break;
		}
		
		// increment instruction count
		instructions_executed++;
	}
}

void processor::exception(unsigned int cause)
{
	switch (cause)
	{
		case 0 : set_csr(0x342, 0x00000000UL); 
				 set_csr(0x343, pc-4);
				 instructions_executed++;
				 cpu_cycles--;
				 break;
		case 2 : set_csr(0x343, current_instruction);	// 2 - Illegal instruction - save the bad instruction
				 set_csr(0x342, 0x00000002UL);			// save cause
				 break;
		case 3 : set_csr(0x342, 0x00000003UL); break;	// 3 - breakpoint - save cause
		case 4 : set_csr(0x342, 0x00000004UL); 
				 set_csr(0x343, mem_addr + offset);
				 break;
		case 6 : set_csr(0x342, 0x00000006UL); 
				 set_csr(0x343, mem_addr + offset);
				 break;
		case 8 : set_csr(0x342, 0x00000008UL); break;	// 8 - ecall from user mode
		case 11: set_csr(0x342, 0x0000000BUL); break;	// 11 - ecall from machine mode
	}



	// First set mstatus register
	// build new interrupt enable stack
	tmpMstatus = 0x00000000UL;
	if ((mstatus & 0x00000008UL) == 0x00000008UL)	// if MIE is set, then set MPIE
		tmpMstatus = 0x00000080UL;
	if (prv == 3)
		tmpMstatus = (tmpMstatus | 0x00001800UL);
	set_csr(0x300, tmpMstatus);



	// set privilege level to machine (3)
	set_prv(3);
	// mepc - save address of problem instruction
	set_csr(0x341, pc - 4);
	// set PC to mtvec value
	set_pc(mtvec & 0xfffffffcUL);
	// decrement instruction counter as nothing is executed, but increase CPU cycles by 2
	instructions_executed--;
	cpu_cycles += 2;
}


// Clear breakpoint
void processor::clear_breakpoint()
{
	// set breakpoint to end of program
	breakpoint = 0xffffffffUL;
}

// Set breakpoint at an address
void processor::set_breakpoint(uint32_t address)
{
	if(verbose_output)
	{
		cout << "Breakpoint set at " << hex << setw(8) << setfill('0') << address << endl;
	}
	breakpoint = address;
}

bool processor::pending_interrupt()
{
	if (((mip & 0x00000001UL) == (mie & 0x00000001UL)) == ((mstatus & 0x00000004UL) >> 3))
	{
		interrupt(0);
		return true;
	}
	else
		return false;
}

void processor::interrupt(unsigned int cause)
{
	switch(cause)
	{
		case 0 : break;
		case 3 : break;
		case 4 : break;
		case 7 : break;
		case 8 : break;
		case 11: break;
	}

	// First set mstatus register
	// build new interrupt enable stack
	tmpMstatus = 0x00000000UL;
	if ((mstatus & 0x00000008UL) == 0x00000008UL)	// if MIE is set, then set MPIE
		tmpMstatus = 0x00000080UL;
	if (prv == 3)
		tmpMstatus = (tmpMstatus | 0x00001800UL);
	set_csr(0x300, tmpMstatus);



	// set privilege level to machine (3)
	set_prv(3);
	// mepc - save address of problem instruction
	set_csr(0x341, pc - 4);
	// set PC to mtvec value
	set_pc(mtvec & 0xfffffffcUL);
	// decrement instruction counter as nothing is executed, but increase CPU cycles by 2
	//instructions_executed--;
	//cpu_cycles += 2;
}

// Show privilege level - stub in stage 1
void processor::show_prv()
{
	if(prv == 3)
		cout << prv << " (machine)" << endl;
	else if (prv == 0)
		cout << prv << " (user)" << endl;
}

// Set privilege level - stub in stage 1
void processor::set_prv(unsigned int prv_num)
{
	// error priveledge checking done in commands.cpp
	prv = prv_num;
}

// Display CSR value - stub in stage 1
void processor::show_csr(unsigned int csr_num)
{
	switch (csr_num)
	{
		case 0xf11:
			cout << setw(8) << setfill('0') << hex << mvendorid << endl;
			break;
		case 0xf12:
			cout << setw(8) << setfill('0') << hex << marchid	<< endl;
			break;
		case 0xf13:
			cout << setw(8) << setfill('0') << hex << mimpid	<< endl;
			break;
		case 0xf14:
			cout << setw(8) << setfill('0') << hex << mhartid	<< endl;
			break;

		case 0x300:
			cout << setw(8) << setfill('0') << hex << mstatus	<< endl;
			break;
		case 0x301:
			cout << setw(8) << setfill('0') << hex << misa		<< endl;
			break;
		case 0x304:
			cout << setw(8) << setfill('0') << hex << mie		<< endl;
			break;
		case 0x305:
			cout << setw(8) << setfill('0') << hex << mtvec		<< endl;
			break;

		case 0x340:
			cout << setw(8) << setfill('0') << hex << mscratch	<< endl;
			break;
		case 0x341:
			cout << setw(8) << setfill('0') << hex << mepc		<< endl;
			break;
		case 0x342:
			cout << setw(8) << setfill('0') << hex << mcause		<< endl;
			break;
		case 0x343:
			cout << setw(8) << setfill('0') << hex << mtval		<< endl;
			break;
		case 0x344:
			cout << setw(8) << setfill('0') << hex << mip		<< endl;
			break;

		default :
			cout << "Illegal CSR number" << endl;
	}
}

// Set CSR to new value - stub in stage 1
void processor::set_csr(unsigned int csr_num, uint32_t new_value)
{
	switch (csr_num)
	{
		case 0xf11:
			cout << "Illegal write to read-only CSR" << endl;
			break;
		case 0xf12:
			cout << "Illegal write to read-only CSR" << endl;
			break;
		case 0xf13:
			cout << "Illegal write to read-only CSR" << endl;
			break;
		case 0xf14:
			cout << "Illegal write to read-only CSR" << endl;
			break;

		case 0x300:
			// mie, mpie, mpp bits available and uxl fixed at 2 all else fixed at 0
			mstatus = (new_value & 0x1888UL);		
			break;
		case 0x301:
			// writes are legal, but value remains fixed
			break;
		case 0x304:
			// usie, msie, utie, mtie, ueie, meie writeable, other bits fixed at 0
			mie = (new_value & 0x00000999UL);
			break;
		case 0x305:
			// available, careful with base numbers
			if ( (new_value & 0x00000001UL) == 0x00000001UL) // vectored mode
				mtvec = (new_value & 0xffffff81UL);	// careful, BASE is only 30 bits
			else
				mtvec = (new_value & 0xfffffffcUL);
			break;

		case 0x340:
			// defs available
			mscratch = new_value;
			break;
		case 0x341:
			// counter. probably not writeable. lowest 2 bits fixed at 0
			mepc = (new_value & 0xfffffffcUL);
			break;
		case 0x342:
			// only interrupt bit and 4 bit exception code field available. all others 0
				mcause = (new_value & 0x8000000fUL);
			break;
		case 0x343:
			// write with misaligned address, or least significant bits with illegal instruciton
			mtval = new_value;
			break;
		case 0x344:
			// usip, msip, utip, mtip, ueip, meip bits writeable, all others 0
			mip = (new_value & 0x00000999UL);
			break;

		default :
			cout << "Illegal CSR number" << endl;
	}
}

void processor::manipulate_mip(uint32_t new_value)
{
	set_csr(0x344, (new_value & 0x00000111UL));
}

// Set CSR to new value - stub in stage 1
uint32_t processor::get_csr(unsigned int csr_num)
{
	switch (csr_num)
	{
		case 0xf11 : return mvendorid;
		case 0xf12 : return marchid;
		case 0xf13 : return mimpid;
		case 0xf14 : return mhartid;

		case 0x300 : return mstatus;
		case 0x301 : return misa;
		case 0x304 : return mie;
		case 0x305 : return mtvec;

		case 0x340 : return mscratch;
		case 0x341 : return mepc;
		case 0x342 : return mcause;
		case 0x343 : return mtval;
		case 0x344 : return mip;

		default	   : cout << "CSR number not found, returning 0xffffffff" << endl;
	}
	return 0xffffffffUL;
}

bool processor::is_read_only(unsigned int csr_num)
{
	switch(csr_num)
	{
		case 0xf11 : return true;
		case 0xf12 : return true;
		case 0xf13 : return true;
		case 0xf14 : return true;

		default : return false;
	}
}


uint32_t processor::get_instruction_count()
{
	return instructions_executed;
}

// Used for Postgraduate assignment. Undergraduate assignment can return 0.
uint32_t processor::get_cycle_count()
{
	// Returns count of CPU cycles
	return cpu_cycles;
}
