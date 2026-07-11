#include <stdio.h>
int main(void) {
  FILE *f = tmpfile();
  fputs("abcdef", f);
  fseek(f, 2, SEEK_SET);
  long pos = ftell(f);
  int c = fgetc(f);
  rewind(f);
  int d = fgetc(f);
  printf("%ld %c %c\n", pos, c, d);
  return 0;
}
