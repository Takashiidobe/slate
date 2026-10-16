/* PR 9700 */
/* Alpha got the base address for the va_list incorrect when there was
   a structure that was passed partially in registers and partially on
   the stack.  */

#include <stdarg.h>

void abort(void);

struct two {
  long x, y;
};

void foo(int a, int b, int c, int d, int e, struct two f, int g, ...) {
  va_list args;
  int     h;

  va_start(args, g);
  h = va_arg(args, int);
  if (g != 1 || h != 2)
    abort();
}

int main() {
  struct two t = {0, 0};
  foo(0, 0, 0, 0, 0, t, 1, 2);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR 9700
// CHECK-NEXT: //!
// CHECK-NEXT: //! Alpha got the base address for the va_list incorrect when there was
// CHECK-NEXT: //! a structure that was passed partially in registers and partially on
// CHECK-NEXT: //! the stack.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct two {
// CHECK-NEXT:     x: i64,
// CHECK-NEXT:     y: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C-unwind" fn foo(
// CHECK-NEXT:     mut a: i32,
// CHECK-NEXT:     mut b: i32,
// CHECK-NEXT:     mut c: i32,
// CHECK-NEXT:     mut d: i32,
// CHECK-NEXT:     mut e: i32,
// CHECK-NEXT:     mut f: two,
// CHECK-NEXT:     mut g: i32,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut args: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut h: i32 = 0 as i32;
// CHECK-NEXT:     args = __va_args.clone();
// CHECK-NEXT:     h = unsafe { args.next_arg::<i32>() };
// CHECK-NEXT:     if g != (1 as i32) || h != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: two = two {
// CHECK-NEXT:         x: (0 as i32) as i64,
// CHECK-NEXT:         y: (0 as i32) as i64,
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         foo(
// CHECK-NEXT:             0 as i32, 0 as i32, 0 as i32, 0 as i32, 0 as i32, t, 1 as i32, 2 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
