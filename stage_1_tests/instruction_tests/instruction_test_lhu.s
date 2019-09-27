	.include "test_defines.h"

	.text
	.align	2
	
	.globl	_start
_start:

	TEST_INIT

	# Load positive value, zero offset, base word aligned

	la		t0, pos+8
	lhu		t1, 0(t0)
	TEST_EXPECT	t1, 0x00003938

	# Load positive value, positive offset, base word aligned

	la		t0, pos+8
	lhu		t1, 2(t0)
	TEST_EXPECT	t1, 0x00003b3a

	# Load positive value, negative offset, base word aligned

	la		t0, pos+8
	lhu		t1, -2(t0)
	TEST_EXPECT	t1, 0x00003736

	# Load positive value, zero offset, base not word aligned

	la		t0, pos+6
	lhu		t1, 0(t0)
	TEST_EXPECT	t1, 0x00003736

	# Load positive value, positive offset, base not word aligned

	la		t0, pos+5
	lhu		t1, 3(t0)
	TEST_EXPECT	t1, 0x00003938

	# Load positive value, negative offset, base not word aligned

	la		t0, pos+5
	lhu		t1, -3(t0)
	TEST_EXPECT	t1, 0x00003332

	# ################################################################

	# Load negative value, zero offset, base word aligned

	la		t0, neg+8
	lhu		t1, 0(t0)
	TEST_EXPECT	t1, 0x0000f9f8

	# Load negative value, positive offset, base word aligned

	la		t0, neg+8
	lhu		t1, 2(t0)
	TEST_EXPECT	t1, 0x0000fbfa

	# Load negative value, negative offset, base word aligned

	la		t0, neg+8
	lhu		t1, -2(t0)
	TEST_EXPECT	t1, 0x0000f7f6

	# Load negative value, zero offset, base not word aligned

	la		t0, neg+6
	lhu		t1, 0(t0)
	TEST_EXPECT	t1, 0x0000f7f6

	# Load negative value, positive offset, base not word aligned

	la		t0, neg+5
	lhu		t1, 5(t0)
	TEST_EXPECT	t1, 0x0000fbfa

	# Load negative value, negative offset, base not word aligned

	la		t0, neg+5
	lhu		t1, -5(t0)
	TEST_EXPECT	t1, 0x0000f1f0


	TEST_END

	.data
	.align	2
	
pos:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
neg:	.byte	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7
	.byte	0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
	
	
