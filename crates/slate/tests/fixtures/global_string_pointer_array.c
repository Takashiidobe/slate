#include <stdio.h>

static char *names[] = {"alpha", "beta"};

int main(void) {
  // @rewrite-begin
  printf("%s %s\n", names[0], names[1]);
  // @rewrite-end
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(
// REWRITES-DAG:         c"%s %s\n".as_ptr(),
// REWRITES-DAG:         unsafe { (*names)[((0 as i64) as usize)] },
// REWRITES-DAG:         unsafe { (*names)[((1 as i64) as usize)] },
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
