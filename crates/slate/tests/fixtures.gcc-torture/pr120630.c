/* PR middle-end/120630 */

__attribute__((noipa)) int foo(const char *x, ...) { return *x; }

int      a, b, c;
unsigned d = 1;

int main() {
  if (a)
    foo("0");
  int e = -1;
  if (a < 1) {
    e = c;
    if (c)
      while (1)
        ;
  }
  b = (~e + 0UL) / -1;
  if (d > b)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/120630
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn foo(mut x: *const i8, mut __va_args: ...) -> i32 {
// CHECK-NEXT:     (unsafe { *x }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: u32 = (1 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:         unsafe { foo(c"0".as_ptr() as *const i8) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut e: i32 = -(1 as i32);
// CHECK-NEXT:     if (unsafe { a }) < (1 as i32) {
// CHECK-NEXT:         e = unsafe { c };
// CHECK-NEXT:         if (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:                 {}
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = ((((!e as i64) as u64).wrapping_add(0 as u64) / ((-(1 as i32) as i64) as u64)) as u32)
// CHECK-NEXT:             as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { d }) > ((unsafe { b }) as u32) {
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
