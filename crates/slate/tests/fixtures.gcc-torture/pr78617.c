int a = 0;
int d = 1;
int f = 1;

int fn1() { return a || 1 >> a; }

int fn2(int p1, int p2) { return p2 >= 2 ? p1 : p1 >> 1; }

int fn3(int p1) { return d ^ p1; }

int fn4(int p1, int p2) { return fn3(!d > fn2((f = fn1() - 1000) || p2, p1)); }

int main() {
  if (fn4(0, 0) != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = 0 as i32;
// CHECK-NEXT: static mut d: i32 = 1 as i32;
// CHECK-NEXT: static mut f: i32 = 1 as i32;
// CHECK-NEXT: fn fn1() -> i32 {
// CHECK-NEXT:     ((unsafe { a }) != (0 as i32) || (1 as i32) >> unsafe { a } != (0 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn fn2(mut p1: i32, mut p2: i32) -> i32 {
// CHECK-NEXT:     if p2 >= (2 as i32) { p1 } else { p1 >> (1 as i32) }
// CHECK-NEXT: }
// CHECK-NEXT: fn fn3(mut p1: i32) -> i32 {
// CHECK-NEXT:     (unsafe { d }) ^ p1
// CHECK-NEXT: }
// CHECK-NEXT: fn fn4(mut p1: i32, mut p2: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = fn1() - (1000 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     fn3(
// CHECK-NEXT:         ((!((unsafe { d }) != (0 as i32)) as i32)
// CHECK-NEXT:             > fn2(([[V_0]] != (0 as i32) || p2 != (0 as i32)) as i32, p1)) as i32,
// CHECK-NEXT:     )
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if fn4(0 as i32, 0 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
