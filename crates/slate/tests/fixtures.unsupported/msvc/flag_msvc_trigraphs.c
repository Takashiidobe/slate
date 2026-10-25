/* { dg-options "/Zc:preprocessor /Zc:trigraphs" } */
void abort(void);

static void check(int got, int want) {
  if (got != want)
    abort();
}

int main(void) {
  static const char text[] = "??=??(??)";
  check(sizeof(text), 4);
  check(text[0], '#');
  check(text[1], '[');
  check(text[2], ']');
  return 0;
}
