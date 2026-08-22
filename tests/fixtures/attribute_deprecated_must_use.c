#include <stdio.h>

__attribute__((deprecated)) static int old_api(int x) { return x + 1; }

__attribute__((deprecated("use new_api instead"))) static int old_api_msg(int x) {
  return x + 2;
}

__attribute__((warn_unused_result)) static int must_check(int x) { return x + 3; }

int main(void) {
  int a = old_api(1);
  int b = old_api_msg(2);
  int c = must_check(3);
  printf("%d %d %d\n", a, b, c);
  return 0;
}
