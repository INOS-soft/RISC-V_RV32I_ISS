# Set unused bits of a writable CSR

# mstatus: only mie, mpie, mpp implemented
csr 300 = ffffffff
csr 300 # expect 00001888

# misa: all bits fixed
csr 301 = bfeffeff
csr 301 # expect 40100100

# mie: only usie, msie, utie, mtie, ueie, meie implemented
csr 304 = ffffffff
csr 304 # expect 00000999

# mtvec: bit 1 fixed at 0, if vectored, bits 7:2 also fixed at 0

csr 305 = fffffffe  # Direct mode
csr 305 # expect fffffffc
csr 305 = 00000000
csr 305 # expect 00000000
csr 305 = ffffffff  # Vectored mode
csr 305 # expect ffffff81

# mscratch: all bits writable

csr 340 = ffffffff
csr 340 # expect ffffffff

# mepc: bit 1:0 fixed at 0

csr 341 = ffffffff
csr 341 # expect fffffffc

# mcause: only Interrupt bit and 4-bit cause

csr 342 = ffffffff
csr 342 # expect 8000000f

# mtval: all bits writable

csr 343 = ffffffff
csr 343 # expect ffffffff

# mip: only usip, msip, utip, mtip, ueip, meip implemented
csr 344 = ffffffff
csr 344 # expect 00000999
