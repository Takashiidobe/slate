#include <stdio.h>

int main(void) {
  char name[] = "slate_stdio_file_ownership.tmp";
  remove(name);
  FILE *f = fopen(name, "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  fputs("owned\n", f);
  fclose(f);

  FILE *g = fopen(name, "r");
  if (!g) {
    puts("reopen-fail");
    return 0;
  }
  char buf[16] = {0};
  fgets(buf, sizeof buf, g);
  fclose(g);
  fputs(buf, stdout);
  remove(name);
  return 0;
}
