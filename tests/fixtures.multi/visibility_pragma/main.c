#include <stdio.h>

int visible_before(void) { return 3; }
int visible_before_global = 5;

#pragma GCC visibility push(hidden)
int                    hidden_outer(void) { return 7; }
int                    hidden_outer_global = 11;

#pragma GCC visibility push(default)
int                    visible_inner(void) { return 13; }
int                    visible_inner_global = 17;
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
// LOWERING-DAG: pub extern "C" fn visible_before
// LOWERING-DAG: pub static mut visible_before_global
// LOWERING-DAG: pub extern "C" fn visible_inner
// LOWERING-DAG: pub static mut visible_inner_global
// LOWERING-DAG: pub extern "C" fn visible_after
// LOWERING-DAG: pub static mut visible_after_global
// LOWERING-DAG: {{^}}static mut hidden_outer_global
// LOWERING-DAG: {{^}}static mut hidden_again_global
// LOWERING-NOT: pub extern "C" fn hidden_outer
// LOWERING-NOT: pub extern "C" fn hidden_again
// LOWERING-NOT: pub static mut hidden_outer_global
// LOWERING-NOT: pub static mut hidden_again_global
// LOWERING-LABEL: {{^}}fn hidden_outer(
// LOWERING: {{^}}}
// LOWERING-LABEL: {{^}}fn hidden_again(
// LOWERING: {{^}}}
