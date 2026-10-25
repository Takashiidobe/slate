/* { dg-options "-fsso-struct=big-endian" } */
#include <stdio.h>
#include <string.h>

struct words {
  unsigned int word;
  unsigned short half;
};

int main(void) {
  struct words w = {0x01020304u, 0x0506};
  unsigned char bytes[sizeof w];
  memcpy(bytes, &w, sizeof w);
  for (unsigned i = 0; i < 6; i++)
    printf("%02x", bytes[i]);
  printf(" %x %x\n", w.word, w.half);
  return 0;
}
