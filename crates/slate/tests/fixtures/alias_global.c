int        real_global = 12;
extern int alias_global __attribute__((alias("real_global")));

int main(void) {
  int result;
  result = alias_global;
  return result;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: static mut real_global: i32 = 12 as i32;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let result: i32 = unsafe { real_global };
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
