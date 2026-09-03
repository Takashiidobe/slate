#include <stdio.h>

static char *names[] = {"alpha", "beta"};

int main(void) {
  // @rewrite-begin
  printf("%s %s\n", names[0], names[1]);
  // @rewrite-end
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: unsafe {
// REWRITES-DAG:     printf(c"%s %s\n".as_ptr(), unsafe { (*names)[0] }, unsafe {
// REWRITES-DAG:         (*names)[1]
// REWRITES-DAG:     })
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
