#include <stdio.h>

// @check-fn-begin
int main(void) {
  int arr[10];
  arr[3] = 5;
  printf("%d\n", arr[3]);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut arr: __SlateAlign16<[i32; 10]> = __SlateAlign16([0 as i32; 10]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (arr.0.as_mut_ptr() as *mut i32).offset((3 as i32) as isize) } = 5
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { * unsafe { (arr.0.as_mut_ptr() as * mut i32).offset((3 as i32) as
// CHECK-NEXT:         isize) } }
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
