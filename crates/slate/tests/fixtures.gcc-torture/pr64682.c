/* PR rtl-optimization/64682 */

int a, b = 1;

__attribute__((noinline, noclone)) void foo(int x) {
  if (x != 5)
    __builtin_abort();
}

int main() {
  int i;
  for (i = 0; i < 56; i++)
    for (; a; a--)
      ;
  int *c = &b;
  if (*c)
    *c = 1 % (unsigned int)*c | 5;

  foo(b);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/64682
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     if x != (5 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (56 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:             {}
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = unsafe { a };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = [[V_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut c: *mut i32 = unsafe { std::ptr::addr_of_mut!(b) };
// CHECK-NEXT:     if (unsafe { *c }) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *c = (((1 as i32) as u32) % ((unsafe { *c }) as u32) | ((5 as i32) as u32)) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(unsafe { b });
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
