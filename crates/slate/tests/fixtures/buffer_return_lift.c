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
// CHECK-LABEL: {{^}}fn make(mut n: i32) -> *mut i32 {
// CHECK-NEXT:     let mut p: *mut i32 = (unsafe { malloc(((n as i64) as u64).wrapping_mul(4 as u64)) })
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { p.offset(i as isize) } = i * i;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut q: *mut i32 = make(4 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d\n".as_ptr() as *const i8,
// CHECK-NEXT:             unsafe { *unsafe { q.offset((0 as i32) as isize) } },
// CHECK-NEXT:             unsafe { *unsafe { q.offset((3 as i32) as isize) } },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { free(q as *mut ()) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
