/* { dg-options "-malign-double" } */
int printf(const char *, ...);

struct wide {
  char c;
  double d;
  int i;
  long long l;
};

int main(void) {
  struct wide w = {1, 2.5, 3, 4};
  printf("%zu %zu %zu %zu\n", sizeof(struct wide), __builtin_offsetof(struct wide, d),
         __builtin_offsetof(struct wide, l), _Alignof(struct wide));
  printf("%d %g %d %lld\n", w.c, w.d, w.i, w.l);
  return 0;
}
