/* { dg-options "/Zc:preprocessor /DSLATE_A=3 /DSLATE_B#4 /DSLATE_C /DSLATE_D /USLATE_D" } */
void abort(void);

static void check(int got, int want) {
  if (got != want)
    abort();
}

int main(void) {
  check(SLATE_A, 3);
  check(SLATE_B, 4);
  check(SLATE_C, 1);
#ifdef SLATE_D
  abort();
#endif
  return 0;
}
