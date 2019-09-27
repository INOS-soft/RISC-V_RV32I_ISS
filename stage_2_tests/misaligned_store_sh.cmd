# Misaligned address during sh store instruction

csr 305 = 4000  # mtvec, direct mode

m 2000 = 00209023  # sh x2, 0(x1)
x2 = FEDCBA98
m 1000 = 33221100

################

x1 = 1001
pc = 2000
.

pc       # expect 4000
csr 342  # mcause, expect 00000006 (store address misaligned)
csr 341  # mepc, expect 00002000
csr 300  # mstatus, expect 00001800
csr 343  # mtval, expect 00001001
m 1000   # expect 7766554433221100

csr 342 = 0  # reset mcause
csr 341 = 0  # reset mepc
csr 300 = 0  # reset mstatus
csr 343 = 0  # reset mtval

################

x1 = 1002
pc = 2000
.

pc       # expect 2004
csr 342  # mcause, expect 00000000
csr 341  # mepc, expect 00000000
csr 300  # mstatus, expect 00000000
csr 343  # mtval, expect 00000000

m 1000   # expect BA981100
