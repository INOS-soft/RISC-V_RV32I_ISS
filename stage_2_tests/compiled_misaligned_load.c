#include <stdint.h>

uint32_t main (void) {
    uint32_t a[2];
    uint32_t b, c;
    uint32_t *p;
    int i;
    a[0] = 0x03020100UL;
    a[1] = 0x13121110UL;
    for (i = 1; i <= 3; i++) {
	p = (uint32_t*)((uint8_t*)(&a[0]) + i);
	b = *p;
	c = (a[0] >> (i*8)) | (a[1] << (32 - i*8));
	if (b != c) return 0;
    }
    return 1;
}
