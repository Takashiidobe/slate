/* PR tree-optimization/66233 */

unsigned int v[8];

__attribute__((noinline, noclone)) void foo(void) {
  int i;
  for (i = 0; i < 8; i++)
    v[i] = (float)i;
}

int main() {
  unsigned int i;
  foo();
  for (i = 0; i < 8; i++)
    if (v[i] != i)
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/66233
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: __SlateAlign16<[u32; 8]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (8 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(v.0) as *mut u32 }.offset(i as isize) } =
// CHECK-NEXT:                 (i as f32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     foo();
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < ((8 as i32) as u32) {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(v.0) as *mut u32 }.offset(i as isize) }
// CHECK-NEXT:         }) != i
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = [[V_2]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_3]];
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
