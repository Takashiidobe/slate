#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int aligned_to(void *p, uintptr_t bytes) {
  return ((uintptr_t)p & (bytes - 1)) == 0;
}

static int with_align(int n) {
  int ok = 1;
  char *small = __builtin_alloca(3);
  char *a = __builtin_alloca_with_align(n, 64);
  char *b = __builtin_alloca_with_align(n, 512);
  char *c = __builtin_alloca_with_align(n, 32768);
  ok &= aligned_to(a, 8) && aligned_to(b, 64) && aligned_to(c, 4096);
  memset(small, 1, 3);
  memset(a, 2, n);
  memset(b, 3, n);
  memset(c, 4, n);
  return ok * (small[2] + a[n - 1] + b[n - 1] + c[n - 1]);
}

static int with_align_uninitialized(int n) {
  int *p = __builtin_alloca_with_align_uninitialized(sizeof(int) * n, 1024);
  for (int i = 0; i < n; i++)
    p[i] = i * i;
  int sum = 0;
  for (int i = 0; i < n; i++)
    sum += p[i];
  return aligned_to(p, 128) * sum;
}

static long large_with_align(void) {
  long total = 0;
  for (int i = 0; i < 3; i++) {
    unsigned char *p = __builtin_alloca_with_align(1100000, 2048);
    memset(p, 5, 1100000);
    total += aligned_to(p, 256) * p[1099999];
  }
  return total;
}

int main(void) {
  printf("%d %d\n", with_align(5), with_align(33));
  printf("%d\n", with_align_uninitialized(10));
  printf("%ld\n", large_with_align());
  return 0;
}
