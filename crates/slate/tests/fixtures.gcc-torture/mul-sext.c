/* { dg-do run } */

typedef __INT64_TYPE__ int64_t;
typedef __INT32_TYPE__ int32_t;

/* f() was misoptimized to a single "mul.d" instruction on LA64.  */
__attribute__((noipa, noinline)) int64_t f(int64_t a, int64_t b) {
  return (int64_t)(int32_t)a * (int64_t)(int32_t)b;
}

int main() {
  int64_t a = 0x1145140000000001;
  int64_t b = 0x1919810000000001;
  if (f(a, b) != 1)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// f() was misoptimized to a single "mul.d" instruction on LA64.
// CHECK-NEXT: fn f(mut a: i64, mut b: i64) -> i64 {
// CHECK-NEXT:     ((a as i32) as i64) * ((b as i32) as i64)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i64 = 1244422862270365697 as i64;
// CHECK-NEXT:     let mut b: i64 = 1808618562365947905 as i64;
// CHECK-NEXT:     if f(a, b) != ((1 as i32) as i64) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
// SLATE-FILECHECK-END
