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
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: bool = unsafe { enabled };
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG: let {{__v[0-9]+}}: bool = unsafe { disabled };
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
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
