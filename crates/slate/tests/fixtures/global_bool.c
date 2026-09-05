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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: bool = unsafe { enabled };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: bool = unsafe { disabled };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         c"%d %d\n".as_ptr(),
// COMMON-REWRITES-DAG:         (unsafe { enabled }) as i32,
// COMMON-REWRITES-DAG:         (unsafe { disabled }) as i32,
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites
