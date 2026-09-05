// { dg-do run { target x86_64-*-* } }
#include <stdio.h>

static int gnu_extended_asm(int input) {
  int output;
  int tied;
  int early;

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
  return early;
}

static int gnu_numeric_operands(int left, int right) {
  int result;
  __asm__("subl %2, %0" : "=r"(result) : "0"(left), "r"(right) : "cc");
  return result;
}

static int gnu_multiple_outputs(void) {
  int left;
  int right;
  __asm__("movl $3, %0\n\tmovl $4, %1" : "=r"(left), "=r"(right));
  return left * 10 + right;
}

int main(void) {
  printf("%d %d %d\n", gnu_extended_asm(7), gnu_numeric_operands(19, 4),
         gnu_multiple_outputs());
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// COMMON-LOWERING-NEXT:     __slate_anon_0: i32,
// COMMON-LOWERING-NEXT:     __slate_anon_1: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_extended_asm({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 19;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_numeric_operands({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_multiple_outputs();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_extended_asm({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         core::arch::asm!("leal 3({1:e}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         core::arch::asm!("addl $2, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         core::arch::asm!("imull {1:e}, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, options(att_syntax));
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         core::arch::asm!("movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, options(att_syntax));
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, const 4, options(att_syntax));
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_numeric_operands({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         core::arch::asm!("subl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_multiple_outputs() -> i32 {
// COMMON-LOWERING-NEXT:     let mut __asm_result: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// COMMON-LOWERING-NEXT:         __slate_anon_0: 0,
// COMMON-LOWERING-NEXT:         __slate_anon_1: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         core::arch::asm!("movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) {{__v[0-9]+}}, lateout(reg) {{__v[0-9]+}}, options(att_syntax));
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// COMMON-REWRITES-NEXT:     __slate_anon_0: i32,
// COMMON-REWRITES-NEXT:     __slate_anon_1: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             gnu_extended_asm(7),
// COMMON-REWRITES-NEXT:             gnu_numeric_operands(19, 4),
// COMMON-REWRITES-NEXT:             gnu_multiple_outputs(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_extended_asm({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         core::arch::asm!("leal 3({1:e}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         core::arch::asm!("addl $2, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         core::arch::asm!("imull {1:e}, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, options(att_syntax));
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         core::arch::asm!("movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, options(att_syntax));
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, const 4, options(att_syntax));
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_numeric_operands({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         core::arch::asm!("subl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_multiple_outputs() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         core::arch::asm!("movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) {{__v[0-9]+}}, lateout(reg) {{__v[0-9]+}}, options(att_syntax));
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} * 10 + {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
