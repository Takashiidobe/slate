/* PR tree-optimization/49161 */

extern void abort(void);

int c;

__attribute__((noinline, noclone)) void bar(int x) {
  if (x != c++)
    abort();
}

__attribute__((noinline, noclone)) void foo(int x) {
  switch (x) {
  case 3:
    goto l1;
  case 4:
    goto l2;
  case 6:
    goto l3;
  default:
    return;
  }
l1:
  goto l4;
l2:
  goto l4;
l3:
  bar(-1);
l4:
  bar(0);
  if (x != 4)
    bar(1);
  if (x != 3)
    bar(-1);
  bar(2);
}

int main() {
  foo(3);
  if (c != 3)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/49161
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i32) {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if x != [[V_0]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: i32 = x;
// CHECK-NEXT:             if [[T_0]] == (3 as i32) {
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if [[T_0]] == (4 as i32) {
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if [[T_0]] == (6 as i32) {
// CHECK-NEXT:                         bar(-(1 as i32));
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         return;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         bar(0 as i32);
// CHECK-NEXT:         if x != (4 as i32) {
// CHECK-NEXT:             bar(1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         if x != (3 as i32) {
// CHECK-NEXT:             bar(-(1 as i32));
// CHECK-NEXT:         }
// CHECK-NEXT:         bar(2 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(3 as i32);
// CHECK-NEXT:     if (unsafe { c }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
