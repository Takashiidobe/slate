/* { dg-options "/Zp2" } */
#include <stddef.h>
void abort(void);

struct mixed {
  char c;
  int i;
  short h;
  double d;
};

static void check(int got, int want) {
  if (got != want)
    abort();
}

int main(void) {
  struct mixed m = {1, 2, 3, 4.5};
  check(sizeof(struct mixed), 16);
  check(offsetof(struct mixed, i), 2);
  check(offsetof(struct mixed, d), 8);
  check(m.c, 1);
  check(m.i, 2);
  check(m.h, 3);
  check(m.d == 4.5, 1);
  return 0;
}
