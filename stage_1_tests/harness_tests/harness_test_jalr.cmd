# jalr instruction format: iiii iiii iiii ssss s000 dddd d110 0111
#                          1100 0000 0000
#                          1098 7654 3210

# ################################################################

# Test with positive displacement
# jal x1, x2, +6
# disp = 0000 0000 0110
# 0000 0000 0110 0001 0000 0000 1110 0111

x1 = 11111111
x2 = 00800000
m 00002000 = 006100E7
pc = 00002000
.
pc    # expect 0x00800006
x1    # expect 0x00002004
x2    # expect 0x00800000

# Test with unaligned positive displacement
# jalr x1, x2, +5
# disp = 0000 0000 0101
# 0000 0000 0101 0001 0000 0000 1110 0111

x1 = 11111111
x2 = 00800000
m 00002010 = 005100E7
pc = 00002010
.
pc    # expect 0x00800004
x1    # expect 0x00002014
x2    # expect 0x00800000

# Test with positive displacement, rs1 is x0
# jalr x1, x0, +6
# disp = 0000 0000 0110
# 0000 0000 0110 0000 0000 0000 1110 0111

x1 = 11111111
m 00002020 = 006000E7
pc = 00002020
.
pc    # expect 0x00000006
x1    # expect 0x00002024

# Test with positive displacement, rd is x0
# jalr x0, x2, +6
# disp = 0000 0000 0110
# 0000 0000 0110 0001 0000 0000 0110 0111

x2 = 00800000
m 00002030 = 00610067
pc = 00002030
.
pc    # expect 0x00800006
x0    # expect unchanged
x2    # expect 0x00800000

# ################################################################

# Test with negative displacement
# jalr x1, x2, -6
# disp = 1111 1111 1010
# 1111 1111 1010 0001 0000 0000 1110 0111

x1 = 11111111
x2 = 00800000
m 00002040 = FFA100E7
pc = 00002040
.
pc    # expect 0x007FFFFA
x1    # expect 0x00002044
x2    # expect 0x00800000

# Test with unaligned negative displacement
# jalr x1, x2, -5
# disp = 1111 1111 1011
# 1111 1111 1011 0001 0000 0000 1110 0111

x1 = 11111111
x2 = 00800000
m 00002050 = FFB100E7
pc = 00002050
.
pc    # expect 0x007FFFFA
x1    # expect 0x00002054
x2    # expect 0x00800000

# Test with negative displacement, rs1 is x0
# jalr x1, x0, -6
# disp = 1111 1111 1010
# 1111 1111 1010 0000 0000 0000 1110 0111

x1 = 11111111
m 00002060 = FFA000E7
pc = 00002060
.
pc    # expect 0xFFFFFFFA
x1    # expect 0x00002064

# Test with negative displacement, rd is x0
# jalr x0, x2, -6
# disp = 1111 1111 1010
# 1111 1111 1010 0001 0000 0000 0110 0111

x2 = 00800000
m 00002070 = FFA10067
pc = 00002070
.
pc    # expect 0x007FFFFA
x0    # expect unchanged
x2    # expect 0x00800000
