# bne instruction format: iiii iiis ssss ssss s001 iiii i110 0011
#                         1100 0002 2222 1111 1    0000 1 
#                         2098 765                 4321 1

# Test taken with positive displacement
# bne x1, x2, +6
# disp = 0 0000 0000 011(0)
# 0000 0000 0010 0000 1001 0011 0110 0011

x1 = 11111111
x2 = 22222222
m 00002000 = 00209363
pc = 00002000
.
pc    # expect 0x00002006
x1    # expect unchanged
x2    # expect unchanged

# Test not taken with positive displacement
# bne x1, x2, +6
# disp = 0 0000 0000 011(0)
# 0000 0000 0010 0000 1001 0011 0110 0011

x1 = 11111111
x2 = 11111111
m 00002010 = 00209363
pc = 00002010
.
pc    # expect 0000002014
x1    # expect unchanged
x2    # expect unchanged

# Test taken with negative displacement
# bne x1, x2, -6
# disp = 1 1111 1111 101(0)
# 1111 1110 0010 0000 1001 1101 1110 0011

x1 = 11111111
x2 = 22222222
m 00002020 = FE209DE3
pc = 00002020
.
pc    # expect 0x000201A
x1    # expect unchanged
x2    # expect unchanged

# Test not taken with negative displacement
# bne x1, x2, -6
# disp = 1 1111 1111 101(0)
# 1111 1110 0010 0000 1001 1101 1110 0011

x1 = 11111111
x2 = 11111111
m 00002030 = FE209DE3
pc = 00002030
.
pc    # expect 0x0002034
x1    # expect unchanged
x2    # expect unchanged
