/* { dg-options "--flavor=gcc -fzero-init-padding-bits=all" } */
#include <stdio.h>
#include <string.h>

union small_first {
  char c;
  long long wide;
};

struct padded {
  char c;
  int i;
};

static void scribble(void) {
  volatile unsigned char junk[256];
  memset((void *)junk, 0xa5, sizeof junk);
}

static void inspect(void) {
  union small_first u = {.c = 1};
  struct padded p = {2, 3};
  unsigned char bytes[sizeof u];
  unsigned char pbytes[sizeof p];
  memcpy(bytes, &u, sizeof u);
  memcpy(pbytes, &p, sizeof p);
  for (unsigned i = 0; i < sizeof u; i++)
    printf("%02x", bytes[i]);
  printf(" ");
  for (unsigned i = 0; i < sizeof p; i++)
    printf("%02x", pbytes[i]);
  printf("\n");
}

int main(void) {
  scribble();
  inspect();
  return 0;
}
