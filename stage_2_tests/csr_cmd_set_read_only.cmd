# Set read only CSR gives error

csr f11 = ffffffff
csr f11 # expect 00000000

csr f12 = ffffffff
csr f12 # expect 00000000

csr f13 = dfe6fdff
csr f13 # expect 20190200

csr f14 = ffffffff
csr f14 # expect 00000000
