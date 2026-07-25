#include <stdio.h>

int main() {
  int x = 1;
  goto done;
  x = 99;
done:
  printf("%d\n", x);
  return 0;
}
