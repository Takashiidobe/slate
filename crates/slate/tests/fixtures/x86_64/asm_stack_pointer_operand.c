#include <stdio.h>

register unsigned long current_stack_pointer __asm__("rsp");
#define ASM_CALL_CONSTRAINT "+r"(current_stack_pointer)

int helper_value = 7;

__asm__(".text\n"
        ".globl slate_sp_helper\n"
        "slate_sp_helper:\n"
        "\tmovl $42, helper_value(%rip)\n"
        "\tret\n");

static unsigned int count(unsigned int w) {
    unsigned int res;
    __asm__("popcntl %[val], %[cnt]" : [cnt] "=r"(res), ASM_CALL_CONSTRAINT : [val] "r"(w));
    return res;
}

static int stack_nonzero(void) {
    unsigned long copy;
    __asm__("movq %1, %0" : "=r"(copy) : "r"(current_stack_pointer));
    return copy != 0 && (copy & 7) == 0;
}

int main(void) {
    __asm__ volatile("call slate_sp_helper" : ASM_CALL_CONSTRAINT : : "memory");
    printf("%d %u %u %d\n", helper_value, count(0xff), count(0x10001), stack_nonzero());
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut helper_value: i32 = 7;
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".text\n.globl slate_sp_helper\nslate_sp_helper:\n\tmovl $42, {0}(%rip)\n\tret\n",
// CHECK-NEXT:     sym helper_value, options(att_syntax)
// CHECK-NEXT: );
// CHECK-NEXT: fn count(mut w: u32) -> u32 {
// CHECK-NEXT:     let mut res: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "popcntl {1:e}, {0:e}", lateout(reg) [[T_0]], in (reg) w as u32, options(pure,
// CHECK-NEXT:             nomem, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         res = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     res
// CHECK-NEXT: }
// CHECK-NEXT: fn stack_nonzero() -> i32 {
// CHECK-NEXT:     let mut copy: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq %rsp, {0:r}", lateout(reg) [[T_0]], options(pure, nomem, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         copy = unsafe { std::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (copy != 0 && copy & 7 == 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("call slate_sp_helper", options(att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %u %u %d\n".as_ptr(),
// CHECK-NEXT:             helper_value,
// CHECK-NEXT:             count(255u32),
// CHECK-NEXT:             count(65537u32),
// CHECK-NEXT:             stack_nonzero(),
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
