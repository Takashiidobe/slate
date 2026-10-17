#include <stdio.h>

// @check-fn-begin
int main(void) {
  int arr[10];
  arr[3] = 5;
  printf("%d\n", arr[3]);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut arr: __SlateAlign16<[i32; 10]> = __SlateAlign16([0 as i32; 10]);
// CHECK-NEXT:     arr.0[3] = 5;
// CHECK-NEXT:     println!("{}", arr.0[3]);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
