/* { dg-options "-fstack-protector-strong" } */
#include <stdio.h>

__attribute__((noinline)) static void fill(char *p, int n) {
  for (int i = 0; i < n; i++)
    p[i] = 'A';
}

static volatile int extra = 8;

__attribute__((noinline)) static int victim(void) {
  char buf[16];
  char *volatile target = buf;
  fill(target, 16 + extra);
  return buf[0];
}

int main(void) {
  puts("start");
  fflush(stdout);
  return victim();
}
