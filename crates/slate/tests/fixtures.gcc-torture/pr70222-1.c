/* PR rtl-optimization/70222 */

int          a = 1;
unsigned int b = 2;
int          c = 0;
int          d = 0;

void foo() {
  int e = ((-(c >= c)) < b) > ((int)(-1ULL >> ((a / a) * 15)));
  d     = -e;
}

__attribute__((noinline, noclone)) void bar(int x) {
  if (x != -1)
    __builtin_abort();
}

int main() {
#if __CHAR_BIT__ == 8 && __SIZEOF_INT__ == 4 && __SIZEOF_LONG_LONG__ == 8
  foo();
  bar(d);
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/70222
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: u32 = (2 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut e: i32 = ((((-(((unsafe { c }) >= unsafe { c }) as i32) as u32) < unsafe { b }) as i32)
// CHECK-NEXT:         > ((((1 as u64).wrapping_neg() >> (unsafe { a }) / unsafe { a } * (15 as i32)) as u32)
// CHECK-NEXT:             as i32)) as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = -e;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i32) {
// CHECK-NEXT:     if x != -(1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     bar(unsafe { d });
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
