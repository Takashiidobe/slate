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
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i8 = b"%c %s\n\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { bom_original };
// LOWERING-DAG: let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { bom_original };
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = c"%c %s\n".as_ptr() as *mut i8;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { bom_original };
// REWRITES-DAG: let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// REWRITES-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { bom_original };
// REWRITES-DAG: unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END rewrites
