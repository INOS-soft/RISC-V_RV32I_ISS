RM=rm -f
AS=riscv64-unknown-elf-as
CC=riscv64-unknown-elf-gcc
LD=riscv64-unknown-elf-ld
OBJCOPY=riscv64-unknown-elf-objcopy
OBJDUMP=riscv64-unknown-elf-objdump
RM=rm -f
ASFLAGS=-march=rv32i
CFLAGS=-march=rv32i -mabi=ilp32
LDFLAGS=-m elf32lriscv
TEXT=0x00000000
DATA=0x00010000

SRCS=$(wildcard *.s)
OBJS=$(subst .s,.o,$(SRCS))
ELFS=$(subst .s,.elf,$(SRCS))
HEXS=$(subst .s,.hex,$(SRCS))
DUMPS=$(subst .s,.dump,$(SRCS))

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

%.elf: %.o
	$(LD) $(LDFLAGS) -Ttext $(TEXT) -Tdata $(DATA) -o $@ $<

%.hex: %.elf
	$(OBJCOPY) --output-target ihex $< $@

%.dump: %.elf
	$(OBJDUMP) -d -M no-aliases $< > $@

all: hex dump

hex: $(HEXS)

dump: $(DUMPS)

clean:
	$(RM) $(OBJS) $(ELFS) $(HEXS) $(DUMPS)
