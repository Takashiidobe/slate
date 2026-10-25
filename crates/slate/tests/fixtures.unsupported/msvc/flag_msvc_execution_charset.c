/* { dg-options "/source-charset:utf-8 /execution-charset:windows-1252" } */
void abort(void);

static void check(int got, int want) {
  if (got != want)
    abort();
}

int main(void) {
  static const char text[] = "é€";
  check(sizeof(text), 3);
  check((unsigned char)text[0], 0xE9);
  check((unsigned char)text[1], 0x80);
  return 0;
}
