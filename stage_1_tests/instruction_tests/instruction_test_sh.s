	.include "test_defines.h"

	.text
	.align	2
	
	.globl	_start
_start:

	TEST_INIT

	li		t2, 0xb0a09080

	# Zero offset, base word aligned

	la		t0, mem01+8
	sh		t2, 0(t0)
	la		t3, mem01
	lw		t1, 8(t3)
	TEST_EXPECT	t1, 0x3b3a9080

	# Positive offset, base word aligned

	la		t0, mem02+8
	sh		t2, 2(t0)
	la		t3, mem02
	lw		t1, 8(t3)
	TEST_EXPECT	t1, 0x90803938

	# Negative offset, base word aligned

	la		t0, mem03+8
	sh		t2, -2(t0)
	la		t3, mem03
	lw		t1, 4(t3)
	TEST_EXPECT	t1, 0x90803534

	# Zero offset, base not word aligned

	la		t0, mem04+6
	sh		t2, 0(t0)
	la		t3, mem04
	lw		t1, 4(t3)
	TEST_EXPECT	t1, 0x90803534

	# Positive offset, base not word aligned

	la		t0, mem05+9
	sh		t2, 1(t0)
	la		t3, mem05
	lw		t1, 8(t3)
	TEST_EXPECT	t1, 0x90803938

	# Negative offset, base not word aligned

	la		t0, mem06+13
	sh		t2, -1(t0)
	la		t3, mem06
	lw		t1, 12(t3)
	TEST_EXPECT	t1, 0x3f3e9080


	TEST_END

	.data
	.align	2
	
mem01:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem02:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem03:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem04:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem05:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem06:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem11:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem12:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem13:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem14:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
mem15:	.byte	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37
	.byte	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
	
