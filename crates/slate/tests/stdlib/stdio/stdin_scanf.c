#include <stdio.h>

int main(void) {
  int i = 0;
  char c = 0;
  int n = scanf("%d %c", &i, &c);
  printf("%d %d %c\n", n, i, c);
  return 0;
}
