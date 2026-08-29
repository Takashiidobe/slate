#include <stdio.h>

enum Err { ERR_NONE = 0, ERR_BAD = 5 };

static int first(void) {
  struct Rec {
    const char *text;
    enum Err code;
  };
  struct Rec items[] = {{"a", ERR_NONE}, {"bb", ERR_BAD}};
  int total = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].code + (int)items[i].text[0];
  return total;
}

static int second(void) {
  struct Rec {
    unsigned long n;
    const char *text;
    enum Err code;
  };
  struct Rec items[] = {{5, "x", ERR_NONE}, {6, "y", ERR_BAD}};
  int total = 0;
  for (unsigned i = 0; i < sizeof(items) / sizeof(struct Rec); i++)
    total += (int)items[i].n + (int)items[i].code + (int)items[i].text[0];
  return total;
}

int main(void) {
  printf("%d %d\n", first(), second());
  return 0;
}
