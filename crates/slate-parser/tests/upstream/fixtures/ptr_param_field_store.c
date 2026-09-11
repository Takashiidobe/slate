#include <stdio.h>

typedef struct {
  int a;
  int b;
} pair_t;

static void fill(pair_t *out, int x, int y) {
  out->a = x;
  out->b = y;
}

int main(void) {
  pair_t p;
  fill(&p, 3, 4);
  printf("%d %d\n", p.a, p.b);
  return 0;
}


