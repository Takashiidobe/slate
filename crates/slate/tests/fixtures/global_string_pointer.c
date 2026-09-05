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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%c %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { bom_original };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: i8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = unsafe { bom_original };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%c %s\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = unsafe { bom_original };
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = unsafe { bom_original };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         c"%c %s\n".as_ptr(),
// COMMON-REWRITES-DAG:         (unsafe { *unsafe { bom_original } }) as i32,
// COMMON-REWRITES-DAG:         unsafe { bom_original },
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites
