/* SLATE: needs cl's traditional preprocessor (/Zc:preprocessor-, cl's
 flagless default). Slate always preprocesses conformingly, as cl does with
 /Zc:preprocessor or /std:c11 and later; most libraries do not build with the
 traditional one, so /Zc:preprocessor- is rejected and will not be supported
 (slate-3amj.5.12). */
/* { dg-options "/Zc:preprocessor /Zc:preprocessor-" } */
void abort(void);

static void check(int got, int want) {
  if (got != want)
    abort();
}

#define PICK_THIRD(a, b, c, ...) c
#define COUNT(...) PICK_THIRD(__VA_ARGS__, 2, 1)
#define SUM(first, ...) (first + sum_rest(0, ##__VA_ARGS__))
#define ELIDE(fmt, ...) elide(fmt, __VA_ARGS__)

static int sum_rest(int zero, ...) { return zero; }
static int elide(int fmt) { return fmt; }

int main(void) {
  check(COUNT(x, y), 1);
  check(SUM(5), 5);
  check(ELIDE(9), 9);
  return 0;
}
