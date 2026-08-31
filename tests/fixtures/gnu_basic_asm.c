
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[cfg(target_arch = "x86_64")]
// REWRITES-NEXT: core::arch::global_asm!(".text\n.globl gnu_basic_asm_function\n.type gnu_basic_asm_function,@function\ngnu_basic_asm_function:\nmovl $37, %eax\nret\n.size gnu_basic_asm_function,.-gnu_basic_asm_function\n", options(att_syntax, raw));
// REWRITES-EMPTY:
// REWRITES-NEXT: #[unsafe(no_mangle)]
// REWRITES-NEXT: static mut gnu_basic_asm_value: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn gnu_basic_asm_function() -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_function_basic_asm() -> i32 {
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("movl $23, gnu_basic_asm_value(%rip)", options(att_syntax, raw));
// REWRITES-NEXT: }
// REWRITES-NEXT: return unsafe { gnu_basic_asm_value };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { gnu_basic_asm_function() };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = gnu_function_basic_asm();
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites

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
