#include <stdio.h>

int main(void) {
  FILE *f = fopen("/dev/null", "w");
  if (!f) {
    return 1;
  }
  fputs("owned\n", f);
  fclose(f);
  return 0;
}
