# Read/clear a writable CSR
# csr rs1 011 rd 1110011 CSRRC

m 0 = 3400B173  # csrrc x2, x1, mscratch

csr 340 = FEFE7F7F
x1 = 80804040
pc = 0
.

csr 340
x2

x1 = A0A01010
pc = 0
.

csr 340
x2

x1 = 0
pc = 0
.

csr 340
x2

m 0 = 3400B073  # csrrc x0, x1, mscratch

x1 = 40002000
pc = 0
.

csr 340
x0
