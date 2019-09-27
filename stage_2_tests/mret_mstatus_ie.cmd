# mret pops mstatus interrupt stack correctly

m 8000 = 30200073  # mret
csr 341 = c000 # mepc

csr 300 = 00001800 # mpie = 0, mie = 0
pc = 8000
.
csr 300 # expect 00000080, mpie = 1, mie = 0

csr 300 = 00001880 # mpie = 1, mie = 0
pc = 8000
.
csr 300 # expect 00000088, mpie = 1, mie = 1

csr 300 = 00001808 # mpie = 0, mie = 1
pc = 8000
.
csr 300 # expect 00000080, mpie = 1, mie = 0

csr 300 = 00001888 # mpie = 1, mie = 1
pc = 8000
.
csr 300 # expect 00000088, mpie = 1, mie = 1

