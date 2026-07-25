#include <stdio.h>
int main(void) {
  FILE *f = tmpfile();
  fputc('Z', f);
  rewind(f);
  int c = fgetc(f);
  putchar(c);
  putchar('\n');
  fclose(f);
  return 0;
}
