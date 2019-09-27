# mip.msip read only

m 0 = 3440A173  # csrrs x2, x1, mip

csr 344  # expect initial mip = 00000000

x1 = 00000008  # mask for msip
pc = 0
.

csr 344  # expect mip = 00000000
x2       # expect 00000000

