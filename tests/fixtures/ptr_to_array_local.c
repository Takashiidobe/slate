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

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG: p = std::ptr::addr_of_mut!(*src);
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG: p = std::ptr::addr_of_mut!(src);
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG: p = std::ptr::addr_of_mut!(*src);
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG: p = std::ptr::addr_of_mut!(src);
// SLATE-FILECHECK-END rewrites-aarch64-gnu
