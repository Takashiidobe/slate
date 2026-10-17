#include <stdio.h>

// @check-fn-begin
int main() {
  int i     = 0;
  int j     = 0;
  int total = 0;
outer:
  j = 0;
inner:
  total = total + (i * j);
  j     = j + 1;
  if (j < 3)
    goto inner;
  i = i + 1;
  if (i < 4)
    goto outer;
  printf("%d\n", total);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut total: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut j: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         j = 0;
// CHECK-NEXT:         total = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             j = 0;
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 total += i * j;
// CHECK-NEXT:                 j += 1;
// CHECK-NEXT:                 if !(j < 3) {
// CHECK-NEXT:                     break;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:             if !(i < 4) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", total);
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
