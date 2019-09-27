# Read/write a writable CSR
# csr rs1 001 rd 1110011 CSRRW

m 0 = 34009173  # csrrw x2, x1, mscratch

x1 = 12345678
pc = 0
.

csr 340
x2

x1 = 87654321
pc = 0
.

csr 340
x2

m 0 = 34009073  # csrrw x0, x1, mscratch

x1 = 81812C2C
pc = 0
.

csr 340
x0
