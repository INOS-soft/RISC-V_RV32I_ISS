# Illegal instruction trap on machine csr access in user mode

prv = 0  # User mode

csr 305 = 4000  # mtvec, direct mode

m 2000 = 34009173  # csrrw x2, x1, mscratch
csr 340 = ABABABAB
x1 = 12345678
x2 = 02020202
pc = 2000
.

pc       # expect 4000
csr 342  # mcause, expect 00000002 (illegal instruction)
csr 341  # mepc, expect 00002000
csr 300  # mstatus, expect 00000000
csr 343  # mtval, expect 34009173

csr 340  # expect ABABABAB
x2       # expect 02020202
