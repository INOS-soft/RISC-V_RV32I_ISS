# addi instruction format: iiii iiii iiii ssss s000 dddd d001 0011

# Tests with different source and destination registers

# Test with positive immediate
x1 = 11111000
m 00002000 = 5A608113	# addi x2, x1, 0x5A6
pc = 00002000
.
x2	# expect 111115A6

# Test with negative immediate
x1 = 11111000
m 00002010 = A5308113	# addi x2, x1, 0xA53
pc = 00002010
.
x2	# expect 11110A53

# Tests with same source and destination registers

# Test with positive immediate
x9 = 11111000
m 00002020 = 5A648493	# addi x9, x9, 0x5A6
pc = 00002020
.
x9	# expect 111115A6

# Test with negative immediate
x9 = 11111000
m 00002030 = A5348493	# addi x9, x9, 0xA53
pc = 00002030
.
x9	# expect 11110A53

