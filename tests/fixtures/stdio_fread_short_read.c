#include <stdio.h>

int main(void) {
  remove("slate_stdio_fread_short_read.tmp");
  FILE *f = fopen("slate_stdio_fread_short_read.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("abcdefghi", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_fread_short_read.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char buf[16] = {0};
  size_t n = fread(buf, 4, 4, g);
  printf("%zu %s\n", n, buf);
  fclose(g);
  remove("slate_stdio_fread_short_read.tmp");
  return 0;
}
