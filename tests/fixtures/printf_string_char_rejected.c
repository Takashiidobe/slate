#include <stdio.h>

int main(void) {
  char buf[4] = "hey";
  // @rewrite-begin
  printf("%s\n", buf);
  // @rewrite-end
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe { printf(c"%s\n".as_ptr(), buf.as_mut_ptr() as *mut i8) };
// SLATE-FILECHECK-END rewrites
