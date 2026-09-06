int printf(const char *, ...);

#ifdef OUTER_FEATURE
#ifdef INNER_FEATURE
static int nested_code(void) { return 11; }
#else
static int nested_code(void) { return 12; }
#endif
#else
static int nested_code(void) { return 20; }
#endif

int main(void) {
  printf("%d\n", nested_code());
  return 0;
}
