/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2019

   Class for processor

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
				}

				else if (offset == 1)
				{
					mask = 0x00ffff00UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 8;
				}
				else if (offset == 2)
				{
					mask = 0xffff0000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 16;
				}

				else if (offset == 3)
				{
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
				}

				// sign extend
				if((tmp_imm_32 & 0x00008000UL) == 0x00008000UL)
					tmp_imm_32 = tmp_imm_32 | 0xffff0000UL;

				set_reg(tmp_rd, tmp_imm_32);
			}

			else if(tmp_funct3 == 2)
			{
				// lw instruction
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
				}

				else if (offset == 1)
				{
					mask = 0x00ffff00UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 8;
				}
				else if (offset == 2)
				{
					mask = 0xffff0000UL;
					tmp_imm_32 = (tmp_imm_32 & mask) >> 16;
				}

				else if (offset == 3)
				{
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
				}

				set_reg(tmp_rd, tmp_imm_32);
			}
		}

		else if(op_code == i_type_j)
		{
			// jalr
			tmp_rs1 = (current_instruction & rs1_bits) >> 15;
			tmp_rd = (current_instruction & rd_bits) >> 7;
			tmp_imm_32 = (current_instruction & i_type_imm_bits) >> 20;
			if((current_instruction & 0x80000000UL) == 0x80000000UL)
				tmp_imm_32 = (tmp_imm_32 | 0xfffff000UL);
			tmp_imm_32 = (tmp_imm_32 + x[tmp_rs1]) & 0xfffffffeUL;
			set_reg(tmp_rd, pc);
			set_pc(tmp_imm_32);
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

			if(tmp_funct3 == 1)
			{
				// sh instruction
				// sb instruction
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
					tmp_imm_32 = (tmp_imm_32 << 8);
					mem->write_word(mem_addr, tmp_imm_32, 0x00ffff00UL);
				}

				else if (offset == 2)
				{
					tmp_imm_32 = (tmp_imm_32 << 16);
					mem->write_word(mem_addr, tmp_imm_32, 0xffff0000UL);
				}

				else if (offset == 3)
				{
					cpu_cycles += 2;

					tmp_imm_32_2 = (tmp_imm_32 & 0x0000ff00UL) >> 8;
					tmp_imm_32 = (tmp_imm_32 << 24);
					mem->write_word(mem_addr, tmp_imm_32, 0xff000000UL);

					mem_addr = mem_addr + 4;
					mem->write_word(mem_addr, tmp_imm_32_2, 0x000000ffUL);	
				}

				else
					cout << "Error in save: offset out of bounds." << endl;
			}

			if(tmp_funct3 == 2)
			{
				// sw instruction
				if(verbose_output)
				{
					cout << "sw: rs1 = " << dec << tmp_rs1 << ", rs2 = " << tmp_rs2
						<< ", immed_S = " << hex << setw(8) << setfill('0') << immed_S << endl;
				}
				tmp_imm_32 = x[tmp_rs2];
				mem->write_word(mem_addr, tmp_imm_32, 0xffffffffUL);
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

			if(tmp_funct3 == 1)
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

			if(tmp_funct3 == 4)
			{
				// BLT
				if((int32_t)x[tmp_rs1] < (int32_t)x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
			}

			if(tmp_funct3 == 5)
			{
				// BGE
				if((int32_t)x[tmp_rs1] >= (int32_t)x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
			}

			if(tmp_funct3 == 6)
			{
				// BLTU
				if(x[tmp_rs1] < x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
			}

			if(tmp_funct3 == 7)
			{
				// BGEU
				if(x[tmp_rs1] >= x[tmp_rs2])
				{
					tmp_imm_32 = tmp_imm_32 + pc - 4;
					set_pc(tmp_imm_32);
					cpu_cycles++;
				}
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

		else if(op_code == ecall_type)
		{
			// ecall instruction
			cpu_cycles--;
			if(current_instruction == ecall_type)
				cout << "ecall: not implemented" << endl;

			else if (current_instruction == ebreak_type)
				cout << "ebreak: not implemented" << endl;
			else
			{
				tmp_funct3 = (current_instruction & funct3_bits) >> 12;
				if(tmp_funct3 != 0)
					cout << "Error: illegal instruction" << endl;
			}
		}

		else
		{
			//cerr << "OP-Code " << hex << op_code << " not recognized." << endl;
			break;
		}
		
		// increment instruction count
		instructions_executed++;
	}
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
		cout << "Brakpoint set at " << hex << setw(8) << setfill('0') << address << endl;
	}
	breakpoint = address;
}

// Show privilege level - stub in stage 1
void processor::show_prv()
{

}

// Set privilege level - stub in stage 1
void processor::set_prv(unsigned int prv_num)
{

}

// Display CSR value - stub in stage 1
void processor::show_csr(unsigned int csr_num)
{

}

// Set CSR to new value - stub in stage 1
void processor::set_csr(unsigned int csr_num, uint32_t new_value)
{

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