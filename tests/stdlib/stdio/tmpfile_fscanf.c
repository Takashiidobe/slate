#include <stdio.h>

int main(void) {
  FILE *f = tmpfile();
  fputs("12 xy 4.5", f);
  rewind(f);
  int i = 0;
  char word[3] = {0};
  double d = 0.0;
  int n = fscanf(f, "%d %2s %lf", &i, word, &d);
  printf("%d %d %s %.1f\n", n, i, word, d);
  fclose(f);
  return 0;
}
