#include <stdio.h>

static char *names[] = {"alpha", "beta"};

int main(void) {
  // @rewrite-begin
  printf("%s %s\n", names[0], names[1]);
  // @rewrite-end
}

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: unsafe {
// COMMON-REWRITES-DAG:     })
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     printf(c"%s %s\n".as_ptr(), unsafe { (*names)[0] }, unsafe {
// REWRITES-X86_64-GNU-DAG:         (*names)[1]
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     printf(c"%s %s\n".as_ptr(), unsafe { names[0] }, unsafe {
// REWRITES-AARCH64-GNU-DAG:         names[1]
// SLATE-FILECHECK-END rewrites-aarch64-gnu
