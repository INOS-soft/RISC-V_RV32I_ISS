# Set writable CSR

csr 340 = 12345678
csr 340

csr 343 = A5A53C3C
csr 343

csr f11	  # mvendorid, expect 00000000
csr F12   # marchid, expect 00000000
csr F13   # mimpid, expect 20190200
csr F14   # mhartid, expect 00000000
csr 300   # mstatus, expect 00000000
csr 301   # misa, expect 40100100
csr 304   # mie, expect 00000000
csr 305   # mtvec, expect 00000000
csr 340   # mscratch, expect 12345678
csr 341   # mepc, expect 00000000
csr 342   # mcause, expect 00000000
csr 343   # mtval, expect A5A53C3C
csr 344   # mip, expect 00000000
