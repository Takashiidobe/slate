#include <stdio.h>

static int sum_items(int *items, int len) {
  int total = 0;
  for (int i = 0; i < len; i++) {
    int item = items[i];
    total += item;
  }
  return total;
}

int main(void) {
  int values[4] = {2, 4, 6, 8};
  printf("%d\n", sum_items(values, 4));
  return 0;
}
