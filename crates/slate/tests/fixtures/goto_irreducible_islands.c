#include <stdio.h>

// @check-fn-begin
int main() {
  volatile int pick_a = 1;
  volatile int pick_b = 0;
  int          x      = 0;
  int          y      = 0;

  if (pick_a)
    goto a2;
a1:
  x = x + 1;
  if (x < 3)
    goto a2;
  goto second;
a2:
  x = x + 2;
  if (x < 4)
    goto a1;

second:
  if (pick_b)
    goto b2;
b1:
  y = y + 3;
  if (y < 9)
    goto b2;
  goto done;
b2:
  y = y + 5;
  if (y < 11)
    goto b1;

done:
  printf("%d %d\n", x, y);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut y: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut pick_b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut pick_a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 6usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             pick_a = 1 as i32;
// CHECK-NEXT:             pick_b = 0 as i32;
// CHECK-NEXT:             x = 0 as i32;
// CHECK-NEXT:             y = 0 as i32;
// CHECK-NEXT:             if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_a)) })
// CHECK-NEXT:                 != (0 as i32)
// CHECK-NEXT:             {
// CHECK-NEXT:                 __slate_state = 4;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 __slate_state = 5;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     x += 2 as i32;
// CHECK-NEXT:                     if x < (4 as i32) {
// CHECK-NEXT:                         __slate_state = 5;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     x += 1 as i32;
// CHECK-NEXT:                     if x < (3 as i32) {
// CHECK-NEXT:                         __slate_state = 4;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_b)) }) != (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             __slate_state = 1;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             __slate_state = 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     y += 5 as i32;
// CHECK-NEXT:                     if y < (11 as i32) {
// CHECK-NEXT:                         __slate_state = 2;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     y += 3 as i32;
// CHECK-NEXT:                     if y < (9 as i32) {
// CHECK-NEXT:                         __slate_state = 1;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{} {}", x, y);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
