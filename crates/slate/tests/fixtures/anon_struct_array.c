#include <stdio.h>

// @check-fn-begin
int main(void) {
  struct {
    int         code;
    const char *message;
    double      confidence;
  } error_log[] = {
      {404, "Not Found", 0.99},
      {500, "Internal Server Error", 0.85},
      {200, "OK", 1.00},
  };

  printf("%d\n", error_log[0].code);
  return 0;
}
// @check-fn-end

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut error_log: __SlateAlign16<[[[SLATERECORD_0:__SlateRecord[0-9]+]]; 3]> = __SlateAlign16([
// CHECK-NEXT:         [[SLATERECORD_0]] {
// CHECK-NEXT:             code: 404 as i32,
// CHECK-NEXT:             message: c"Not Found".as_ptr() as *const i8,
// CHECK-NEXT:             confidence: 0.99f64,
// CHECK-NEXT:         },
// CHECK-NEXT:         [[SLATERECORD_0]] {
// CHECK-NEXT:             code: 500 as i32,
// CHECK-NEXT:             message: c"Internal Server Error".as_ptr() as *const i8,
// CHECK-NEXT:             confidence: 0.85f64,
// CHECK-NEXT:         },
// CHECK-NEXT:         [[SLATERECORD_0]] {
// CHECK-NEXT:             code: 200 as i32,
// CHECK-NEXT:             message: c"OK".as_ptr() as *const i8,
// CHECK-NEXT:             confidence: 1.0f64,
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", unsafe { (* unsafe { (error_log.0.as_mut_ptr() as * mut [[SLATERECORD_0]])
// CHECK-NEXT:         .offset((0 as i32) as isize) }).code }
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// SLATE-FILECHECK-END
