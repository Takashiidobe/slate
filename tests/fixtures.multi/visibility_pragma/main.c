#include <stdio.h>

int visible_before(void) { return 3; }
int visible_before_global = 5;

#pragma GCC visibility push(hidden)
int hidden_outer(void) { return 7; }
int hidden_outer_global = 11;

#pragma GCC visibility push(default)
int visible_inner(void) { return 13; }
int visible_inner_global = 17;
#pragma GCC visibility pop

int hidden_again(void) { return 19; }
int hidden_again_global = 23;
#pragma GCC visibility pop

int visible_after(void) { return 29; }
int visible_after_global = 31;

int main(void) {
  printf("%d %d %d %d %d %d %d %d %d %d\n", visible_before(),
         visible_before_global, hidden_outer(), hidden_outer_global,
         visible_inner(), visible_inner_global, hidden_again(),
         hidden_again_global, visible_after(), visible_after_global);
  return 0;
}
