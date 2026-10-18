#include <stdio.h>

__asm__(".text\n"
        ".globl gnu_basic_asm_function\n"
        ".type gnu_basic_asm_function,@function\n"
        "gnu_basic_asm_function:\n"
        "movl $37, %eax\n"
        "ret\n"
        ".size gnu_basic_asm_function,.-gnu_basic_asm_function\n");

extern int gnu_basic_asm_function(void);

static int gnu_basic_asm_value;

static int gnu_function_basic_asm(void) {
  __asm__ volatile("movl $23, gnu_basic_asm_value(%rip)");
  return gnu_basic_asm_value;
}

static int gnu_extended_asm(int input) {
  int output;
  int tied;
  int early;
  int memory = 5;
  int zero;

  __asm__ volatile("leal 3(%1), %0" : "=r"(output) : "r"(input) : "cc");
  __asm__ volatile("addl $2, %0" : "+r"(output) : : "cc");
  __asm__("imull %[right], %[result]"
          : [result] "=r"(tied)
          : "0"(output), [right] "r"(2)
          : "cc");
  __asm__("movl %[left], %[result]\n\taddl %[right], %[result]"
          : [result] "=&r"(early)
          : [left] "r"(tied), [right] "r"(1)
          : "cc");
  __asm__("addl $%c[amount], %[value]"
          : [value] "+r"(early)
          : [amount] "i"(4)
          : "cc");
  __asm__ volatile("incl %[slot]" : [slot] "+m"(memory) : : "cc", "memory");
  __asm__("testl %[value], %[value]"
          : "=@ccz"(zero)
          : [value] "r"(input)
          : "cc");
  return early + memory + zero;
}

static int gnu_numeric_operands(int left, int right) {
  int result;
  __asm__("subl %2, %0" : "=r"(result) : "0"(left), "r"(right) : "cc");
  return result;
}

static int gnu_local_label(void) {
  __asm__ volatile("jmp 1f\n\t1:\n\tnop" : : : "memory");
  __asm__ inline volatile("" : : : "memory");
  return 17;
}

static int gnu_asm_goto(int value) {
  __asm__ goto("testl %[value], %[value]\n\tjz %l[zero]"
               :
               : [value] "r"(value)
               : "cc"
               : zero);
  return 11;
zero:
  return 13;
}

static int gnu_asm_goto_output(int value) {
  int output = value;
  __asm__ goto("testl %[output], %[output]\n\tjz %l[zero]"
               : [output] "+r"(output)
               :
               : "cc"
               : zero);
  return output + 1;
zero:
  return output + 2;
}

int main(void) {
  printf("%d %d %d %d %d %d %d %d\n", gnu_basic_asm_function(),
         gnu_function_basic_asm(), gnu_extended_asm(7),
         gnu_numeric_operands(19, 4), gnu_local_label(), gnu_asm_goto(0),
         gnu_asm_goto_output(5), gnu_asm_goto_output(0));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(asm_goto_with_outputs)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn gnu_basic_asm_function() -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut gnu_basic_asm_value: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".text\n.globl gnu_basic_asm_function\n.type gnu_basic_asm_function,@function\ngnu_basic_asm_function:\nmovl $37, %eax\nret\n.size gnu_basic_asm_function,.-gnu_basic_asm_function\n",
// CHECK-NEXT:     options(att_syntax)
// CHECK-NEXT: );
// CHECK-NEXT: fn gnu_function_basic_asm() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $23, {0}(%rip)", sym gnu_basic_asm_value, options(att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { gnu_basic_asm_value }
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_extended_asm(mut input: i32) -> i32 {
// CHECK-NEXT:     let mut output: i32 = 0;
// CHECK-NEXT:     let mut tied: i32 = 0;
// CHECK-NEXT:     let mut early: i32 = 0;
// CHECK-NEXT:     let mut memory: i32 = 5;
// CHECK-NEXT:     let mut zero: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leal 3({1:e}), {0:e}", lateout(reg) [[T_0]], in (reg) input as u32,
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         output = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $2, {0:e}", inlateout(reg) output as u32 => [[T_1]], options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         output = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "imull {1:e}, {0:e}", inlateout(reg) output as u32 => [[T_2]], in (reg) (2 as
// CHECK-NEXT:             i32) as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         tied = unsafe { std::mem::transmute::<u32, i32>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) [[T_3]], in (reg) tied as
// CHECK-NEXT:             u32, in (reg) (1 as i32) as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         early = unsafe { std::mem::transmute::<u32, i32>([[T_3]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl ${1}, {0:e}", inlateout(reg) early as u32 => [[T_4]], const 4 as i32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         early = unsafe { std::mem::transmute::<u32, i32>([[T_4]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "incl ({0:r})", in (reg) std::ptr::addr_of_mut!(memory), options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: u8;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "testl {1:e}, {1:e}\nsetz {0}", lateout(reg_byte) [[T_5]], in (reg) input as
// CHECK-NEXT:             u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         zero = unsafe { std::mem::transmute::<u32, i32>([[T_5]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     early + memory + zero
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_numeric_operands(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "subl {1:e}, {0:e}", inlateout(reg) left as u32 => [[T_0]], in (reg) right as
// CHECK-NEXT:             u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_local_label() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("jmp 1f\n\t1:\n\tnop", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     17
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_asm_goto(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return 13;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return 11;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     core::arch::asm!(
// CHECK-NEXT:                         "testl {0:e}, {0:e}\n\tjz {1}", in (reg) value as u32, label {
// CHECK-NEXT:                         __slate_state = 0usize; }, options(nostack, att_syntax)
// CHECK-NEXT:                     );
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_asm_goto_output(value: i32) -> i32 {
// CHECK-NEXT:     let mut output: std::mem::MaybeUninit<i32> = std::mem::MaybeUninit::uninit();
// CHECK-NEXT:     let [[SLATE_SLOT_0:__slate_slot_[0-9]+]]: *mut i32 = std::ptr::addr_of_mut!(output) as *mut i32;
// CHECK-NEXT:     let mut __slate_state: usize = 2usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     return *[[SLATE_SLOT_0]] + 2;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     return *[[SLATE_SLOT_0]] + 1;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     std::ptr::write([[SLATE_SLOT_0]], value);
// CHECK-NEXT:                     __slate_state = 1usize;
// CHECK-NEXT:                     let [[T_0]]: u32;
// CHECK-NEXT:                     core::arch::asm!(
// CHECK-NEXT:                         "testl {0:e}, {0:e}\n\tjz {1}", inlateout(reg) * [[SLATE_SLOT_0]]
// CHECK-NEXT:                         as u32 => [[T_0]], label { __slate_state = 0usize; },
// CHECK-NEXT:                         options(nostack, att_syntax)
// CHECK-NEXT:                     );
// CHECK-NEXT:                     *[[SLATE_SLOT_0]] = unsafe {
// CHECK-NEXT:                         std::mem::transmute::<u32, i32>([[T_0]] as u32)
// CHECK-NEXT:                     };
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {}", unsafe { gnu_basic_asm_function() },
// CHECK-NEXT:         gnu_function_basic_asm(), gnu_extended_asm(7 as i32), gnu_numeric_operands(19 as
// CHECK-NEXT:         i32, 4 as i32), gnu_local_label(), gnu_asm_goto(0 as i32), gnu_asm_goto_output(5
// CHECK-NEXT:         as i32), gnu_asm_goto_output(0 as i32)
// CHECK-NEXT:     );
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
