#include <stdio.h>

int main(void) {
  int i = 0;
  int steps = 0;
  while (i < 6) {
    int x = i % 3;
    switch (x) {
    case 0:
    case 1:
    case 2:
      i++;
      steps++;
      continue;
    default:
      break;
    }
    printf("unreachable %d\n", i);
    i++;
    steps++;
  }
  printf("steps=%d\n", steps);
  return 0;
}
