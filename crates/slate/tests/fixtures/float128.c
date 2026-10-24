__float128 add(__float128 a, __float128 b) { return a + b; }

int main(void) {
  __float128 one  = 1.0Q;
  __float128 tiny = 0x1p-100Q;
  __float128 sum  = add(one, tiny);
  if (sum == one)
    return 1;
  if (sum - one != tiny)
    return 2;
  if ((__float128)42 != 42.0Q)
    return 3;
  if ((int)42.75Q != 42)
    return 4;
  if ((double)1.5Q != 1.5)
    return 5;
  if (sizeof(__float128) != 16)
    return 6;
  if (_Alignof(__float128) != 16)
    return 7;
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(f128)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_libgcc {
// CHECK-NEXT:     #[link(name = "gcc", kind = "static")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-NEXT: fn add(a: f128, b: f128) -> f128 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let one: f128 = 1f128;
// CHECK-NEXT:     let tiny: f128 = 7.88860905221011805411728565282786229E-31f128;
// CHECK-NEXT:     let sum: f128 = add(one, tiny);
// CHECK-NEXT:     if sum == one {
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if sum - one != tiny {
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (42i32 as f128) != 42f128 {
// CHECK-NEXT:         return 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (42.75f128 as i32) != 42 {
// CHECK-NEXT:         return 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1.5f128 as f64) != 1.5f64 {
// CHECK-NEXT:         return 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     if false {
// CHECK-NEXT:         return 6;
// CHECK-NEXT:     }
// CHECK-NEXT:     if false {
// CHECK-NEXT:         return 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
