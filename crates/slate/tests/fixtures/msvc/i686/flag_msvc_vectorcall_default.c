/* { dg-options "/Zc:preprocessor /Gv /std:c11" } */
void __cdecl abort(void);

static int add(int a, int b) { return a + b; }

static double mix(int a, double b, float c, int d, long long e) {
  return a + b + c + d + e;
}

int main(void) {
  if (_Generic(&add, int(__vectorcall *)(int, int): 1, default: 0) != 1)
    abort();
  if (add(2, 3) != 5)
    abort();
  if (mix(1, 2.5, 0.25f, 4, 8) != 15.75)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(abi_vectorcall)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "/Zc:preprocessor /Gv /std:c11" }
// CHECK-NEXT: extern "vectorcall-unwind" fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "vectorcall-unwind" fn mix(a: i32, b: f64, c: f32, d: i32, e: i64) -> f64 {
// CHECK-NEXT:     (a as f64) + b + (c as f64) + (d as f64) + (e as f64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if false {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if add(2i32, 3i32) != 5 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if mix(1i32, 2.5f64, 0.25f32, 4i32, 8i64) != 15.75f64 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
