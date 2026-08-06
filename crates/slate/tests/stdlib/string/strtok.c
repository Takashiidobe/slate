#include <stdio.h>
#include <string.h>
int main(void) {
  char  b[] = "alpha,beta,,gamma";
  char *a   = strtok(b, ",");
  char *c   = strtok(0, ",");
  char *d   = strtok(0, ",");
  char *e   = strtok(0, ",");
  printf("%s %s %s %d\n", a, c, d, e == 0);
  return 0;
}
