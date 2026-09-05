#include <stdio.h>

int main(void) {
  char buf[4] = "hey";
  // @rewrite-begin
  printf("%s\n", buf);
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: unsafe { printf(c"%s\n".as_ptr(), buf.as_mut_ptr() as *mut i8) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: unsafe { printf(c"%s\n".as_ptr(), buf.as_mut_ptr() as *mut u8) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
