/* { dg-options "/Zc:preprocessor /std:c11" } */
void abort(void);

static void check(long got, long want) {
  if (got != want)
    abort();
}

struct aligned {
  _Alignas(16) char c;
};

_Static_assert(sizeof(int) == 4, "int");

int main(void) {
  check(__STDC_VERSION__, 201112L);
  check(_Alignof(struct aligned), 16);
  check(_Generic((char)0, char: 1, default: 0), 1);
  return 0;
}
