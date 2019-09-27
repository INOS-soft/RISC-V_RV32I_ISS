# ecall instruction executed in machine mode

prv = 3  # Machine mode

csr 305 = 4000  # mtvec, direct mode

m 2000 = 00000073  # ecall
pc = 2000
.

pc       # expect 4000
csr 342  # mcause, expect 0000000B (ecall from machine mode)
csr 341  # mepc, expect 00002000
csr 300  # mstatus, expect 00001800
csr 343  # mtval, expect 00000000
