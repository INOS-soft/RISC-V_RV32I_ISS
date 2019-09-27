# Read/set a writable CSR
# csr rs1 010 rd 1110011 CSRRS

m 0 = 3400A173  # csrrs x2, x1, mscratch

csr 340 = 01018080
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

m 0 = 3400A073  # csrrs x0, x1, mscratch

x1 = 40002000
pc = 0
.

csr 340
x0
