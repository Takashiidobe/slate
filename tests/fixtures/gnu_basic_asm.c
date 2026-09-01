
// LOWERING-DAG: #[cfg(target_arch = "x86_64")]
// LOWERING-DAG: core::arch::global_asm!(".text\n.globl gnu_basic_asm_function
// LOWERING-DAG: #[unsafe(no_mangle)]
// LOWERING-DAG: static mut gnu_basic_asm_value: i32 = 0;
// LOWERING-LABEL: {{^}}fn gnu_function_basic_asm() -> i32 {
// LOWERING-DAG: core::arch::asm!("movl $23, gnu_basic_asm_value(%rip)", options(att_syntax, raw));
// LOWERING: {{^}}}

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

int main(void) {
  printf("%d %d\n", gnu_basic_asm_function(), gnu_function_basic_asm());
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[cfg(target_arch = "x86_64")]
// REWRITES-NEXT: core::arch::global_asm!(
// REWRITES-NEXT:     ".text\n.globl gnu_basic_asm_function\n.type gnu_basic_asm_function,@function\ngnu_basic_asm_function:\nmovl $37, %eax\nret\n.size gnu_basic_asm_function,.-gnu_basic_asm_function\n",
// REWRITES-NEXT:     options(att_syntax, raw)
// REWRITES-NEXT: );
// REWRITES-EMPTY:
// REWRITES-NEXT: #[unsafe(no_mangle)]
// REWRITES-NEXT: static mut gnu_basic_asm_value: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn gnu_basic_asm_function() -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_function_basic_asm() -> i32 {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         core::arch::asm!(
// REWRITES-NEXT:             "movl $23, gnu_basic_asm_value(%rip)",
// REWRITES-NEXT:             options(att_syntax, raw)
// REWRITES-NEXT:         );
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return unsafe { gnu_basic_asm_value };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// REWRITES-NEXT:             unsafe { gnu_basic_asm_function() },
// REWRITES-NEXT:             gnu_function_basic_asm(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
