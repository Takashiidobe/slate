#include <stdio.h>

int main(void) {
  // @rewrite-begin
  printf("%5s|%-5s|%.1s|%6.1s\n", "hi", "hi", "hi", "hi");
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%5s|%-5s|%.1s|%6.1s\n".as_ptr(),
// REWRITES-X86_64-GNU-DAG:         c"hi".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:         c"hi".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:         c"hi".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-DAG:         c"hi".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-DAG:         c"hi".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:         c"hi".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:         c"hi".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-DAG:         c"hi".as_ptr() as *mut u8,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
