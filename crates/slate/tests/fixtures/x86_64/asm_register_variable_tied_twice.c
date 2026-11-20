#include <stdio.h>

static long tied_twice(long x) {
  register long t __asm__("r11") = x;
  __asm__("addq %[t], %0" : "+r"(t) : [t] "r"(t));
  return t;
}

static long input_twice(long x) {
  register long t __asm__("r11") = x;
  long out;
  __asm__("leaq (%1,%2), %0" : "=r"(out) : "r"(t), "r"(t));
  return out;
}

int main(void) {
  printf("%ld %ld\n", tied_twice(21), input_twice(50));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn tied_twice(mut t: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addq %r11, %r11",
// CHECK-NEXT:             inlateout("r11") t as u64 => [[T_0]],
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         t = unsafe { core::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     t
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn input_twice(mut t: i64) -> i64 {
// CHECK-NEXT:     let mut out: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq (%r11,%r11), {0:r}",
// CHECK-NEXT:             lateout(reg) [[T_0]],
// CHECK-NEXT:             in("r11") t as u64,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         out = unsafe { core::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %ld\n".as_ptr(),
// CHECK-NEXT:             tied_twice(21i64),
// CHECK-NEXT:             input_twice(50i64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
