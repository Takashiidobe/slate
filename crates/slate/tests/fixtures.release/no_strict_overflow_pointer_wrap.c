/* { dg-options "-O2 -fno-strict-overflow" } */
#include <stdio.h>
#include <limits.h>

__attribute__((noinline)) static int end_wraps(char *buf, unsigned long size) {
  char *end = buf + size;
  return end < buf;
}

__attribute__((noinline)) static int start_wraps(char *buf, unsigned long back) {
  char *start = buf - back;
  return start > buf;
}

__attribute__((noinline)) static unsigned long clamped_size(char *buf, unsigned long size) {
  char *end = buf + size;
  if (end < buf) {
    end = (char *)-1;
    size = end - buf;
  }
  return size == ULONG_MAX - (unsigned long)buf;
}

int main(void) {
  static char buf[16];
  volatile unsigned long huge = ULONG_MAX - 1;
  volatile unsigned long small = 4;
  printf("%d %d\n", end_wraps(buf + 8, huge), end_wraps(buf, small));
  printf("%d %d\n", start_wraps(buf + 8, huge), start_wraps(buf + 8, small));
  printf("%lu %lu\n", clamped_size(buf + 8, huge), clamped_size(buf, small));
  return 0;
}
