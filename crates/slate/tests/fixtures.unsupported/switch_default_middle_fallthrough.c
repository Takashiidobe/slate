#include <stdio.h>

int score(int x) {
  int out = 0;
  switch (x) {
  case 1:
    out += 1;
  default:
    out += 2;
  case 3:
    out += 3;
    break;
  case 4:
    out += 4;
  }
  return out;
}

int shared(int x) {
  int out = 0;
  switch (x) {
  case 2:
  default:
    out += 10;
  case 5:
    out += 20;
  }
  return out;
}

int main(void) {
  for (int i = 0; i < 7; i++)
    printf("%d,%d ", score(i), shared(i));
  printf("\n");
  return 0;
}
