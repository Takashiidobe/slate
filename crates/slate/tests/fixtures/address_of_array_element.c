#include <stdio.h>

int read_pointer(int *pointer) { return *pointer; }

// @check-fn-begin
int read_array_element_through_call(void) {
  int  values[1] = {10};
  int *pointer   = &values[0];
  return read_pointer(pointer);
}
// @check-fn-end

// @check-fn-begin
int main(void) {
  int  values[4] = {2, 4, 6, 8};
  int *p         = &values[1];
  int *q         = &values[3];
  printf("%d %ld %d\n", *p + *q, q - p, read_array_element_through_call());
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn read_array_element_through_call() -> i32 {
// CHECK-NEXT:     let mut values: [i32; 1] = [10];
// CHECK-NEXT:     let pointer: *mut i32 = unsafe {
// CHECK-NEXT:         (values.as_mut_ptr() as *mut i32).offset(0 as isize)
// CHECK-NEXT:     };
// CHECK-NEXT:     read_pointer(pointer)
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let p: *mut i32 = unsafe { (values.0.as_mut_ptr() as *mut i32).offset(1 as isize) };
// CHECK-NEXT:     let q: *mut i32 = unsafe { (values.0.as_mut_ptr() as *mut i32).offset(3 as isize) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %ld %d\n".as_ptr(),
// CHECK-NEXT:             (unsafe { *p }) + unsafe { *q },
// CHECK-NEXT:             (unsafe { q.offset_from(p as *mut i32) }) as i64,
// CHECK-NEXT:             read_array_element_through_call(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
