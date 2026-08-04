#include <stdio.h>

int main(void) {
  FILE *f = fopen("slate_fprintf_non_standard.tmp", "w");
  if (f == NULL) {
    return 1;
  }
  fprintf(f, "value: %d\n", 7);
  fclose(f);
  remove("slate_fprintf_non_standard.tmp");
  return 0;
}
