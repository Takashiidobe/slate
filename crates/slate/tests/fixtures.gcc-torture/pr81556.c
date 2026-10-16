/* PR tree-optimization/81556 */

unsigned long long int b = 0xb82ff73c5c020599ULL;
unsigned long long int c = 0xd4e8188733a29d8eULL;
unsigned long long int d = 2, f = 1, g = 0, h = 0;
unsigned long long int e = 0xf27771784749f32bULL;

__attribute__((noinline, noclone)) void foo(void) {
  _Bool a = d > 1;
  g       = f % ((d > 1) << 9);
  h       = a & (e & (a & b & c));
}

int main() {
  foo();
  if (g != 1 || h != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/81556
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u64 = 13272098465497875865 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: u64 = 15341539099603541390 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: u64 = ((2 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: u64 = ((1 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: u64 = ((0 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut h: u64 = ((0 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: u64 = 17471558040813171499 as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut a: bool = (unsafe { d }) > (((1 as i32) as i64) as u64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = (unsafe { f })
// CHECK-NEXT:             % ((((((unsafe { d }) > (((1 as i32) as i64) as u64)) as i32) << (9 as i32)) as i64)
// CHECK-NEXT:                 as u64);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         h = (((a as i32) as i64) as u64)
// CHECK-NEXT:             & ((unsafe { e }) & ((((a as i32) as i64) as u64) & unsafe { b } & unsafe { c }));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if (unsafe { g }) != (((1 as i32) as i64) as u64)
// CHECK-NEXT:         || (unsafe { h }) != (((0 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
