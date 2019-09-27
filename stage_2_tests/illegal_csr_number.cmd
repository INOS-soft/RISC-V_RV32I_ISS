# Illegal instruction trap on undefined csr number

csr 305 = 4000  # mtvec, direct mode

m 2000 = 30209173  # csrrw x2, x1, medeleg
x1 = 12345678
x2 = 02020202
pc = 2000
.

pc       # expect 4000
csr 342  # mcause, expect 00000002 (illegal instruction)
csr 341  # mepc, expect 00002000
csr 300  # mstatus, expect 00001800
csr 343  # mtval, expect 30209173

x2       # expect 02020202
