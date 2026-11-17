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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn walk(mut position: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         match labels.0[*position as usize] as usize {
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 loop {
// CHECK-NEXT:                     let [[V_0:__v[0-9]+]]: *mut i32 = position;
// CHECK-NEXT:                     position = [[V_0]].add(1);
// CHECK-NEXT:                     match labels.0[*position as usize] as usize {
// CHECK-NEXT:                         4 => {}
// CHECK-NEXT:                         5 => {
// CHECK-NEXT:                             break;
// CHECK-NEXT:                         }
// CHECK-NEXT:                         _ => {
// CHECK-NEXT:                             unreachable!();
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             5 => {}
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 unreachable!();
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK: {{^}}}
// SLATE-FILECHECK-END
