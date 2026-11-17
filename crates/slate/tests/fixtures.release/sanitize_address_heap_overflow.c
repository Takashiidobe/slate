/* { dg-options "-fsanitize=address" } */
#include <stdio.h>
#include <stdlib.h>

static volatile int extra = 1;
static char *volatile buffer;

__attribute__((noinline)) static int poke(char *p, int i) {
  p[i] = 'A';
  return p[i];
}

int main(void) {
  buffer = malloc(16);
  puts("start");
  fflush(stdout);
  int value = poke(buffer, 15 + extra);
  free(buffer);
  return value;
}
