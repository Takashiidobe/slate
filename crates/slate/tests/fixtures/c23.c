#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char output[16] = {};
  int written = strfroml(output, sizeof(output), "%.1f", 3.5L);
  printf("%d %s\n", written, output);
  return 0;
}
