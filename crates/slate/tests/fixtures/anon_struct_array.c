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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn __slate_main() -> i32 {
// CHECK-NEXT:     let error_log: __SlateAlign16<[__SlateRecord_anon_struct_array_c_9; 3]> = __SlateAlign16([
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: __SlateRecord_anon_struct_array_c_9 = unsafe {
// CHECK-NEXT:                 std::mem::zeroed()
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_0]].code = 404i32;
// CHECK-NEXT:             [[T_0]].message = c"Not Found".as_ptr();
// CHECK-NEXT:             [[T_0]].confidence = 0.99f64;
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: __SlateRecord_anon_struct_array_c_9 = unsafe {
// CHECK-NEXT:                 std::mem::zeroed()
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_1]].code = 500i32;
// CHECK-NEXT:             [[T_1]].message = c"Internal Server Error".as_ptr();
// CHECK-NEXT:             [[T_1]].confidence = 0.85f64;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         },
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: __SlateRecord_anon_struct_array_c_9 = unsafe {
// CHECK-NEXT:                 std::mem::zeroed()
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_2]].code = 200i32;
// CHECK-NEXT:             [[T_2]].message = c"OK".as_ptr();
// CHECK-NEXT:             [[T_2]].confidence = 1.0f64;
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     println!("{}", error_log.0[0].code);
// CHECK-NEXT:     0
// CHECK: {{^}}}
// SLATE-FILECHECK-END
