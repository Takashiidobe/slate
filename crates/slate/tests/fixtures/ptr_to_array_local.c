#include <stdio.h>

typedef int arr_t[5];

int main(void) {
  arr_t  src = {1, 2, 3, 4, 5};
  // @lowering-begin
  // @rewrite-begin
  arr_t *p   = &src;
  // @rewrite-end
  // @lowering-end
  int    sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += (*p)[i];
  }
  printf("%d\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: p = std::ptr::addr_of_mut!(*src);
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: p = std::ptr::addr_of_mut!(*src);
// SLATE-FILECHECK-END rewrites
