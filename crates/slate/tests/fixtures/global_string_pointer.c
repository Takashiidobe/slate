#include <stdio.h>

char *bom_original = "Hi";

int main(void) {
  printf("%c %s\n", *bom_original, bom_original);
  return 0;
}
