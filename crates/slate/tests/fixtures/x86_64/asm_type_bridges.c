#include <stdio.h>
struct word { unsigned int value; };
union half { unsigned short value; };
enum number { THREE = 3 };
static int answer(void) { return 42; }
int main(void) {
  _Bool flag = 1;
  signed char byte = -2;
  unsigned int tied = 0;
  int input = -4;
  enum number number = THREE;
  struct word word = { 40 };
  union half half = { 9 };
  int (*fn)(void) = answer;
  __asm__("incb %0" : "+r"(byte));
  __asm__("incl %0" : "=r"(tied) : "0"(input));
  __asm__("xorb $1, %0" : "+r"(flag));
  __asm__("incl %0" : "+r"(number));
  __asm__("addl $2, %0" : "+r"(word));
  __asm__("incw %0" : "+r"(half));
  __asm__("" : "+r"(fn));
  printf("%d %d %u %u %d\n", flag, number, word.value, half.value, fn());
  fn = 0;
  __asm__("" : "+r"(fn));
  printf("%d %d %u\n", fn == 0, byte, tied);
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct word {
// CHECK-NEXT:     value: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union half {
// CHECK-NEXT:     value: u16,
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.answer")]
// CHECK-NEXT: extern "C-unwind" fn answer() -> i32 {
// CHECK-NEXT:     42
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut flag: bool = true;
// CHECK-NEXT:     let mut byte: i8 = -2;
// CHECK-NEXT:     let mut tied: u32 = 0;
// CHECK-NEXT:     let mut input: i32 = -4;
// CHECK-NEXT:     let mut number: u32 = 3;
// CHECK-NEXT:     let mut word: word = word { value: 40 };
// CHECK-NEXT:     let mut half: half = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: half = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].value = 9u16;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut r#fn: Option<unsafe extern "C-unwind" fn() -> i32> = Some(answer);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incb {0:l}", inlateout(reg) (byte as u8) as u32 => [[T_1]], options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         byte = unsafe { std::mem::transmute::<u8, i8>([[T_1]] as u8) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incl {0:e}", inlateout(reg) input as u32 => [[T_2]], options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         tied = unsafe { std::mem::transmute::<u32, u32>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xorb $1, {0:l}", inlateout(reg) (flag as u8) as u32 => [[T_3]], options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = ([[T_3]] as u8) != 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incl {0:e}", inlateout(reg) number as u32 => [[T_4]], options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         number = unsafe { std::mem::transmute::<u32, u32>([[T_4]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $2, {0:e}", inlateout(reg) unsafe { std::mem::transmute::< word, u32 >
// CHECK-NEXT:             (word) } => [[T_5]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         word = unsafe { std::mem::transmute::<u32, word>([[T_5]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]]: u16;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incw {0:x}", inlateout(reg) unsafe { std::mem::transmute::< half, u16 >
// CHECK-NEXT:             (half) } => [[T_6]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         half = unsafe { std::mem::transmute::<u16, half>([[T_6]] as u16) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_7:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "\n# {0:r}", inlateout(reg) unsafe { std::mem::transmute::< Option < unsafe
// CHECK-NEXT:             extern "C-unwind" fn () -> i32 >, u64 > (r#fn) } => [[T_7]], options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r#fn = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 u64,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:             >([[T_7]] as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %u %u %d\n".as_ptr(),
// CHECK-NEXT:             flag as i32,
// CHECK-NEXT:             number as u32,
// CHECK-NEXT:             word.value,
// CHECK-NEXT:             (half.value as u32) as i32,
// CHECK-NEXT:             r#fn.unwrap()(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     r#fn = None;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_8:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "\n# {0:r}", inlateout(reg) unsafe { std::mem::transmute::< Option < unsafe
// CHECK-NEXT:             extern "C-unwind" fn () -> i32 >, u64 > (r#fn) } => [[T_8]], options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r#fn = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 u64,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:             >([[T_8]] as u64)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %d %u\n".as_ptr(), (r#fn == None) as i32, byte as i32, tied) };
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
