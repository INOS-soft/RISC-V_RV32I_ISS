	.include "test_defines.h"

	.text
	.align	2
	
	.globl	_start
_start:

	TEST_INIT

	# Test positive value, zero shift
	li		t0, 0x5A6B7C8D
	li		t2, 0
	sll		t1, t0, t2
	TEST_EXPECT	t1, 0x5A6B7C8D

	# Test positive value, non-zero shift
	li		t0, 0x5A6B7C8D
	li		t2, 4
	sll		t1, t0, t2
	TEST_EXPECT	t1, 0xA6B7C8D0

	# Test positive value, non-zero shift
	li		t0, 0x5A6B7C8D
	li		t2, 16
	sll		t1, t0, t2
	TEST_EXPECT	t1, 0x7C8D0000

	# Test positive value, non-zero shift, rs2 > 31
	li		t0, 0x5A6B7C8D
	li		t2, 48
	sll		t1, t0, t2
	TEST_EXPECT	t1, 0x7C8D0000

	# Test negative value, zero shift
	li		t0, 0xA5B6C7D8
	li		t2, 0
	sll		t1, t0, t2
	TEST_EXPECT	t1, 0xA5B6C7D8

	# Test negative value, non-zero shift
	li		t0, 0xA5B6C7D8
	li		t2, 4
	sll		t1, t0, t2
	TEST_EXPECT	t1, 0x5B6C7D80

	# Test negative value, non-zero shift
	li		t0, 0xA5B6C7D8
	li		t2, 16
	sll		t1, t0, t2
	TEST_EXPECT	t1, 0xC7D80000

	# Test negative value, non-zero shift
	li		t0, 0xA5B6C7D8
	li		t2, 48
	sll		t1, t0, t2
	TEST_EXPECT	t1, 0xC7D80000

	# ################################################################

	# Test with x0 as destination
	li		t0, 0xA5B6C7D8
	li		t2, 20
	sll		zero, t0, t2
	TEST_EXPECT	zero, 0x00000000
	

	TEST_END
