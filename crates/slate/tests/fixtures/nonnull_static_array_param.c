#include <stdio.h>

int sum4(int arr[static 4]) {
  int s = 0;
  for (int i = 0; i < 4; i++) {
    s += arr[i];
  }
  return s;
}

int main(void) {
  int values[4] = {1, 2, 3, 4};
  printf("%d\n", sum4(values));
  return 0;
}
