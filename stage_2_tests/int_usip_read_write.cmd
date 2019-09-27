# mip.usip read only

m 0 = 3440A173  # csrrs x2, x1, mip
m 4 = 3440B173  # csrrc x2, x1, mip

csr 344  # expect initial mip = 00000000

x1 = 00000001  # mask for usip
pc = 0
.

csr 344  # expect mip = 00000001
x2       # expect 00000000

pc = 4
.

csr 344  # expect mip = 00000000
x2       # expect 00000001

