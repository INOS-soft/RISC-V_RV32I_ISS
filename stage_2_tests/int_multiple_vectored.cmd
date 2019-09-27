# User software interrupt (cause 0) in machine mode, vectored

m 1000 = 00000013  # nop
m 1004 = 00000013  # nop
m 2000 = 00000013  # nop
m 2004 = 00000013  # nop
m 2008 = 00000013  # nop
m 200c = 00000013  # nop
m 2010 = 00000013  # nop
m 2014 = 00000013  # nop
m 2018 = 00000013  # nop
m 201c = 00000013  # nop
m 2020 = 00000013  # nop
m 2024 = 00000013  # nop
m 2028 = 00000013  # nop
m 202c = 00000013  # nop
m 2030 = 00000013  # nop
m 2034 = 00000013  # nop
m 2038 = 00000013  # nop
m 203c = 00000013  # nop

csr 305 = 2001  # mtvec, vectored mode

# All enabled; USI, MSI, UTI, MTI, UEI, MEI pending

csr 300 = 00000008  # mpie = 0, mie = 1
csr 304 = 00000999  # set mie.{usie, msie, utie, mtie, ueie, meie}
csr 344 = 00000999  # set mip.{usip, msip, utip, mtip, ueip, meip}
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 2030 (mei has highest priority of pending interrupts)
csr 300  # expect mstatus = 00001880
csr 342  # expect mcause = 8000000B
csr 341  # expect mepc = 00001000

# All enabled; USI, MSI, UTI, MTI, UEI pending

csr 300 = 00000008  # mpie = 0, mie = 1
csr 304 = 00000999  # set mie.{usie, msie, utie, mtie, ueie, meie}
csr 344 = 00000199  # set mip.{usip, msip, utip, mtip, ueip}
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 2010 (msi has highest priority of pending interrupts)
csr 300  # expect mstatus = 00001880
csr 342  # expect mcause = 80000003
csr 341  # expect mepc = 00001000

# All enabled; USI, UTI, MTI, UEI pending

csr 300 = 00000008  # mpie = 0, mie = 1
csr 304 = 00000999  # set mie.{usie, msie, utie, mtie, ueie, meie}
csr 344 = 00000191  # set mip.{usip, utip, mtip, ueip}
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 2020 (mti has highest priority of pending interrupts)
csr 300  # expect mstatus = 00001880
csr 342  # expect mcause = 80000007
csr 341  # expect mepc = 00001000

# All enabled; USI, UTI, UEI pending

csr 300 = 00000008  # mpie = 0, mie = 1
csr 304 = 00000999  # set mie.{usie, msie, utie, mtie, ueie, meie}
csr 344 = 00000111  # set mip.{usip, utip, ueip}
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 2024 (uei has highest priority of pending interrupts)
csr 300  # expect mstatus = 00001880
csr 342  # expect mcause = 80000008
csr 341  # expect mepc = 00001000

# All enabled; USI, UTI pending

csr 300 = 00000008  # mpie = 0, mie = 1
csr 304 = 00000999  # set mie.{usie, msie, utie, mtie, ueie, meie}
csr 344 = 00000011  # set mip.{usip, utip}
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 2004 (usi has highest priority of pending interrupts)
csr 300  # expect mstatus = 00001880
csr 342  # expect mcause = 80000000
csr 341  # expect mepc = 00001000

# All enabled; UTI pending

csr 300 = 00000008  # mpie = 0, mie = 1
csr 304 = 00000999  # set mie.{usie, msie, utie, mtie, ueie, meie}
csr 344 = 00000010  # set mip.{utip}
csr 342 = 00000000  # mcause
csr 341 = 00000000  # mepc
prv = 3

pc = 1000
.

prv      # expect 3
pc       # expect 2014 (uti has highest priority of pending interrupts)
csr 300  # expect mstatus = 00001880
csr 342  # expect mcause = 80000004
csr 341  # expect mepc = 00001000

