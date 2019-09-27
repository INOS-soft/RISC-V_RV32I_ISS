	.include "test_defines.h"

	.text
	.align	2
	
	.globl	_start
_start:

	TEST_INIT

	# Load positive value, zero offset, base word aligned

	la		t0, pos+8
	lw		t1, 0(t0)
	TEST_EXPECT	t1, 0x3b3a3938

	# Load positive value, positive offset, base word aligned

	la		t0, pos+8
	lw		t1, 4(t0)
	TEST_EXPECT	t1, 0x3f3e3d3c

	# Load positive value, negative offset, base word aligned

	la		t0, pos+8
	lw		t1, -4(t0)
	TEST_EXPECT	t1, 0x37363534

	# Load positive value, positive offset, base not word aligned

	la		t0, pos+5
	lw		t1, 3(t0)
	TEST_EXPECT	t1, 0x3b3a3938

	# Load positive value, negative offset, base not word aligned

	la		t0, pos+5
	lw		t1, -1(t0)
	TEST_EXPECT	t1, 0x37363534

	# ################################################################

	# Load negative value, zero offset, base word aligned

	la		t0, neg+8
	lw		t1, 0(t0)
	TEST_EXPECT	t1, 0xfbfaf9f8

	# Load negative value, positive offset, base word aligned

	la		t0, neg+8
	lw		t1, 4(t0)
	TEST_EXPECT	t1, 0xfffefdfc

	# Load negative value, negative offset, base word aligned

	la		t0, neg+8
	lw		t1, -4(t0)
	TEST_EXPECT	t1, 0xf7f6f5f4

	# Load negative value, positive offset, base not word aligned

	la		t0, neg+3
	lw		t1, 5(t0)
	TEST_EXPECT	t1, 0xfbfaf9f8

	# Load negative value, negative offset, base not word aligned

	la		t0, neg+3
	lw		t1, -3(t0)
	TEST_EXPECT	t1, 0xf3f2f1f0


	TEST_END

	.data
	.align	2
	
pos:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
neg:	.byte	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7
	.byte	0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
	
	
