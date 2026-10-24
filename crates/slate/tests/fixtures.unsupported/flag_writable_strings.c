/* { dg-options "-fwritable-strings" } */
int puts(const char *);

static char *greeting(void) { return "hello"; }

int main(void) {
  char *s = greeting();
  s[0] = 'j';
  puts(greeting());
  return 0;
}
