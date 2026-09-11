#include <stdio.h>
#include <stdlib.h>

static int sum_fixed(int *p) {
  int total = 0;
  for (int i = 0; i < 4; i++) {
    total += p[i];
  }
  return total;
}

static void scale_fixed(int *p) {
  for (int i = 0; i < 3; i++) {
    p[i] = p[i] * 2;
  }
}

static int sum3(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  return t;
}

static int mix(int *p) {
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t += p[i];
  }
  t += p[5];
  return t;
}

int main(void) {
  int arr[4] = {10, 20, 30, 40};
  int r      = sum_fixed(arr);

  int a[3] = {1, 2, 3};
  scale_fixed(a);

  int *m = malloc(3 * sizeof(int));
  m[0]   = 5;
  m[1]   = 6;
  m[2]   = 7;
  int r2 = sum3(m);
  free(m);

  int big[6] = {1, 2, 3, 4, 5, 6};
  int r3     = mix(big);

  printf("%d %d %d %d %d\n", r, a[0], a[2], r2, r3);
  return 0;
}


