#include <stdio.h>
#include <stdlib.h>

// @check-fn-begin
static int *make(int n) {
  int *p = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++) {
    p[i] = i * i;
  }
  return p;
}
// @check-fn-end

// @check-fn-begin
int main(void) {
  int *q = make(4);
  printf("%d %d\n", q[0], q[3]);
  free(q);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn make(n: i32) -> *mut i32 {
// CHECK-NEXT:     let p: *mut i32 = (unsafe { malloc(((n as i64) as u64).wrapping_mul(4)) })
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { p.offset(i as isize) } = i * i;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let q: *mut i32 = make(4 as i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { * unsafe { q.offset(0 as isize) } }, unsafe { * unsafe { q
// CHECK-NEXT:         .offset(3 as isize) } }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { free(q as *mut ()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
