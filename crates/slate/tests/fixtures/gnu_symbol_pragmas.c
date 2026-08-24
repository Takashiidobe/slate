// LOWERING-DAG: .weak pragma_weak_alias\n.set pragma_weak_alias, pragma_weak_target
// LOWERING-DAG: fn pragma_weak_alias(_0: i32) -> i32;
// LOWERING-DAG: fn pragma_actual(arg0: i32) -> i32 {
// LOWERING-NOT: pragma_renamed

#include <stdio.h>

int pragma_weak_target(int value) { return value + 7; }

#pragma weak pragma_weak_alias = pragma_weak_target
extern int   pragma_weak_alias(int);

#pragma redefine_extname pragma_renamed pragma_actual
int pragma_renamed(int value) { return value * 3; }

int main(void) {
  printf("%d %d %d\n", pragma_weak_alias(29),
         pragma_weak_alias == pragma_weak_target, pragma_renamed(13));
  return 0;
}
