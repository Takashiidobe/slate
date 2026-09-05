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
// REWRITES-DAG:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d %d %d %d %d %d %d %d\n".as_ptr() as *mut i8;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = visible_before();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { visible_before_global };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = hidden_outer();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { hidden_outer_global };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = visible_inner();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { visible_inner_global };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = hidden_again();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { hidden_again_global };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = visible_after();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { visible_after_global };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
