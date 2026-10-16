#include <stdio.h>

// @check-fn-begin
char *mutable_pick(int i) {
  if (i == 0) {
    return "mut";
  }
  return "other";
}
// @check-fn-end

// @check-fn-begin
const char *const_pick(int i) {
  if (i == 0) {
    return "const";
  }
  return "other";
}
// @check-fn-end

unsigned char *bytes_pick(void) { return (unsigned char *)"bytes"; }

// @check-fn-begin
int main(void) {
  printf("%s %s %s\n", mutable_pick(0), const_pick(0), (char *)bytes_pick());
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn mutable_pick(mut i: i32) -> *mut i8 {
// CHECK-NEXT:     if i == (0 as i32) {
// CHECK-NEXT:         return c"mut".as_ptr() as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     c"other".as_ptr() as *mut i8
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn const_pick(mut i: i32) -> *const i8 {
// CHECK-NEXT:     if i == (0 as i32) {
// CHECK-NEXT:         return c"const".as_ptr() as *const i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     c"other".as_ptr() as *const i8
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %s %s\n".as_ptr() as *const i8,
// CHECK-NEXT:             mutable_pick(0 as i32),
// CHECK-NEXT:             const_pick(0 as i32),
// CHECK-NEXT:             bytes_pick() as *mut i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
