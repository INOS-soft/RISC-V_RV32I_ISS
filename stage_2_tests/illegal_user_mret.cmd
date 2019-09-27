# Illegal instruction trap on mret in user mode

prv = 0  # User mode

csr 305 = 4000  # mtvec, direct mode

m 2000 = 30200073  # mret
pc = 2000
.

pc       # expect 4000
csr 342  # mcause, expect 00000002 (illegal instruction)
csr 341  # mepc, expect 00002000
csr 300  # mstatus, expect 00000000
csr 343  # mtval, expect 30200073
