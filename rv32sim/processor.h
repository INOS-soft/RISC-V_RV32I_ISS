#ifndef PROCESSOR_H
#define PROCESSOR_H

/* ****************************************************************
   RISC-V Instruction Set Simulator
   Computer Architecture, Semester 1, 2019

   Class for processor
   
   Written by:
   Nathanael Whitrow
   For feedback, comments and questions please contact me at:
   nathanael.whitrow@gmail.com

**************************************************************** */

#include "memory.h"

using namespace std;

class processor {

  private:

    // constructor arguments
    memory * mem;
    bool verbose_output;
    bool stage2_check;

    // Processor hardware
    uint32_t    x[32] = {};                // registers
    uint32_t    pc = 0;                    // program counter
    int         prv = 3;

    // CSR hardware
    uint32_t mvendorid  = 0x00000000UL;     // f11
    uint32_t marchid    = 0x00000000UL;     // f12
    uint32_t mimpid     = 0x20190200UL;     // f13
    uint32_t mhartid    = 0x00000000UL;     // f14

    uint32_t tmpMstatus = 0x00000000UL;
    uint32_t mstatus    = 0x00000000UL;     // 300 Writes legal, some bits fixed
    uint32_t misa       = 0x40100100UL;     // 301 Writes legal, all bits remain fixed
    uint32_t mie        = 0x00000000UL;     // 304 Writes legal, some bits fixed
    uint32_t mtvec      = 0x00000000UL;     // 305 Writes legal

    uint32_t mscratch   = 0x00000000UL;     // 340 Writes legal
    uint32_t mepc       = 0x00000000UL;     // 341 Writes legal
    uint32_t mcause     = 0x00000000UL;     // 342 Writes legal
    uint32_t mtval      = 0x00000000UL;     // 343 Writes legal
    uint32_t mip        = 0x00000000UL;     // 344 Writes lega, some bits fixed

/*
    csr[0xf11] = 0x00000000UL;      // mvendorid
    csr[0xf12] = 0x00000000UL;      // marchid
    csr[0xf13] = 0x20190200UL;      // mimpid
    csr[0xf14] = 0x00000000UL;      // mhartid
    csr[0x300] = 0x00000000UL;      // mstatus
    csr[0x301] = 0x40100100UL;      // misa
    csr[0x304] = 
*/
    // ISS program stuff
    uint32_t breakpoint = 0xffffffffUL; // start bp at end of program
    uint32_t instructions_executed = 0;
    uint32_t cpu_cycles = 0;
    
    // Temporary storage for decoding
    uint32_t current_instruction = 0;
    uint32_t op_code = 0;
    uint32_t tmp_imm_32 = 0;
    //uint32_t tmp_imm_32_2 = 0;
    uint32_t immed_I = 0;
    uint32_t immed_S = 0;
    uint32_t mem_addr = 0;
    uint32_t mask = 0;
    unsigned int offset = 0;

    unsigned int tmp_rd = 0;
    unsigned int tmp_rs1 = 0;
    unsigned int tmp_rs2 = 0;
    unsigned int tmp_funct3 = 0;
    unsigned int tmp_funct7 = 0;

    // temporary:
    uint32_t bits_4_1 = 0;
    uint32_t bits_4_0 = 0;
    uint32_t bits_11_5 = 0;
    uint32_t bits_10_5 = 0;
    uint32_t bits_11 = 0;
    uint32_t bits_12 = 0;
    uint32_t bits_10_1 = 0;
    uint32_t bits_19_12 = 0;
    uint32_t bits_20 = 0;


    // hard-coded formats for instruction types
    // used for bitwise operations and comparison in decoding current_instruction
    uint32_t opcode_bits =      0x0000007fUL;
    uint32_t rd_bits =          0x00000f80UL;
    uint32_t funct3_bits =      0x00007000UL;
    uint32_t funct7_bits =      0xfe000000UL;
    uint32_t rs1_bits =         0x000f8000UL;
    uint32_t rs2_bits =         0x01f00000UL;
    uint32_t i_type_imm_bits =  0xfff00000UL;
    uint32_t i_type_imm_4_0 =   0x01f00000UL;
    uint32_t s_imm_11_to_5 =    0xfe000000UL;
    uint32_t s_imm_4_to_0 =     0x00000f80UL;
    // so many b-type immediates...
    uint32_t b_type_imm_12 =    0x80000000UL;
    uint32_t b_type_imm_10_5 =  0x7e000000UL;
    uint32_t b_type_imm_4_1 =   0x00000f00UL;
    uint32_t b_type_imm_11 =    0x00000080UL;
    // just a single u-type
    uint32_t u_type_imm_bits =  0xfffff000UL;
    // j_type is kinda funky...
    uint32_t j_type_imm_20 =    0x80000000UL;
    uint32_t j_type_imm_10_1 =  0x7fe00000UL;
    uint32_t j_type_imm_11 =    0x00100000UL;
    uint32_t j_type_imm_19_12 = 0x000ff000UL;

    // All the op-codes for comparison
    uint32_t r_type_a =     0x00000033UL;
    uint32_t i_type_l =     0x00000003UL;
    uint32_t i_type_j =     0x00000067UL;
    uint32_t i_type_a =     0x00000013UL;
    uint32_t s_type   =     0x00000023UL;
    uint32_t b_type   =     0x00000063UL;
    // u_type has only 2 commands each with unique op-codes:
    uint32_t lui_type =     0x00000037UL;
    uint32_t auipc_type =   0x00000017UL;
    // j_type has only one command with a unique op-code:
    uint32_t jal_type =     0x0000006fUL;
    uint32_t fence_type =   0x0000000fUL;
    uint32_t system =       0x00000073UL;
    uint32_t ecall_type =   0x00000073UL;
    uint32_t ebreak_type =  0x00100073UL;
    uint32_t mret_type =    0x30200073UL;

  public:

    // Consructor
    processor(memory* main_memory, bool verbose, bool stage2);

    // Display PC value
    void show_pc();

    // Set PC to new value
    void set_pc(uint32_t new_pc);

    // Display register value
    void show_reg(unsigned int reg_num);

    // Set register to new value
    void set_reg(unsigned int reg_num, uint32_t new_value);

    // Execute a number of instructions
    void execute(unsigned int num, bool breakpoint_check);

    // called when an exception is encountered
    void exception(unsigned int cause);

    // Clear breakpoint
    void clear_breakpoint();

    // Set breakpoint at an address
    void set_breakpoint(uint32_t address);

    // Show privilege level - stub in stage 1
    void show_prv();

    // Set privilege level - stub in stage 1
    void set_prv(unsigned int prv_num);

    // Display CSR value - stub in stage 1
    void show_csr(unsigned int csr_num);

    // Set CSR to new value - stub in stage 1
    void set_csr(unsigned int csr_num, uint32_t new_value);

    // return a CSR register value
    uint32_t get_csr(unsigned int csr_num);

    // used for changing mip
    void manipulate_mip(uint32_t new_value);

    bool pending_interrupt();

    void interrupt(unsigned int cause);

    // returns true if csr is read only
    bool is_read_only(unsigned int csr_num);

    uint32_t get_instruction_count();

    // Used for Postgraduate assignment. Undergraduate assignment can return 0.
    uint32_t get_cycle_count();

};

#endif
