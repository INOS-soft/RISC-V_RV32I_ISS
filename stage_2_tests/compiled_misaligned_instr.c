#include <stdint.h>

int fn0(void) {
    return -1;
}

int fn1(void) {
    return 1;
}

int main (void) {
    int i;
    int (*p)(void);
    p = (int (*)(void))((char*)(&fn1) + 2);
    i = (*p)();
    return i;
}
