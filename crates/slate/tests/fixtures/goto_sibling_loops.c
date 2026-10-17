#include <stdio.h>

// @check-fn-begin
int main() {
  int i    = 0;
  int k    = 0;
  int sum  = 0;
  int prod = 1;
first:
  sum = sum + i;
  i   = i + 1;
  if (i < 5)
    goto first;
  k = 1;
second:
  prod = prod * k;
  k    = k + 1;
  if (k < 5)
    goto second;
  printf("%d %d\n", sum, prod);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut prod: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut sum: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0 as i32;
// CHECK-NEXT:         k = 0 as i32;
// CHECK-NEXT:         sum = 0 as i32;
// CHECK-NEXT:         prod = 1 as i32;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             sum += i;
// CHECK-NEXT:             i += 1 as i32;
// CHECK-NEXT:             if !(i < (5 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         k = 1 as i32;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             prod *= k;
// CHECK-NEXT:             k += 1 as i32;
// CHECK-NEXT:             if !(k < (5 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{} {}", sum, prod);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
