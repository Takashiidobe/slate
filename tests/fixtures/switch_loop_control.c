#include <stdio.h>

int run(void) {
  int out = 0;
  for (int i = 0; i <= 3; i++) {
    switch (i) {
    case 0:
      out += 1;
      break;
    case 1:
      continue;
    case 2:
      out += 20;
      break;
    default:
      out += 100;
      break;
    }
    out += 3;
  }
  return out;
}

int main(void) {
  printf("%d\n", run());
  return 0;
}
