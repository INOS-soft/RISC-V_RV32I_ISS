# Read all defined CSRs

csr f11	  # mvendorid, expect 00000000
csr F12   # marchid, expect 00000000
csr F13   # mimpid, expect 20190200
csr F14   # mhartid, expect 00000000
csr 300   # mstatus, expect 00000000
csr 301   # misa, expect 40100100
csr 304   # mie, expect 00000000
csr 305   # mtvec, expect 00000000
csr 340   # mscratch, expect 00000000
csr 341   # mepc, expect 00000000
csr 342   # mcause, expect 00000000
csr 343   # mtval, expect 00000000
csr 344   # mip, expect 00000000
