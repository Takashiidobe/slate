/* { dg-options "-fpack-struct=2" } */
int printf(const char *, ...);

struct mixed {
  char c;
  int i;
  short h;
  double d;
};

struct nested {
  char tag;
  struct mixed inner;
};

int main(void) {
  struct mixed m = {1, 2, 3, 4.5};
  struct nested n = {7, {8, 9, 10, 11.5}};
  printf("%zu %zu %zu %zu\n", sizeof(struct mixed), __builtin_offsetof(struct mixed, i),
         __builtin_offsetof(struct mixed, d), _Alignof(struct mixed));
  printf("%zu %zu\n", sizeof(struct nested), __builtin_offsetof(struct nested, inner));
  printf("%d %d %d %g %d %g\n", m.c, m.i, m.h, m.d, n.tag + n.inner.c, n.inner.d);
  return 0;
}
