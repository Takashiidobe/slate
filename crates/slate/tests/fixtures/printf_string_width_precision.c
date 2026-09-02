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
// REWRITES-DAG:         b"hi\0".as_ptr() as *mut i8,
// REWRITES-DAG:         b"hi\0".as_ptr() as *mut i8,
// REWRITES-DAG:         b"hi\0".as_ptr() as *mut i8,
// REWRITES-DAG:         b"hi\0".as_ptr() as *mut i8,
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
