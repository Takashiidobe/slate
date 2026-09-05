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

// @rewrite-fn-begin
int main(void) {
  printf("%d %d %d %d %d %d %d %d %d %d\n", visible_before(),
         visible_before_global, hidden_outer(), hidden_outer_global,
         visible_inner(), visible_inner_global, hidden_again(),
         hidden_again_global, visible_after(), visible_after_global);
  return 0;
}
// @rewrite-fn-end

// LOWERING-DAG: pub extern "C-unwind" fn visible_before
// LOWERING-DAG: pub static mut visible_before_global
// LOWERING-DAG: pub extern "C-unwind" fn visible_inner
// LOWERING-DAG: pub static mut visible_inner_global
// LOWERING-DAG: pub extern "C-unwind" fn visible_after
// LOWERING-DAG: pub static mut visible_after_global
// LOWERING-DAG: {{^}}static mut hidden_outer_global
// LOWERING-DAG: {{^}}static mut hidden_again_global
// LOWERING-NOT: pub extern "C-unwind" fn hidden_outer
// LOWERING-NOT: pub extern "C-unwind" fn hidden_again
// LOWERING-NOT: pub static mut hidden_outer_global
// LOWERING-NOT: pub static mut hidden_again_global
// LOWERING-LABEL: {{^}}fn hidden_outer(
// LOWERING: {{^}}}
// LOWERING-LABEL: {{^}}fn hidden_again(
// LOWERING: {{^}}}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             c"%d %d %d %d %d %d %d %d %d %d\n".as_ptr(),
// REWRITES-DAG:             visible_before(),
// REWRITES-DAG:             unsafe { visible_before_global },
// REWRITES-DAG:             hidden_outer(),
// REWRITES-DAG:             unsafe { hidden_outer_global },
// REWRITES-DAG:             visible_inner(),
// REWRITES-DAG:             unsafe { visible_inner_global },
// REWRITES-DAG:             hidden_again(),
// REWRITES-DAG:             unsafe { hidden_again_global },
// REWRITES-DAG:             visible_after(),
// REWRITES-DAG:             unsafe { visible_after_global },
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: fn main() {
// REWRITES-X86_64-GNU-DAG:     unsafe {
// REWRITES-X86_64-GNU-DAG:         printf(
// REWRITES-X86_64-GNU-DAG:             c"%d %d %d %d %d %d %d %d %d %d\n".as_ptr(),
// REWRITES-X86_64-GNU-DAG:             visible_before(),
// REWRITES-X86_64-GNU-DAG:             unsafe { visible_before_global },
// REWRITES-X86_64-GNU-DAG:             hidden_outer(),
// REWRITES-X86_64-GNU-DAG:             unsafe { hidden_outer_global },
// REWRITES-X86_64-GNU-DAG:             visible_inner(),
// REWRITES-X86_64-GNU-DAG:             unsafe { visible_inner_global },
// REWRITES-X86_64-GNU-DAG:             hidden_again(),
// REWRITES-X86_64-GNU-DAG:             unsafe { hidden_again_global },
// REWRITES-X86_64-GNU-DAG:             visible_after(),
// REWRITES-X86_64-GNU-DAG:             unsafe { visible_after_global },
// REWRITES-X86_64-GNU-DAG:         )
// REWRITES-X86_64-GNU-DAG:     };
// REWRITES-X86_64-GNU-DAG:     std::process::exit(0 as i32);
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: fn main() {
// REWRITES-AARCH64-GNU-DAG:     unsafe {
// REWRITES-AARCH64-GNU-DAG:         printf(
// REWRITES-AARCH64-GNU-DAG:             c"%d %d %d %d %d %d %d %d %d %d\n".as_ptr(),
// REWRITES-AARCH64-GNU-DAG:             visible_before(),
// REWRITES-AARCH64-GNU-DAG:             unsafe { visible_before_global },
// REWRITES-AARCH64-GNU-DAG:             hidden_outer(),
// REWRITES-AARCH64-GNU-DAG:             unsafe { hidden_outer_global },
// REWRITES-AARCH64-GNU-DAG:             visible_inner(),
// REWRITES-AARCH64-GNU-DAG:             unsafe { visible_inner_global },
// REWRITES-AARCH64-GNU-DAG:             hidden_again(),
// REWRITES-AARCH64-GNU-DAG:             unsafe { hidden_again_global },
// REWRITES-AARCH64-GNU-DAG:             visible_after(),
// REWRITES-AARCH64-GNU-DAG:             unsafe { visible_after_global },
// REWRITES-AARCH64-GNU-DAG:         )
// REWRITES-AARCH64-GNU-DAG:     };
// REWRITES-AARCH64-GNU-DAG:     std::process::exit(0 as i32);
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites-aarch64-gnu
