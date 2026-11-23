#include <stdio.h>
struct word { unsigned int value; };
union half { unsigned short value; };
enum number { THREE = 3 };
static int answer(void) { return 42; }

static int aliasing_outputs(void) {
  int value = 0;
  int *alias = &value;
  __asm__ volatile("movl $11, %0; movl $22, %1" : "=r"(*alias), "=r"(value));
  return value;
}

static int raw_pointer_output(int *base) {
  int *p;
  __asm__("leaq 4(%1), %0" : "=r"(p) : "r"(base));
  __asm__("addq $4, %0" : "+r"(p));
  return *p;
}

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
  int array[3] = {10, 20, 30};
  printf("%d %d\n", aliasing_outputs(), raw_pointer_output(array));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct word {
// CHECK-NEXT:     value: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union half {
// CHECK-NEXT:     value: u16,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.answer")]
// CHECK-NEXT: extern "C-unwind" fn answer() -> i32 {
// CHECK-NEXT:     42
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn aliasing_outputs() -> i32 {
// CHECK-NEXT:     let mut value: i32 = 0;
// CHECK-NEXT:     let alias: *mut i32 = core::ptr::addr_of_mut!(value);
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "movl $11, {0:e}; movl $22, {1:e}",
// CHECK-NEXT:                 lateout(reg) [[T_0]],
// CHECK-NEXT:                 lateout(reg) [[T_1]],
// CHECK-NEXT:                 options(nostack, att_syntax),
// CHECK-NEXT:             );
// CHECK-NEXT:             *alias = core::mem::transmute::<u32, i32>([[T_0]] as u32);
// CHECK-NEXT:         }
// CHECK-NEXT:         value = unsafe { core::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn raw_pointer_output(base: *mut i32) -> i32 {
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq 4({1:r}), {0:r}",
// CHECK-NEXT:             lateout(reg) p,
// CHECK-NEXT:             in(reg) base as u64,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addq $4, {0:r}",
// CHECK-NEXT:             inlateout(reg) p => p,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *p }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut flag: bool = true;
// CHECK-NEXT:     let mut byte: i8 = -2;
// CHECK-NEXT:     let mut tied: u32 = 0;
// CHECK-NEXT:     let input: i32 = -4;
// CHECK-NEXT:     let mut number: u32 = 3;
// CHECK-NEXT:     let mut word: word = word { value: 40 };
// CHECK-NEXT:     let mut half: half = half { value: 9 };
// CHECK-NEXT:     let mut r#fn: Option<unsafe extern "C-unwind" fn() -> i32> = Some(answer);
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "incb {0:l}",
// CHECK-NEXT:                 inlateout(reg) (byte as u8) as u32 => [[T_0]],
// CHECK-NEXT:                 options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         byte = unsafe { core::mem::transmute::<u8, i8>([[T_0]] as u8) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incl {0:e}",
// CHECK-NEXT:             inlateout(reg) input as u32 => tied,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_1]]: u32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "xorb $1, {0:l}",
// CHECK-NEXT:                 inlateout(reg) (flag as u8) as u32 => [[T_1]],
// CHECK-NEXT:                 options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         flag = ([[T_1]] as u8) != 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incl {0:e}",
// CHECK-NEXT:             inlateout(reg) number => number,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "addl $2, {0:e}",
// CHECK-NEXT:                 inlateout(reg) core::mem::transmute::<word, u32>(word) => [[T_2]],
// CHECK-NEXT:                 options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         word = unsafe { core::mem::transmute::<u32, word>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "incw {0:x}",
// CHECK-NEXT:                 inlateout(reg) core::mem::transmute::<half, u16>(half) => [[T_3]],
// CHECK-NEXT:                 options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         half = unsafe { core::mem::transmute::<u16, half>([[T_3]] as u16) };
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: u64;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "\n# {0:r}",
// CHECK-NEXT:                 inlateout(reg) core::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> i32>, u64>(
// CHECK-NEXT:                     r#fn,
// CHECK-NEXT:                 ) => [[T_4]],
// CHECK-NEXT:                 options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         r#fn = unsafe {
// CHECK-NEXT:             core::mem::transmute::<u64, Option<unsafe extern "C-unwind" fn() -> i32>>(
// CHECK-NEXT:                 [[T_4]] as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %u %u %d\n".as_ptr(),
// CHECK-NEXT:             flag as i32,
// CHECK-NEXT:             number as u32,
// CHECK-NEXT:             word.value,
// CHECK-NEXT:             half.value as i32,
// CHECK-NEXT:             r#fn.unwrap()(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     r#fn = None;
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: u64;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::arch::asm!(
// CHECK-NEXT:                 "\n# {0:r}",
// CHECK-NEXT:                 inlateout(reg) core::mem::transmute::<Option<unsafe extern "C-unwind" fn() -> i32>, u64>(
// CHECK-NEXT:                     r#fn,
// CHECK-NEXT:                 ) => [[T_5]],
// CHECK-NEXT:                 options(pure, nomem, nostack, att_syntax),
// CHECK-NEXT:             );
// CHECK-NEXT:         }
// CHECK-NEXT:         r#fn = unsafe {
// CHECK-NEXT:             core::mem::transmute::<u64, Option<unsafe extern "C-unwind" fn() -> i32>>(
// CHECK-NEXT:                 [[T_5]] as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %u\n".as_ptr(),
// CHECK-NEXT:             (r#fn == None) as i32,
// CHECK-NEXT:             byte as i32,
// CHECK-NEXT:             tied,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut array: [i32; 3] = [10, 20, 30];
// CHECK-NEXT:     let __slate_printf_arg1 = raw_pointer_output(array.as_mut_ptr());
// CHECK-NEXT:     println!("{} {}", aliasing_outputs(), __slate_printf_arg1);
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
