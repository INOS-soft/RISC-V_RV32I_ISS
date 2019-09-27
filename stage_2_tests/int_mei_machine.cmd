# Machine external interrupt (cause 11) in machine mode

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

# MEI disabled, globally disabled

csr 300 = 00000000  # mpie = 0, mie = 0
csr 304 = 00000000  # mie.meie = 0
csr 344 = 00000800  # mip.meip = 1
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 1004
csr 300  # expect mstatus = 00000000
csr 342  # expect mcause = 00000000
csr 341  # expect mepc = 00000000

# non-MEI enabled, globally disabled

csr 300 = 00000000  # mpie = 0, mie = 0
csr 304 = 00000199  # mie.meie = 0, others enabled
csr 344 = 00000800  # mip.meip = 1
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 1004
csr 300  # expect mstatus = 00000000
csr 342  # expect mcause = 00000000
csr 341  # expect mepc = 00000000

# MEI enabled, globally disabled

csr 300 = 00000000  # mpie = 0, mie = 0
csr 304 = 00000800  # mie.meie = 1
csr 344 = 00000800  # mip.meip = 1
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 1004
csr 300  # expect mstatus = 00000000
csr 342  # expect mcause = 00000000
csr 341  # expect mepc = 00000000

# MEI enabled, globally enabled

csr 300 = 00000008  # mpie = 0, mie = 1
csr 304 = 00000800  # mie.meie = 1
csr 344 = 00000800  # mip.meip = 1
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 2004
csr 300  # expect mstatus = 00001880
csr 342  # expect mcause = 8000000B
csr 341  # expect mepc = 00001000

