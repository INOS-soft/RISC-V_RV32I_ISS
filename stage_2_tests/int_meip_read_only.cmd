# mip.meip read only

m 0 = 3440A173  # csrrs x2, x1, mip

csr 344  # expect initial mip = 00000000

x1 = 0000000000000800  # mask for meip
pc = 0
.

csr 344  # expect mip = 00000000
x2       # expect 00000000

