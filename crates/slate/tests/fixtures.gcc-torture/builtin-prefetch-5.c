/* Test that __builtin_prefetch does no harm.

   Use addresses that are unlikely to be word-aligned.  Some targets
   have alignment requirements for prefetch addresses, so make sure the
   compiler takes care of that.  This fails if it aborts, anything else
   is OK.  */

void exit(int);

struct S {
  short a;
  short b;
  char  c[8];
} s;

char  arr[100];
char *ptr = arr;
int   idx = 3;

void arg_ptr(char *p) { __builtin_prefetch(p, 0, 0); }

void arg_idx(char *p, int i) { __builtin_prefetch(&p[i], 0, 0); }

void glob_ptr(void) { __builtin_prefetch(ptr, 0, 0); }

void glob_idx(void) { __builtin_prefetch(&ptr[idx], 0, 0); }

int main() {
  __builtin_prefetch(&s.b, 0, 0);
  __builtin_prefetch(&s.c[1], 0, 0);

  arg_ptr(&s.c[1]);
  arg_ptr(ptr + 3);
  arg_idx(ptr, 3);
  arg_idx(ptr + 1, 2);
  idx = 3;
  glob_ptr();
  glob_idx();
  ptr++;
  idx = 2;
  glob_ptr();
  glob_idx();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test that __builtin_prefetch does no harm.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Use addresses that are unlikely to be word-aligned.  Some targets
// CHECK-NEXT: //! have alignment requirements for prefetch addresses, so make sure the
// CHECK-NEXT: //! compiler takes care of that.  This fails if it aborts, anything else
// CHECK-NEXT: //! is OK.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn __builtin_prefetch([[V_1:__v[0-9]+]]: *const (), ...);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i16,
// CHECK-NEXT:     b: i16,
// CHECK-NEXT:     c: [i8; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut arr: __SlateAlign16<[i8; 100]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut ptr: *mut i8 = unsafe { std::ptr::addr_of_mut!(arr.0) as *mut i8 };
// CHECK-EMPTY:
// CHECK-NEXT: static mut idx: i32 = 3 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn arg_ptr(mut p: *mut i8) {
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn arg_idx(mut p: *mut i8, mut i: i32) {
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn glob_ptr() {
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn glob_idx() {
// CHECK-NEXT:     {};
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     {};
// CHECK-NEXT:     {};
// CHECK-NEXT:     arg_ptr(unsafe {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(s.c) as *mut i8 }.offset((1 as i32) as isize)
// CHECK-NEXT:     });
// CHECK-NEXT:     arg_ptr(unsafe { unsafe { ptr }.offset((3 as i32) as isize) });
// CHECK-NEXT:     arg_idx(unsafe { ptr }, 3 as i32);
// CHECK-NEXT:     arg_idx(
// CHECK-NEXT:         unsafe { unsafe { ptr }.offset((1 as i32) as isize) },
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         idx = 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     glob_ptr();
// CHECK-NEXT:     glob_idx();
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i8 = unsafe { ptr };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut i8 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ptr = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         idx = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     glob_ptr();
// CHECK-NEXT:     glob_idx();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
