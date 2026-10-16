/* PR rtl-optimization/28096 */
/* Origin: Jan Stein <jan@gatespacetelematics.com> */

extern void abort(void);

int bar(int, int) __attribute__((noinline));
int bar(int a, int b) {
  if (b != 1)
    abort();
}

void foo(int, int) __attribute__((noinline));
void foo(int e, int n) {
  int i, bb2, bb5;

  if (e > 0)
    e = -e;

  for (i = 0; i < n; i++) {
    if (e >= 0) {
      bb2 = 0;
      bb5 = 0;
    } else {
      bb5 = -e;
      bb2 = bb5;
    }

    bar(bb5, bb2);
  }
}

int main(void) {
  foo(1, 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/28096
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Jan Stein <jan@gatespacetelematics.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     if b != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut e: i32, mut n: i32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut bb2: i32 = 0 as i32;
// CHECK-NEXT:     let mut bb5: i32 = 0 as i32;
// CHECK-NEXT:     if e > (0 as i32) {
// CHECK-NEXT:         e = -e;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < n {
// CHECK-NEXT:         if e >= (0 as i32) {
// CHECK-NEXT:             bb2 = 0 as i32;
// CHECK-NEXT:             bb5 = 0 as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             bb5 = -e;
// CHECK-NEXT:             bb2 = bb5;
// CHECK-NEXT:         }
// CHECK-NEXT:         bar(bb5, bb2);
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(1 as i32, 1 as i32);
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
