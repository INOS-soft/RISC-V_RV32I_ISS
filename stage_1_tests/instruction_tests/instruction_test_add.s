	.include "test_defines.h"

	.text
	.align	2
	
	.globl	_start
_start:

	TEST_INIT

	# Test positive/zero value
	li		t0, 0x5A5A5A5A
	add		t1, t0, zero
	TEST_EXPECT	t1, 0x5A5A5A5A

	# Test positive/positive value, no overflow
	li		t0, 0x5A5A5A5A
	li		t1, 16
	add		t2, t0, t1
	TEST_EXPECT	t2, 0x5A5A5A6A

	# Test positive/negative value
	li		t0, 0x5A5A5A5A
	li		t1, -16
	add		t2, t0, t1
	TEST_EXPECT	t2, 0x5A5A5A4A

	# Test positive/positive value, overflow
	li		t0, 0x7FFFFFFF
	li		t1, 16
	add		t2, t0, t1
	TEST_EXPECT	t2, 0x8000000F

	# ################################################################

	# Test negative/zero value
	li		t0, 0xA5A5A5A5
	add		t2, t0, zero
	TEST_EXPECT	t2, 0xA5A5A5A5

	# Test negative/positive value
	li		t0, 0xA5A5A5A5
	li		t1, 16
	add		t2, t0, t1
	TEST_EXPECT	t2, 0xA5A5A5B5
	
	# Test negative/negative value, no overflow
	li		t0, 0xA5A5A5A5
	li		t1, -16
	add		t2, t0, t1
	TEST_EXPECT	t2, 0xA5A5A595

	# Test negative/negative value, overflow
	li		t0, 0x8000000F
	li		t1, -16
	add		t2, t0, t1
	TEST_EXPECT	t2, 0x7FFFFFFF

	# ################################################################

	# Test with x0 as destination
	li		t0, 0x5A5A5A5A
	li		t1, 16
	add		zero, t0, t1
	TEST_EXPECT	zero, 0x00000000
	

	TEST_END
