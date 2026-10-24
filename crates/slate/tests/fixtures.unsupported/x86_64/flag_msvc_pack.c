/* { dg-options "--flavor=msvc /Zp2" } */
#include <stddef.h>
int printf(const char *, ...);

struct mixed {
  char c;
  int i;
  short h;
  double d;
};

int main(void) {
  struct mixed m = {1, 2, 3, 4.5};
  printf("%u %u %u\n", (unsigned)sizeof(struct mixed), (unsigned)offsetof(struct mixed, i),
         (unsigned)offsetof(struct mixed, d));
  printf("%d %d %d %g\n", m.c, m.i, m.h, m.d);
  return 0;
}
