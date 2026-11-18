#include <stdio.h>

register unsigned long current_stack_pointer __asm__("sp");
#define ASM_CALL_CONSTRAINT "+r"(current_stack_pointer)

__asm__(".text\n"
        ".globl slate_sp_helper\n"
        "slate_sp_helper:\n"
        "\tstr x30, [sp, #-16]!\n"
        "\tmov w9, #42\n"
        "\tldr x30, [sp], #16\n"
        "\tret\n");

static unsigned call_helper(void) {
    unsigned value;
    __asm__ volatile("bl slate_sp_helper\n\tmov %w0, w9"
                     : "=r"(value), ASM_CALL_CONSTRAINT
                     :
                     : "memory", "x9", "x30");
    return value;
}

static unsigned long stack_copy(void) {
    unsigned long copy;
    __asm__("mov %0, %1" : "=r"(copy) : "r"(current_stack_pointer));
    return copy;
}

static unsigned stack_low(void) {
    unsigned low;
    __asm__("mov %w0, %w1" : "=r"(low) : "r"(current_stack_pointer));
    return low;
}

int main(void) {
    unsigned value = call_helper();
    unsigned long copy = stack_copy();
    unsigned long direct = current_stack_pointer;
    printf("%u %d %d %d\n", value, copy != 0 && (copy & 15) == 0,
           (unsigned)stack_copy() == stack_low(), direct != 0 && (direct & 15) == 0);
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".text\n.globl slate_sp_helper\nslate_sp_helper:\n\tstr x30, [sp, #-16]!\n\tmov w9, #42\n\tldr x30, [sp], #16\n\tret\n"
// CHECK-NEXT: );
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn call_helper() -> u32 {
// CHECK-NEXT:     let mut value: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "bl slate_sp_helper\n\tmov {0:w}, w9", lateout(reg) [[T_0]], out("x9") _,
// CHECK-NEXT:             out("x30") _, options(preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         value = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     value
// CHECK-NEXT: }
// CHECK-NEXT: fn stack_copy() -> u64 {
// CHECK-NEXT:     let mut copy: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {0:x}, sp", lateout(reg) [[T_0]], options(pure, nomem, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         copy = unsafe { core::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     copy
// CHECK-NEXT: }
// CHECK-NEXT: fn stack_low() -> u32 {
// CHECK-NEXT:     let mut low: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov {0:w}, wsp", lateout(reg) [[T_0]], options(pure, nomem, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         low = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     low
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let value: u32 = call_helper();
// CHECK-NEXT:     let copy: u64 = stack_copy();
// CHECK-NEXT:     let direct: u64 = unsafe {
// CHECK-NEXT:         let [[T_0]]: usize;
// CHECK-NEXT:         core::arch::asm!("mov {}, sp", lateout(reg) [[T_0]]);
// CHECK-NEXT:         [[T_0]] as u64
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%u %d %d %d\n".as_ptr(),
// CHECK-NEXT:             value,
// CHECK-NEXT:             (copy != 0 && copy & 15 == 0) as i32,
// CHECK-NEXT:             ((stack_copy() as u32) == stack_low()) as i32,
// CHECK-NEXT:             (direct != 0 && direct & 15 == 0) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
