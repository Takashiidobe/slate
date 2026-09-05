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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:     __slate_anon_0: i32,
// LOWERING-NEXT:     __slate_anon_1: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_extended_asm({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 19;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_numeric_operands({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = gnu_multiple_outputs();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_extended_asm({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("leal 3({1:e}), {0:e}", lateout(reg) {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("addl $2, {0:e}", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}}, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("imull {1:e}, {0:e}", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}}, in(reg) {{_v[0-9]+}}, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) {{_v[0-9]+}}, in(reg) {{_v[0-9]+}}, in(reg) {{_v[0-9]+}}, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}}, const 4, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_numeric_operands({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("subl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_multiple_outputs() -> i32 {
// LOWERING-NEXT:     let mut __asm_result: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: 0,
// LOWERING-NEXT:         __slate_anon_1: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) {{_v[0-9]+}}, lateout(reg) {{_v[0-9]+}}, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:     __slate_anon_0: i32,
// REWRITES-NEXT:     __slate_anon_1: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// REWRITES-NEXT:             gnu_extended_asm(7),
// REWRITES-NEXT:             gnu_numeric_operands(19, 4),
// REWRITES-NEXT:             gnu_multiple_outputs(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_extended_asm({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         core::arch::asm!("leal 3({1:e}), {0:e}", lateout(reg) {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         core::arch::asm!("addl $2, {0:e}", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}}, options(att_syntax));
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         core::arch::asm!("imull {1:e}, {0:e}", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}}, in(reg) {{_v[0-9]+}}, options(att_syntax));
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         core::arch::asm!("movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) {{_v[0-9]+}}, in(reg) {{_v[0-9]+}}, in(reg) {{_v[0-9]+}}, options(att_syntax));
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) {{_v[0-9]+}} => {{_v[0-9]+}}, const 4, options(att_syntax));
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_numeric_operands({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         core::arch::asm!("subl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{_v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_multiple_outputs() -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         core::arch::asm!("movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) {{_v[0-9]+}}, lateout(reg) {{_v[0-9]+}}, options(att_syntax));
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{_v[0-9]+}} * 10 + {{_v[0-9]+}}
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
