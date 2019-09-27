# Machine software interrupt (cause 3) in user mode

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

csr 305 = 2000  # mtvec, direct mode

# MSI disabled, globally enabled due to being in user mode

csr 300 = 00000000  # mpie = 0, mie = 0
csr 304 = 00000000  # mie.msie = 0
csr 344 = 00000008  # mip.msip = 1
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 0

pc = 1000
.

prv      # expect 0
pc       # expect 1004
csr 300  # expect mstatus = 00000000
csr 342  # expect mcause = 00000000
csr 341  # expect mepc = 00000000

# non-MSI enabled, globally enabled due to being in user mode

csr 300 = 00000000  # mpie = 0, mie = 0
csr 304 = 00000991  # mie.msie = 0, others enabled
csr 344 = 00000008  # mip.msip = 1
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 0

pc = 1000
.

prv      # expect 0
pc       # expect 1004
csr 300  # expect mstatus = 00000000
csr 342  # expect mcause = 00000000
csr 341  # expect mepc = 00000000

# MSI enabled, globally enabled due to being in user mode

csr 300 = 00000000  # mpie = 0, mie = 0
csr 304 = 00000008  # mie.msie = 1
csr 344 = 00000008  # mip.msip = 1
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 0

pc = 1000
.

prv      # expect 3
pc       # expect 2004
csr 300  # expect mstatus = 00000000
csr 342  # expect mcause = 80000003
csr 341  # expect mepc = 00001000

# MSI enabled, globally enabled and in user mode

csr 300 = 00000008  # mpie = 0, mie = 1
csr 304 = 00000008  # mie.msie = 1
csr 344 = 00000008  # mip.msip = 1
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 0

pc = 1000
.

prv      # expect 3
pc       # expect 2004
csr 300  # expect mstatus = 00000080
csr 342  # expect mcause = 80000003
csr 341  # expect mepc = 00001000

