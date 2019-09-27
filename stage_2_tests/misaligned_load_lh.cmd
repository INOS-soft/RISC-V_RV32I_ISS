# Misaligned address during lh/lhu load instruction

csr 305 = 4000  # mtvec, direct mode

m 2000 = 00009103  # lh x2, 0(x1)
x2 = 0
m 1000 = FEDCBA98

################

x1 = 1001
pc = 2000
.

pc       # expect 4000
csr 342  # mcause, expect 00000004 (load address misaligned)
csr 341  # mepc, expect 00002000
csr 300  # mstatus, expect 00001800
csr 343  # mtval, expect 00001001
x2       # expect 0

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

x2       # expect FFFFFEDC

csr 342 = 0  # reset mcause
csr 341 = 0  # reset mepc
csr 300 = 0  # reset mstatus
csr 343 = 0  # reset mtval

################

m 2000 = 0000D103  # lhu x2, 0(x1)
x2 = 0

x1 = 1001
pc = 2000
.

pc       # expect 4000
csr 342  # mcause, expect 00000004 (load address misaligned)
csr 341  # mepc, expect 00002000
csr 300  # mstatus, expect 00001800
csr 343  # mtval, expect 00001001
x2       # expect 0

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

x2       # expect 0000FEDC
