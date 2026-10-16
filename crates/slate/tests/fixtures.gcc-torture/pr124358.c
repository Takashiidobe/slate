/* PR tree-optimization/124358 */

[[gnu::noipa]] void foo(int d) {
  static int u = 11;
  if (d != u++)
    __builtin_abort();
}

int main() {
  int a[1][4] = {11, 12, 13, 14};
  int (*p)[4] = a;
  for (int i = 0; i < 1; i++)
    for (int j = 0; j < 4; j++)
      foo(*(*(p + i) + j));
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/124358
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut d: i32) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { u };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if d != [[V_0]] {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: i32 = 11 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<{{\[\[}}i32; 4]; 1]> =
// CHECK-NEXT:         __SlateAlign16({{\[\[}}11 as i32, 12 as i32, 13 as i32, 14 as i32]]);
// CHECK-NEXT:     let mut p: *mut [i32; 4] = a.0.as_mut_ptr() as *mut [i32; 4];
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (1 as i32) {
// CHECK-NEXT:         let mut j: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < (4 as i32) {
// CHECK-NEXT:             foo(unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { (*unsafe { p.offset(i as isize) }).as_mut_ptr() as *mut i32 }
// CHECK-NEXT:                         .offset(j as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             });
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             j = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
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
