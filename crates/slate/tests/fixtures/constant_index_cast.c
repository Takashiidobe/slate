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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = 5;
// LOWERING-DAG: let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG: arr[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG: let {{__v[0-9]+}}: i64 = 3;
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = arr[({{__v[0-9]+}} as usize)];
// LOWERING-DAG: let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{__v[0-9]+}}: i64 = 3;
// REWRITES-DAG: arr[({{__v[0-9]+}} as usize)] = 5;
// REWRITES-DAG: unsafe { printf(c"%d\n".as_ptr(), arr[3]) };
// SLATE-FILECHECK-END rewrites
