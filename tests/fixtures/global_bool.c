#include <stdbool.h>
#include <stdio.h>

bool        enabled = true;
static bool disabled;

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  printf("%d %d\n", enabled, disabled);
  // @rewrite-end
  // @lowering-end
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: bool = unsafe { enabled };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: bool = unsafe { disabled };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%d %d\n".as_ptr(),
// REWRITES-DAG:         (unsafe { enabled }) as i32,
// REWRITES-DAG:         (unsafe { disabled }) as i32,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
