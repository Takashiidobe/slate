#include <stdio.h>

int main(void) {
  // @rewrite-begin
  printf("%5s|%-5s|%.1s|%6.1s\n", "hi", "hi", "hi", "hi");
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     printf(
// COMMON-REWRITES-DAG:         c"%5s|%-5s|%.1s|%6.1s\n".as_ptr(),
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:         c"hi".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:         c"hi".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:         c"hi".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:         c"hi".as_ptr() as *mut i8,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:         c"hi".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:         c"hi".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:         c"hi".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:         c"hi".as_ptr() as *mut u8,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
