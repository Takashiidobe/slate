/* { dg-require-effective-target indirect_jumps } */
#include <setjmp.h>

extern void abort(void);
extern void exit(int);

static jmp_buf env;

void baz(void) { __asm__ volatile("" : : : "memory"); }

static inline int g(int x) {
  if (x) {
    baz();
    return 0;
  } else {
    baz();
    return 1;
  }
}

int f(int *e) {
  if (*e)
    return 1;

  int x = setjmp(env);
  int n = g(x);
  if (n == 0)
    exit(0);
  if (x)
    abort();
  longjmp(env, 42);
}

int main(int argc, char **argv) {
  int v = 0;
  return f(&v);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target indirect_jumps }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _setjmp(__env: *mut __jmp_buf_tag) -> i32;
// CHECK-NEXT:     fn longjmp(__env: *mut __jmp_buf_tag, __val: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __val: [u64; 16],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __jmp_buf_tag {
// CHECK-NEXT:     __jmpbuf: [i64; 8],
// CHECK-NEXT:     __mask_was_saved: i32,
// CHECK-NEXT:     __saved_mask: [[SLATERECORD_0]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut env: __SlateAlign16<[__jmp_buf_tag; 1]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn baz() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn g(mut x: i32) -> i32 {
// CHECK-NEXT:     if x != (0 as i32) {
// CHECK-NEXT:         baz();
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         baz();
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut e: &i32) -> i32 {
// CHECK-NEXT:     if (unsafe { *(e as *const i32) }) != (0 as i32) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x: i32 =
// CHECK-NEXT:         unsafe { _setjmp(unsafe { std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag }) };
// CHECK-NEXT:     let mut n: i32 = g(x);
// CHECK-NEXT:     if n == (0 as i32) {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if x != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         longjmp(
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag },
// CHECK-NEXT:             42 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut v: i32 = 0 as i32;
// CHECK-NEXT:     f(unsafe { &(*std::ptr::addr_of_mut!(v)) })
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
