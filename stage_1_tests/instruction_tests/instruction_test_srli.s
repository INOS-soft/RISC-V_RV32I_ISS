	.include "test_defines.h"

	.text
	.align	2
	
	.globl	_start
_start:

	TEST_INIT

	# Test positive value, zero shift
	li		t0, 0x5A6B7C8D
	srli		t1, t0, 0
	TEST_EXPECT	t1, 0x5A6B7C8D

	# Test positive value, non-zero shift
	li		t0, 0x5A6B7C8D
	srli		t1, t0, 4
	TEST_EXPECT	t1, 0x05A6B7C8

	# Test positive value, non-zero shift
	li		t0, 0x5A6B7C8D
	srli		t1, t0, 16
	TEST_EXPECT	t1, 0x00005A6B

	# Test negative value, zero shift
	li		t0, 0xA5B6C7D8
	srli		t1, t0, 0
	TEST_EXPECT	t1, 0xA5B6C7D8

	# Test negative value, non-zero shift
	li		t0, 0xA5B6C7D8
	srli		t1, t0, 4
	TEST_EXPECT	t1, 0x0A5B6C7D

	# Test negative value, non-zero shift
	li		t0, 0xA5B6C7D8
	srli		t1, t0, 16
	TEST_EXPECT	t1, 0x0000A5B6

	# ################################################################

	# Test with x0 as destination
	li		t0, 0xA5B6C7D8
	srli		zero, t0, 20
	TEST_EXPECT	zero, 0x00000000
	

	TEST_END
