/* { dg-options "-fpascal-strings" } */
int printf(const char *, ...);

int main(void) {
  const unsigned char *greeting = (const unsigned char *)"\pHello";
  const unsigned char *empty = (const unsigned char *)"\p";
  printf("%d %s %d %zu\n", greeting[0], (const char *)greeting + 1, empty[0], sizeof("\pHello"));
  return 0;
}
