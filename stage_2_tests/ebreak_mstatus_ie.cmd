# ebreak trap pushes interrupt enable stack correctly

m 1000 = 00100073  # ebreak
csr 305 = 2000  # mtvec, direct mode

# Machine mode

csr 300 = 00000000  # mpie = 0, mie = 0

pc = 1000
.

csr 300  # mstatus, expect 00001800

csr 300 = 00000008  # mpie = 0, mie = 1

pc = 1000
.

csr 300  # mstatus, expect 00001880

csr 300 = 00000088  # mpie = 1, mie = 1

pc = 1000
.

csr 300  # mstatus, expect 00001880

# User mode

csr 300 = 00000000  # mpie = 0, mie = 0
prv = 0

pc = 1000
.

csr 300  # mstatus, expect 00000000

csr 300 = 00000008  # mpie = 0, mie = 1
prv = 0

pc = 1000
.

csr 300  # mstatus, expect 00000080

csr 300 = 00000088  # mpie = 1, mie = 1
prv = 0

pc = 1000
.

csr 300  # mstatus, expect 00000080
