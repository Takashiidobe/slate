/* PR tree-optimization/49419 */

extern void abort(void);

struct S {
  int w, x, y;
} *t;

int foo(int n, int f, int *s, int m) {
  int x, i, a;
  if (n == -1)
    return 0;
  for (x = n, i = 0; t[x].w == f && i < m; i++)
    x = t[x].x;
  if (i == m)
    abort();
  a = i + 1;
  for (x = n; i > 0; i--) {
    s[i] = t[x].y;
    x    = t[x].x;
  }
  s[0] = x;
  return a;
}

int main(void) {
  int      s[3], i;
  struct S buf[3] = {{1, 1, 2}, {0, 0, 0}, {0, 0, 0}};
  t               = buf;
  if (foo(0, 1, s, 3) != 2)
    abort();
  if (s[0] != 1 || s[1] != 2)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/49419
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     w: i32,
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut t: *mut S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut n: i32, mut f: i32, mut s: *mut i32, mut m: i32) -> i32 {
// CHECK-NEXT:     let mut x: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     if n == -(1 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     x = n;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { (*unsafe { unsafe { t }.offset(x as isize) }).w }) == f
// CHECK-NEXT:         && i < m
// CHECK-NEXT:     {
// CHECK-NEXT:         x = unsafe { (*unsafe { unsafe { t }.offset(x as isize) }).x };
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if i == m {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     a = i + (1 as i32);
// CHECK-NEXT:     x = n;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i > (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { s.offset(i as isize) } =
// CHECK-NEXT:                 unsafe { (*unsafe { unsafe { t }.offset(x as isize) }).y };
// CHECK-NEXT:         }
// CHECK-NEXT:         x = unsafe { (*unsafe { unsafe { t }.offset(x as isize) }).x };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { s.offset((0 as i32) as isize) } = x;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: [i32; 3] = [0 as i32; 3];
// CHECK-NEXT:     let mut buf: __SlateAlign16<[S; 3]> = __SlateAlign16([
// CHECK-NEXT:         S {
// CHECK-NEXT:             w: 1 as i32,
// CHECK-NEXT:             x: 1 as i32,
// CHECK-NEXT:             y: 2 as i32,
// CHECK-NEXT:         },
// CHECK-NEXT:         S {
// CHECK-NEXT:             w: 0 as i32,
// CHECK-NEXT:             x: 0 as i32,
// CHECK-NEXT:             y: 0 as i32,
// CHECK-NEXT:         },
// CHECK-NEXT:         S {
// CHECK-NEXT:             w: 0 as i32,
// CHECK-NEXT:             x: 0 as i32,
// CHECK-NEXT:             y: 0 as i32,
// CHECK-NEXT:         },
// CHECK-NEXT:     ]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         t = buf.0.as_mut_ptr() as *mut S;
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(0 as i32, 1 as i32, s.as_mut_ptr() as *mut i32, 3 as i32) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { (s.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (1 as i32)
// CHECK-NEXT:         || (unsafe { *unsafe { (s.as_mut_ptr() as *mut i32).offset((1 as i32) as isize) } })
// CHECK-NEXT:             != (2 as i32)
// CHECK-NEXT:     {
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
