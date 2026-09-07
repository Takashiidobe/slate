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
// LOWERING-X86_64-GNU: #![feature(c_variadic)]
// LOWERING-X86_64-GNU-NEXT: #![allow(
// LOWERING-X86_64-GNU-NEXT:     dead_code,
// LOWERING-X86_64-GNU-NEXT:     unused,
// LOWERING-X86_64-GNU-NEXT:     non_camel_case_types,
// LOWERING-X86_64-GNU-NEXT:     non_snake_case,
// LOWERING-X86_64-GNU-NEXT:     non_upper_case_globals,
// LOWERING-X86_64-GNU-NEXT:     arithmetic_overflow,
// LOWERING-X86_64-GNU-NEXT:     unconditional_panic,
// LOWERING-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-X86_64-GNU-NEXT:     unused_comparisons
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_0: i32,
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_1: i32,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = gnu_extended_asm({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 19;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = gnu_numeric_operands({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = gnu_multiple_outputs();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn gnu_extended_asm({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("leal 3({1:e}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("addl $2, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("imull {1:e}, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, const 4, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn gnu_numeric_operands({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("subl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn gnu_multiple_outputs() -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut __asm_result: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-X86_64-GNU-NEXT:         __slate_anon_0: 0,
// LOWERING-X86_64-GNU-NEXT:         __slate_anon_1: 0,
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) {{__v[0-9]+}}, lateout(reg) {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU: #![feature(c_variadic)]
// REWRITES-X86_64-GNU-NEXT: #![allow(
// REWRITES-X86_64-GNU-NEXT:     dead_code,
// REWRITES-X86_64-GNU-NEXT:     unused,
// REWRITES-X86_64-GNU-NEXT:     non_camel_case_types,
// REWRITES-X86_64-GNU-NEXT:     non_snake_case,
// REWRITES-X86_64-GNU-NEXT:     non_upper_case_globals,
// REWRITES-X86_64-GNU-NEXT:     arithmetic_overflow,
// REWRITES-X86_64-GNU-NEXT:     unconditional_panic,
// REWRITES-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-X86_64-GNU-NEXT:     unused_comparisons
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_0: i32,
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_1: i32,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         printf(
// REWRITES-X86_64-GNU-NEXT:             c"%d %d %d\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             gnu_extended_asm(7),
// REWRITES-X86_64-GNU-NEXT:             gnu_numeric_operands(19, 4),
// REWRITES-X86_64-GNU-NEXT:             gnu_multiple_outputs(),
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     std::process::exit(0 as i32);
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn gnu_extended_asm({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("leal 3({1:e}), {0:e}", lateout(reg) {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("addl $2, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("imull {1:e}, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, in(reg) {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) {{__v[0-9]+}} => {{__v[0-9]+}}, const 4, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn gnu_numeric_operands({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("subl {1:e}, {0:e}", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, in(reg) {{arg[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn gnu_multiple_outputs() -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) {{__v[0-9]+}}, lateout(reg) {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}} * 10 + {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites
