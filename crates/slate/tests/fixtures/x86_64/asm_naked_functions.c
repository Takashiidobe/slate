#include <stdio.h>

__attribute__((naked)) int identity(int x) {
  __asm__("movl %edi, %eax\n\tret");
}

__attribute__((naked)) long add(long a, long b) {
  __asm__("leaq (%rdi,%rsi), %rax");
  ;
  __asm__("ret");
}

__attribute__((naked)) static int forty_two(void) {
  __asm__ volatile("movl %0, %%eax\n\tret" : : "i"(40 + 2));
}

__attribute__((naked)) int declared(void);

int declared(void) {
  __asm__("movl $5, %eax\n\tret # {braces}");
}

int main(void) {
  int (*fp)(int) = identity;
  printf("%d %ld %d %d %d\n", identity(9), add(30, 12), forty_two(), declared(), fp(3));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(naked)]
// CHECK-NEXT: extern "C-unwind" fn identity(x: i32) -> i32 {
// CHECK-NEXT:     core::arch::naked_asm!("movl %edi, %eax\n\tret", options(att_syntax));
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(naked)]
// CHECK-NEXT: extern "C-unwind" fn add(a: i64, b: i64) -> i64 {
// CHECK-NEXT:     core::arch::naked_asm!("leaq (%rdi,%rsi), %rax\nret", options(att_syntax));
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(naked)]
// CHECK-NEXT: extern "C-unwind" fn forty_two() -> i32 {
// CHECK-NEXT:     core::arch::naked_asm!(
// CHECK-NEXT:         "movl ${0}, %eax\n\tret", const 42 as i32, options(att_syntax)
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(naked)]
// CHECK-NEXT: extern "C-unwind" fn declared() -> i32 {
// CHECK-NEXT:     core::arch::naked_asm!("movl $5, %eax\n\tret # {{[{][{]}}braces}}", options(att_syntax));
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let fp: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(identity);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %ld %d %d %d\n".as_ptr(),
// CHECK-NEXT:             identity(9 as i32),
// CHECK-NEXT:             add(30 as i64, 12 as i64),
// CHECK-NEXT:             forty_two(),
// CHECK-NEXT:             declared(),
// CHECK-NEXT:             unsafe { fp.unwrap()(3 as i32) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
