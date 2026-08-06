#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_int a        = 1;
  int        expected = 0;
  int        attempts = 0;

  while (!atomic_compare_exchange_strong(&a, &expected, 3)) {
    attempts++;
  }

  printf("%d %d %d\n", attempts, expected, (int)a);
  return 0;
}
