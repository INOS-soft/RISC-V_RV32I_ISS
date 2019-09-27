# lui instruction format: iiii iiii iiii iiii iiii dddd d011 0111

# Test with positive immediate
x1 = 11111111
m 00002000 = 5A5A6137	# lui x2, 0x5A5A6
pc = 00002000
.
x2	# expect 0x5A5A6000

# Test with negative immediate
x1 = 11111111
m 00002010 = A5A53137	# lui x2, 0xA5A53
pc = 00002010
.
x2	# expect 0xA5A53000

