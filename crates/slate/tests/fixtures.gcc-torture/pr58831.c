#include <assert.h>

int   a, *b, c, d, f, **i, p, q, *r;
short o, j;

static int __attribute__((noinline, noclone)) fn1(int *p1, int **p2) {
  int **e = &b;
  for (; p; p++)
    *p1 = 1;
  *e = *p2 = &d;

  assert(r);

  return c;
}

static int **__attribute__((noinline, noclone)) fn2(void) {
  for (f = 0; f != 42; f++) {
    int *g[3] = {0, 0, 0};
    for (o = 0; o; o--)
      for (; a > 1;) {
        int **h[1] = {&g[2]};
      }
  }
  return &r;
}

int main(void) {
  i = fn2();
  fn1(b, i);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __assert_fail(
// CHECK-NEXT:         __assertion: *const i8,
// CHECK-NEXT:         __file: *const i8,
// CHECK-NEXT:         __line: u32,
// CHECK-NEXT:         __function: *const i8,
// CHECK-NEXT:     );
// CHECK-NEXT:     fn __assert_single_arg([[V_0:__v[0-9]+]]: bool) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut i: *mut *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut p: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut q: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut r: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut o: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut j: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn fn1(mut p1: *mut i32, mut p2: *mut *mut i32) -> i32 {
// CHECK-NEXT:     let mut e: *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(b) };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { p }) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *p1 = 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = unsafe { p };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             p = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut i32 = unsafe { std::ptr::addr_of_mut!(d) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p2 = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *e = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as u64;
// CHECK-NEXT:     if (unsafe { r }) != std::ptr::null_mut::<i32>() {
// CHECK-NEXT:         {}
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __assert_fail(
// CHECK-NEXT:                 c"r".as_ptr() as *const i8,
// CHECK-NEXT:                 c"{{.*}}tests/fixtures.gcc-torture/pr58831.c"
// CHECK-NEXT:                     .as_ptr() as *const i8,
// CHECK-NEXT:                 (12 as i32) as u32,
// CHECK-NEXT:                 c"int fn1(int *, int **)".as_ptr() as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { c }
// CHECK-NEXT: }
// CHECK-NEXT: fn fn2() -> *mut *mut i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { f }) != (42 as i32) {
// CHECK-NEXT:         let mut g: __SlateAlign16<[*mut i32; 3]> = __SlateAlign16([
// CHECK-NEXT:             std::ptr::null_mut::<i32>(),
// CHECK-NEXT:             std::ptr::null_mut::<i32>(),
// CHECK-NEXT:             std::ptr::null_mut::<i32>(),
// CHECK-NEXT:         ]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             o = (0 as i32) as i16;
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { o }) != (0 as i16) {
// CHECK-NEXT:             '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while (unsafe { a }) > (1 as i32) {
// CHECK-NEXT:                 [
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (g.0.as_mut_ptr() as *mut *mut i32).offset((2 as i32) as isize)
// CHECK-NEXT:                     },
// CHECK-NEXT:                 ];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i16 = unsafe { o };
// CHECK-NEXT:             let mut [[V_5:__v[0-9]+]]: i16 = (([[V_4]] as i32) - (1 as i32)) as i16;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 o = [[V_5]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = unsafe { f };
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             f = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::ptr::addr_of_mut!(r) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = fn2();
// CHECK-NEXT:     }
// CHECK-NEXT:     fn1(unsafe { b }, unsafe { i });
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
