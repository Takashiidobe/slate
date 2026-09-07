#include <stdio.h>

static int asm_probe(int x) {
  __asm__ volatile("" : "+r"(x));
  return x + 1;
}

int main(void) {
  volatile int input = 4;
  printf("%d\n", asm_probe(input));
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
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut input: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = asm_probe({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn asm_probe({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         core::arch::asm!("\n/* {0:e} */", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 1;
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
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let mut input: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), 4 as i32) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, asm_probe({{__v[0-9]+}})) };
// REWRITES-X86_64-GNU-NEXT:     std::process::exit(0 as i32);
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn asm_probe({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         core::arch::asm!("\n/* {0:e} */", inlateout(reg) {{arg[0-9]+}} => {{__v[0-9]+}}, options(att_syntax));
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}} + 1
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites
