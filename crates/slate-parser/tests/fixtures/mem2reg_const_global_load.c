#include <stdio.h>

struct Byte {
  signed char value;
};

struct Byte make_byte(void) {
  struct Byte result = {7};
  return result;
}

int initialize_chars(void) {
  char text[4] = "abc";
  return 5;
}

int main(void) {
  struct Byte byte = make_byte();
  printf("%d\n", byte.value + initialize_chars());
  return 0;
}



// SLATE-FILECHECK-ISYSTEM ~/Projects/slate/libc-shim/include /usr/lib/clang/22/include

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// SLATE-FILECHECK-END DEFAULT
