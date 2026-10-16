#include <stdio.h>

typedef int arr_t[5];

// @check-fn-begin
int main(void) {
  arr_t  src = {1, 2, 3, 4, 5};
  arr_t *p   = &src;
  int    sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += (*p)[i];
  }
  printf("%d\n", sum);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut src: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut p: *mut [i32; 5] = std::ptr::addr_of_mut!(src.0);
// CHECK-NEXT:     let mut sum: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (5 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = sum;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]]
// CHECK-NEXT:             + unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { (*p).as_mut_ptr() as *mut i32 }.offset(i as isize) }
// CHECK-NEXT:             };
// CHECK-NEXT:         sum = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", sum);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
