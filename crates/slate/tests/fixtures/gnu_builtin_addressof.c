#include <stdio.h>

static int address_probe(void) {
  int value = 37;
  int *address = __builtin_addressof(value);
  return (address == &value) + (*address == 37);
}

int main(void) {
  printf("%d\n", address_probe());
  return 0;
}
