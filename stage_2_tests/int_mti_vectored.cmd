# Machine timer interrupt (cause 7) in machine mode, vectored

m 1000 = 00000013  # nop
m 1004 = 00000013  # nop
m 2000 = 00000013  # nop
m 2004 = 00000013  # nop
m 2008 = 00000013  # nop
m 200c = 00000013  # nop
m 2010 = 00000013  # nop
m 2014 = 00000013  # nop
m 2018 = 00000013  # nop
m 201c = 00000013  # nop
m 2020 = 00000013  # nop
m 2024 = 00000013  # nop
m 2028 = 00000013  # nop
m 202c = 00000013  # nop
m 2030 = 00000013  # nop
m 2034 = 00000013  # nop
m 2038 = 00000013  # nop
m 203c = 00000013  # nop

csr 305 = 2001  # mtvec, vectored mode

# MTI enabled, globally enabled

csr 300 = 00000008  # mpie = 0, mie = 1
csr 304 = 00000080  # mie.mtie = 1
csr 344 = 00000080  # mip.mtip = 1
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 2020
csr 300  # expect mstatus = 00001880
csr 342  # expect mcause = 80000007
csr 341  # expect mepc = 00001000

