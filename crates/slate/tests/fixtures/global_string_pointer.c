#include <stdio.h>

char *bom_original = "Hi";

int main(void) {
  // @lowering-begin
  // @rewrite-begin
  printf("%c %s\n", *bom_original, bom_original);
  // @rewrite-end
  // @lowering-end
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"%c %s\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = unsafe { bom_original };
// LOWERING-DAG: let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = unsafe { bom_original };
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG: printf(
// REWRITES-DAG: c"%c %s\n".as_ptr(),
// REWRITES-DAG: (unsafe { *unsafe { bom_original } }) as i32,
// REWRITES-DAG: unsafe { bom_original },
// REWRITES-DAG: )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
