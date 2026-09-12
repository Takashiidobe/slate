#include <stddef.h>
#include <stdio.h>

struct __attribute__((packed, aligned(4))) PackedAligned {
  char a;
  int  b;
};

int main(void) {
  struct PackedAligned s;
  s.a = 7;
  s.b = 0x1234;

  printf("%zu %zu\n", sizeof(struct PackedAligned),
         _Alignof(struct PackedAligned));
  printf("%zu %zu\n", offsetof(struct PackedAligned, a),
         offsetof(struct PackedAligned, b));
  printf("%d %x\n", s.a, s.b);

  s.b = s.b + 1;
  printf("%x\n", s.b);
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
