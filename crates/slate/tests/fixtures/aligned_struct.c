#include <stdio.h>
#include <stddef.h>

struct __attribute__((aligned(16))) Aligned {
    char a;
    int b;
};

int main(void) {
    struct Aligned s;
    s.a = 5;
    s.b = 0x1234;

    printf("%zu %zu\n", sizeof(struct Aligned), _Alignof(struct Aligned));
    printf("%zu %zu\n", offsetof(struct Aligned, a), offsetof(struct Aligned, b));
    printf("%d %x\n", s.a, s.b);
    return 0;
}
