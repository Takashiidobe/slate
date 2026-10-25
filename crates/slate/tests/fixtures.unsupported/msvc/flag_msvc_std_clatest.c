/* { dg-options "/std:clatest" } */
void abort(void);

static void check(long got, long want) {
  if (got != want)
    abort();
}

int main(void) {
  int value = 7;
  typeof(value) copy = value + 1;
  check(__STDC_VERSION__, 202312L);
  check(copy, 8);
  check(sizeof(typeof(char)), 1);
  return 0;
}
