/* { dg-options "/Zc:preprocessor /arch:armv9.0 /feature:rcpc2" } */
#if __ARM_ARCH != 900 || defined(_M_ARM64_EXTENSION) || __ARM_FEATURE_RCPC != 2
#error "wrong /arch:armv9.0 /feature:rcpc2 predefines"
#endif

int main(void) {
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor /arch:armv9.0 /feature:rcpc2" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
