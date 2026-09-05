#include <stdio.h>

int main(void) {
  int arr[10];
  // @rewrite-begin
  // @lowering-begin
  arr[3] = 5;
  printf("%d\n", arr[3]);
  // @lowering-end
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-DAG: arr[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = arr[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: i64 = 3;
// COMMON-REWRITES-DAG: arr[({{__v[0-9]+}} as usize)] = 5;
// COMMON-REWRITES-DAG: unsafe { printf(c"%d\n".as_ptr(), arr[3]) };
// SLATE-FILECHECK-END common-rewrites
