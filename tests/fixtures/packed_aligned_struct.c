#include <stdio.h>
#include <stddef.h>

struct __attribute__((packed, aligned(4))) PackedAligned {
    char a;
    int b;
};

int main(void) {
    struct PackedAligned s;
    s.a = 7;
    s.b = 0x1234;

    printf("%zu %zu\n", sizeof(struct PackedAligned), _Alignof(struct PackedAligned));
    printf("%zu %zu\n", offsetof(struct PackedAligned, a), offsetof(struct PackedAligned, b));
    printf("%d %x\n", s.a, s.b);

    s.b = s.b + 1;
    printf("%x\n", s.b);
    return 0;
}
