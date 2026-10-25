/* { dg-options "/Zc:preprocessor /utf-8" } */
void abort(void);

static void check(int got, int want) {
  if (got != want)
    abort();
}

int main(void) {
  static const char narrow[] = "é";
  check(sizeof(narrow), 3);
  check((unsigned char)narrow[0], 0xC3);
  check(sizeof(L"é") / sizeof(L"é"[0]), 2);
  check(L"é"[0], 0xE9);
  return 0;
}
