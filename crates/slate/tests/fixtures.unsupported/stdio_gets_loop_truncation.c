#include <stdio.h>

int main(void) {
  remove("slate_stdio_gets_loop_truncation.tmp");
  FILE *f = fopen("slate_stdio_gets_loop_truncation.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("0123456789abcdef\n", f);
  fputs("short\n", f);
  fclose(f);

  FILE *g = fopen("slate_stdio_gets_loop_truncation.tmp", "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char line[8];
  while (fgets(line, sizeof line, g) != NULL) {
    fputs(line, stdout);
  }
  fclose(g);
  remove("slate_stdio_gets_loop_truncation.tmp");
  return 0;
}
