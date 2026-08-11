#include <stdio.h>

extern int failures;
void run_case(void (*fn)(int), int ok);
void check(int ok);
void other_check(int ok);

int main(void) {
  void (*other)(int) = other_check;
  other(1);
  void (*fn)(int) = check;
  run_case(fn, 0);
  printf("failures: %d\n", failures);
  return 0;
}
