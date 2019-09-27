/* Random number generator based on
   Pseudo-Random Sequence Generator for 32-Bit CPUs
   A fast, machine-independent generator for 32-bit Microprocessors
   B. Schneier
   Dr. Dobb's Journal, v. 17, n. 2, February 1992, pp. 34-40.
   https://www.schneier.com/paper-pseudorandom-sequence.html
 */

unsigned int random (unsigned int seed) {
  seed = ((((seed >> 31)   /*Shift the 32nd bit to the first bit*/
	    ^ (seed >> 6)  /*XOR it with the seventh bit*/
	    ^ (seed >> 4)  /*XOR it with the fifth bit*/
	    ^ (seed >> 2)  /*XOR it with the third bit*/
	    ^ (seed >> 1)  /*XOR it with the second bit*/
	    ^ seed)        /*and XOR it with the first bit.*/
	   & 0x0000001)    /*Strip all the other bits off and*/
	  <<31)            /*move it back to the 32nd bit.*/
    | (seed >> 1);         /*Or with the seed shifted right.*/
  return seed;
}


/* Thrash memory with accesses to random addresses.
   n times:
     Generate a random address in the range 0x80000000 to 0xFFFFFFF0
     Write 4 words starting at that address with the address value
   Reset the random number seed
   n times:
     Generate a random address, using same pseudo-random sequence
     Read the words and compare with expected
     If different, exit and return the address
   If all ok, return 0
 */

unsigned int thrash (unsigned int seed, unsigned int n) {
  unsigned int i, j;
  unsigned int num;
  unsigned int* addr;
  num = seed;
  for (i = 0; i < n; i++) {
    num = random(num);
    addr = (unsigned int*)((num & 0x7FFFFFF0) | 0x80000000);
    for (j = 0; j < 16; j += 4) {
      *(addr + j) = (unsigned int)(addr + j);
    }
  }
  num = seed;
  for (i = 0; i < n; i++) {
    num = random(num);
    addr = (unsigned int*)((num & 0x7FFFFFF0) | 0x80000000);
    for (j = 0; j < 16; j += 4) {
      if (*(addr + j) != (unsigned int)(addr + j)) return 1;
    }
  }
  return 0;
}

int main (void) {
  int seed = 0x12345678;
  int num = 100000;
  
  return thrash(seed, num);

}
