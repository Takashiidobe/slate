/* PR rtl-optimization/64957 */

__attribute__((noinline, noclone)) int foo(int b) {
  return (((b ^ 5) | 1) ^ 5) | 1;
}

__attribute__((noinline, noclone)) int bar(int b) {
  return (((b ^ ~5) & ~1) ^ ~5) & ~1;
}

int main() {
  int i;
  for (i = 0; i < 16; i++)
    if (foo(i) != (i | 1) || bar(i) != (i & ~1))
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/64957
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut b: i32) -> i32 {
// CHECK-NEXT:     (b ^ (5 as i32) | (1 as i32)) ^ (5 as i32) | (1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut b: i32) -> i32 {
// CHECK-NEXT:     ((b ^ !(5 as i32)) & !(1 as i32) ^ !(5 as i32)) & !(1 as i32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (16 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:         if foo(i) != i | (1 as i32) {
// CHECK-NEXT:             [[V_0]] = true as bool;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_0]] = bar(i) != i & !(1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         if [[V_0]] {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
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
