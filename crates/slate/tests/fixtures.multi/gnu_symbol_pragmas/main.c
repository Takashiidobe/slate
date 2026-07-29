#include <stdio.h>

extern int pragma_weak_target(int);
extern int pragma_weak_alias(int);

#pragma redefine_extname pragma_renamed pragma_actual
extern int pragma_renamed(int);

int main(void) {
  printf("%d %d\n", pragma_weak_alias(29), pragma_renamed(13));
  return 0;
}
