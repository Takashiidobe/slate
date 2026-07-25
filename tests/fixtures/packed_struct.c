#include <stddef.h>
#include <stdio.h>

struct __attribute__((packed)) Packed {
  char a;
  int b;
  char c;
};

int main(void) {
  struct Packed p;
  p.a = 1;
  p.b = 0x11223344;
  p.c = 2;

  printf("%zu %zu\n", sizeof(struct Packed), _Alignof(struct Packed));
  printf("%zu %zu %zu\n", offsetof(struct Packed, a),
         offsetof(struct Packed, b), offsetof(struct Packed, c));
  printf("%d %x %d\n", p.a, p.b, p.c);
  return 0;
}
