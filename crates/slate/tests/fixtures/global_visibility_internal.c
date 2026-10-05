#include <stdio.h>

__attribute__((visibility("internal"))) extern const unsigned char opmodes[4];
__attribute__((visibility("hidden"))) extern const char *const typenames[3];
__attribute__((visibility("protected"))) int counter;

const unsigned char opmodes[4] = {1, 2, 4, 8};
const char *const typenames[3] = {"nil", "boolean", "number"};

int main(void) {
  for (int i = 0; i < 4; i++) {
    counter += opmodes[i];
  }
  printf("%d %s %s\n", counter, typenames[0], typenames[2]);
  return counter == 15 ? 0 : 1;
}
