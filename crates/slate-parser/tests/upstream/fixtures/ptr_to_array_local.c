#include <stdio.h>

typedef int arr_t[5];

int main(void) {
  arr_t  src = {1, 2, 3, 4, 5};
  arr_t *p   = &src;
  int    sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += (*p)[i];
  }
  printf("%d\n", sum);
  return 0;
}


