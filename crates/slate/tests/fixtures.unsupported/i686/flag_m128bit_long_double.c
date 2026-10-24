/* { dg-options "--flavor=gcc -m128bit-long-double" } */
int printf(const char *, ...);

struct holder {
  char c;
  long double value;
};

int main(void) {
  struct holder h = {1, 2.5L};
  printf("%zu %zu %zu\n", sizeof(long double), sizeof(struct holder), __builtin_offsetof(struct holder, value));
  printf("%d %g\n", h.c, (double)h.value);
  return 0;
}
