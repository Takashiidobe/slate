/* PR rtl-optimization/57130 */

struct S {
  int a, b, c, d;
} s[2] = {{6, 8, -8, -5}, {0, 2, -1, 2}};

__attribute__((noinline, noclone)) void foo(struct S r) {
  static int cnt;
  if (__builtin_memcmp(&r, &s[cnt++], sizeof r) != 0)
    __builtin_abort();
}

int main() {
  struct S r = {6, 8, -8, -5};
  foo(r);
  r = (struct S){0, 2, -1, 2};
  foo(r);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/57130
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT:     d: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: __SlateAlign16<[S; 2]> = __SlateAlign16([
// CHECK-NEXT:     S {
// CHECK-NEXT:         a: 6 as i32,
// CHECK-NEXT:         b: 8 as i32,
// CHECK-NEXT:         c: -(8 as i32),
// CHECK-NEXT:         d: -(5 as i32),
// CHECK-NEXT:     },
// CHECK-NEXT:     S {
// CHECK-NEXT:         a: 0 as i32,
// CHECK-NEXT:         b: 2 as i32,
// CHECK-NEXT:         c: -(1 as i32),
// CHECK-NEXT:         d: 2 as i32,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut r: S) {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = unsafe { cnt };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         cnt = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             std::ptr::addr_of_mut!(r) as *const (),
// CHECK-NEXT:             (unsafe { unsafe { std::ptr::addr_of_mut!(s.0) as *mut S }.offset([[V_3]] as isize) })
// CHECK-NEXT:                 as *const (),
// CHECK-NEXT:             16 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut cnt: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut r: S = S {
// CHECK-NEXT:         a: 6 as i32,
// CHECK-NEXT:         b: 8 as i32,
// CHECK-NEXT:         c: -(8 as i32),
// CHECK-NEXT:         d: -(5 as i32),
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(r);
// CHECK-NEXT:     r = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = S {
// CHECK-NEXT:                 a: 0 as i32,
// CHECK-NEXT:                 b: 2 as i32,
// CHECK-NEXT:                 c: -(1 as i32),
// CHECK-NEXT:                 d: 2 as i32,
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(r);
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
