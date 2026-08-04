#include <stdio.h>

int pragma_weak_target(int value) { return value + 7; }

#pragma weak pragma_weak_alias = pragma_weak_target
extern int pragma_weak_alias(int);

#pragma redefine_extname pragma_renamed pragma_actual
int pragma_renamed(int value) { return value * 3; }

int main(void) {
  printf("%d %d %d\n", pragma_weak_alias(29),
         pragma_weak_alias == pragma_weak_target, pragma_renamed(13));
  return 0;
}
