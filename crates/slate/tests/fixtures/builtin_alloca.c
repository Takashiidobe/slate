#include <alloca.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct frame {
  int *args;
  int count;
};

static int const_alloca(void) {
  unsigned char *buf = __builtin_alloca(16);
  memset(buf, 3, 16);
  return buf[0] + buf[15];
}

static int dyn_alloca(int n) {
  unsigned char *buf = __builtin_alloca(n);
  memset(buf, 7, n);
  return buf[0] + buf[n - 1];
}

static int uninitialized_alloca(int n) {
  unsigned char *buf = __builtin_alloca_uninitialized(n);
  memset(buf, 9, n);
  return buf[0] + buf[n - 1];
}

static int loop_alloca(int n) {
  int *slots[8];
  for (int i = 0; i < n; i++) {
    slots[i] = alloca(sizeof(int) * (i + 1));
    for (int j = 0; j <= i; j++)
      slots[i][j] = i * 10 + j;
  }
  int sum = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j <= i; j++)
      sum += slots[i][j];
  return sum;
}

static int sum_frame(const struct frame *f) {
  int sum = 0;
  for (int i = 0; i < f->count; i++)
    sum += f->args[i];
  return sum;
}

static int branch_alloca(int argc, const int *argv, int min) {
  struct frame f = {(int *)argv, argc};
  if (argc < min) {
    f.args = alloca(sizeof(int) * min);
    for (int i = 0; i < argc; i++)
      f.args[i] = argv[i];
    for (int i = argc; i < min; i++)
      f.args[i] = -1;
    f.count = min;
  }
  return sum_frame(&f);
}

static int ternary_alloca(char *scratch, int n) {
  char *buf = scratch ? scratch : alloca(n);
  for (int i = 0; i < n; i++)
    buf[i] = (char)i;
  return buf[n - 1];
}

static int zero_alloca(void) {
  char *a = alloca(0);
  char *b = alloca(4);
  memcpy(b, "abc", 4);
  return a != 0 && strcmp(b, "abc") == 0;
}

static int aligned_alloca(void) {
  int ok = 1;
  for (int n = 1; n < 40; n += 3) {
    char *p = __builtin_alloca(n);
    ok &= ((uintptr_t)p & 15) == 0;
  }
  return ok;
}

static int recursive_alloca(int depth) {
  int *cell = alloca(sizeof(int));
  *cell = depth;
  if (depth == 0)
    return 0;
  int below = recursive_alloca(depth - 1);
  return *cell + below;
}

static long large_alloca(int rounds) {
  long total = 0;
  for (int r = 0; r < rounds; r++) {
    unsigned char *a = alloca(600000);
    unsigned char *b = alloca(600000);
    memset(a, 1, 600000);
    memset(b, 2, 600000);
    total += a[599999] + b[0];
  }
  return total;
}

static long repeated_large(void) {
  long total = 0;
  for (int i = 0; i < 4; i++)
    total += large_alloca(1);
  return total;
}

int main(void) {
  int argv[] = {4, 5};
  char scratch[8];
  printf("%d %d %d\n", const_alloca(), dyn_alloca(10),
         uninitialized_alloca(12));
  printf("%d\n", loop_alloca(8));
  printf("%d %d\n", branch_alloca(2, argv, 5), branch_alloca(2, argv, 1));
  printf("%d %d\n", ternary_alloca(scratch, 8), ternary_alloca(0, 20));
  printf("%d %d\n", zero_alloca(), aligned_alloca());
  printf("%d\n", recursive_alloca(100));
  printf("%ld\n", repeated_large());
  return 0;
}
