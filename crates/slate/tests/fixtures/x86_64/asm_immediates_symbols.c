#include <stdio.h>
static int values[] = { 17, 29, 41 };
static int answer(void) { return 53; }
int main(void) {
  int result = 10;
  int only_negated = 0;
  int *positive;
  int *negative;
  int (*fn)(void);
  const char *text;
  __asm__("addl %1, %0\n\taddl $%n1, %0\n\taddl $%P1, %0"
          : "+r"(result) : "i"(4));
  __asm__("addl $%n1, %0" : "+r"(only_negated) : "i"(-5));
  __asm__("leaq %c1(%%rip), %0" : "=r"(positive) : "s"(&values[2]));
  __asm__("leaq %p1(%%rip), %0" : "=r"(negative) : "Ws"((char *)&values[2] - 4));
  __asm__("leaq %P1(%%rip), %0" : "=r"(fn) : "i"(answer));
  __asm__("leaq %c1(%%rip), %0" : "=r"(text) : "i"("hello"));
  printf("%d %d %d %d %s\n", result + only_negated, *positive, *negative, fn(), text);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut values: [i32; 3] = [17 as i32, 29 as i32, 41 as i32];
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.answer")]
// CHECK-NEXT: extern "C-unwind" fn answer() -> i32 {
// CHECK-NEXT:     53
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut result: i32 = 10;
// CHECK-NEXT:     let mut only_negated: i32 = 0;
// CHECK-NEXT:     let mut positive: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut negative: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut r#fn: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut text: *const i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl ${1}, {0:e}\n\taddl $-4, {0:e}\n\taddl ${1}, {0:e}", inlateout(reg)
// CHECK-NEXT:             result as u32 => [[T_0]], const 4 as i32, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $5, {0:e}\n# {1}", inlateout(reg) only_negated as u32 => [[T_1]], const -
// CHECK-NEXT:             5 as i32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         only_negated = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq {1}+8(%rip), {0:r}", lateout(reg) [[T_2]], sym values, options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         positive = unsafe { std::mem::transmute::<u64, *mut i32>([[T_2]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq {1}+4(%rip), {0:r}", lateout(reg) [[T_3]], sym values, options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         negative = unsafe { std::mem::transmute::<u64, *mut i32>([[T_3]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq {1}(%rip), {0:r}", lateout(reg) [[T_4]], sym answer, options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r#fn = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 u64,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:             >([[T_4]] as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq {1}(%rip), {0:r}", lateout(reg) [[T_5]], sym __slate_asm_string_12,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         text = unsafe { std::mem::transmute::<u64, *const i8>([[T_5]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d %s\n".as_ptr(),
// CHECK-NEXT:             result + only_negated,
// CHECK-NEXT:             unsafe { *positive },
// CHECK-NEXT:             unsafe { *negative },
// CHECK-NEXT:             unsafe { r#fn.unwrap()() },
// CHECK-NEXT:             text,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static __slate_asm_string_12: [u8; 6] = *b"hello\0";
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
