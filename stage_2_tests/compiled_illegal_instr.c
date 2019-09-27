#include <stdint.h>

int fn0(void) {
    return -1;
}

int main (void) {
    uint32_t fn_ok[2];
    uint32_t fn_illegal;
    fn_ok[0] = 0x00100513; // li a0, 1
    fn_ok[1] = 0x00008067; // ret
    fn_illegal = 0x02c58533; // mul x10, x11, x12
    int i;
    int (*p)(void);
    p = (int (*)(void))(&fn_ok);
    if ((*p)() != 1) return 0;
    p = (int (*)(void))(&fn_illegal);
    if ((*p)() != -1) return 0;
    return 1;
}
