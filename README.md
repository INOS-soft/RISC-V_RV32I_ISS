# RISC-V_RV32I_ISS

I designed only the processor and memory classes.
The other files were provided in the course: Computer Architecture, Adelaide Uni, Semester 1, 2019.

Currently the project is set up to run in a unix-like environment.
It can be built with the existing make file and by using the command 'make all'

The program reads one command per line from standard input.
Commands are:
"xn"                Show the content of register xn in hex (n is register number, from 0 to 31). The value is displayed as 8 hex digits with leading 0s.
"xn = value"        Set register xn to value (value in hex).
"pc"                Show content of PC register in hex. The value is displayed as 8 hex digits with leading 0s.
"pc = address"      Set PC register to address (address in hex).
"m address"         Show the content of memory word at address (address in hex, rv32sim rounds it down to nearest word-aligned address). The value is displayed as 8 hex digits with leading 0s.
"m address = value" Set memory word at address to value (address in hex, rv32sim rounds it down to nearest word-aligned address; value in hex).
l "filename"        Load memory from Intel hex format file named filename. If the file includes a start address record, the PC is set to the start address.
.                   Execute one instruction.
. n                 Execute n instructions.
b address           Set an execution breakpoint at address. If the simulator is executing multiple instructions (. n command), it stops when the PC reaches address without executing that instruction. There is only one execution breakpoint; using the b command with a different address removes any previously set breakpoint.
b                   Clear the breakpoint.

For examples see the test files.

WARNING: Interrupt handling not yet complete.
