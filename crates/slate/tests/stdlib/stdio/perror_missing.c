#include <stdio.h>

int main(void) {
  FILE *f = fopen("slate_stdio_missing_input.tmp", "r");
  if (!f) {
    perror("stdio-probe");
    return 0;
  }
  fclose(f);
  return 0;
}
