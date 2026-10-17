#include <stdio.h>

// @check-fn-begin
void walk(int *position) {
  static void *labels[] = {&&again, &&done};
  goto        *labels[*position];
again:
  position++;
  goto *labels[*position];
done:
  return;
}
// @check-fn-end

int main(void) {
  int path[] = {0, 1};
  walk(path);
  printf("done\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn walk(mut position: *mut i32) {
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     let [[V_0:__v[0-9]+]]: *mut i32 = position;
// CHECK-NEXT:                     position = unsafe { [[V_0]].offset(1 as isize) };
// CHECK-NEXT:                     match (unsafe { labels.0[(unsafe { *position }) as usize] }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         4 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         5 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     match (unsafe { labels.0[(unsafe { *position }) as usize] }) as usize
// CHECK-NEXT:                     {
// CHECK-NEXT:                         4 => {
// CHECK-NEXT:                             __slate_state = 1usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         5 => {
// CHECK-NEXT:                             __slate_state = 0usize;
// CHECK-NEXT:                             continue;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
