#include <stdio.h>

static int count_down(int value) {
  int steps = 0;
  __asm__(".Lloop%=:\n\t"
          "testl %1, %1\n\t"
          "jz .Ldone%=\n\t"
          "decl %1\n\t"
          "incl %0\n\t"
          "jmp .Lloop%=\n"
          ".Ldone%=:"
          : "+r"(steps), "+r"(value)
          :
          : "cc");
  return steps;
}

static inline __attribute__((always_inline)) unsigned clamp_ten(unsigned value) {
  __asm__("cmpl $10, %0\n\t"
          "jbe skip_%=\n\t"
          "movl $10, %0\n"
          "skip_%=:"
          : "+r"(value)
          :
          : "cc");
  return value;
}

static unsigned clamp_sum(unsigned limit) {
  unsigned sum = 0;
  for (unsigned i = 0; i < limit; i++) {
    sum += clamp_ten(i) + clamp_ten(i * 3);
  }
  return sum;
}

static int first_char(void) {
  const char *text;
  __asm__(".pushsection .rodata\n"
          "str_%=: .asciz \"slate\"\n"
          ".popsection\n\t"
          "leaq str_%=(%%rip), %0"
          : "=r"(text));
  return text[0] + text[4];
}

static int with_own_label(int value) {
  __asm__("testl %0, %0\n\t"
          "jns 2f\n\t"
          "negl %0\n"
          "2:\n\t"
          "cmpl $100, %0\n\t"
          "jb small%=\n\t"
          "movl $100, %0\n"
          "small%=:"
          : "+r"(value)
          :
          : "cc");
  return value;
}

int main(void) {
  printf("%d %d %u %d %d %d\n", count_down(0), count_down(7), clamp_sum(16),
         first_char(), with_own_label(-42), with_own_label(500));
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
// CHECK-NEXT: fn count_down(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut steps: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "3:\n\ttestl {1:e}, {1:e}\n\tjz 2f\n\tdecl {1:e}\n\tincl {0:e}\n\tjmp 3b\n2:",
// CHECK-NEXT:             inlateout(reg) steps as u32 => [[T_0]], inlateout(reg) value as u32 => [[T_1]],
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         steps = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     steps
// CHECK-NEXT: }
// CHECK-NEXT: fn clamp_ten(mut value: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmpl $10, {0:e}\n\tjbe 2f\n\tmovl $10, {0:e}\n2:", inlateout(reg) value as
// CHECK-NEXT:             u32 => [[T_0]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn clamp_sum(limit: u32) -> u32 {
// CHECK-NEXT:     let mut sum: u32 = 0;
// CHECK-NEXT:     for i in 0..limit {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: u32 = clamp_ten(i).wrapping_add(clamp_ten(i.wrapping_mul(3)));
// CHECK-NEXT:         sum = sum.wrapping_add([[V_0]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn first_char() -> i32 {
// CHECK-NEXT:     let mut text: *const i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             ".pushsection .rodata\n2: .asciz \"slate\"\n.popsection\n\tleaq 2b(%rip), {0:r}",
// CHECK-NEXT:             lateout(reg) [[T_0]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         text = unsafe { std::mem::transmute::<u64, *const i8>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     ((unsafe { *text.add(0) }) as i32) + ((unsafe { *text.add(4) }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn with_own_label(mut value: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "testl {0:e}, {0:e}\n\tjns 2f\n\tnegl {0:e}\n2:\n\tcmpl $100, {0:e}\n\tjb 3f\n\tmovl $100, {0:e}\n3:",
// CHECK-NEXT:             inlateout(reg) value as u32 => [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %u %d %d %d\n".as_ptr(),
// CHECK-NEXT:             count_down(0i32),
// CHECK-NEXT:             count_down(7i32),
// CHECK-NEXT:             clamp_sum(16u32),
// CHECK-NEXT:             first_char(),
// CHECK-NEXT:             with_own_label(-42i32),
// CHECK-NEXT:             with_own_label(500i32),
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
