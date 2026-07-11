#include <stdio.h>

int pick(int x) {
  int out = 5;
  switch (x) {
  case 4:
    out += 4;
    break;
  case 9:
    out += 9;
    break;
  }
  return out;
}

int main(void) {
  printf("%d %d %d\n", pick(4), pick(9), pick(2));
  return 0;
}
