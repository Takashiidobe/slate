#include <stdio.h>

static void fill(int *p, int n, int val) {
  for (int i = 0; i < n; i++) {
    *(p + i) = val;
  }
}

int main(void) {
  int buf[4] = {0};
  fill(buf, 4, 7);
  printf("%d %d %d %d\n", buf[0], buf[1], buf[2], buf[3]);
  return buf[0] + buf[1] + buf[2] + buf[3];
}
