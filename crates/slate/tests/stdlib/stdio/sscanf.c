#include <stdio.h>
int main(void) {
  int i = 0;
  double d = 0.0;
  char c = 0;
  sscanf("12 3.5 z", "%d %lf %c", &i, &d, &c);
  printf("%d %.1f %c\n", i, d, c);
  return 0;
}
