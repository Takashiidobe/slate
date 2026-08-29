#include <stdio.h>

int main(void) {
  int left;
  int right;
  __asm__("movl $3, %0\n\tmovl $4, %1" : "=r"(left), "=r"(right));
  printf("%d _v9 anon_4 anon_struct_i32\n", left * 10 + right);
  return 0;
}
// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// REWRITES-NEXT:     __slate_anon_0: i32,
// REWRITES-NEXT:     __slate_anon_1: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut left: i32 = 0;
// REWRITES-NEXT: let mut right: i32 = 0;
// REWRITES-NEXT: let mut __asm_result: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} { __slate_anon_0: 0, __slate_anon_1: 0 };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) {{_v[0-9]+}}, lateout(reg) {{_v[0-9]+}}, options(att_syntax));
// REWRITES-NEXT: }
// REWRITES-NEXT: left = {{_v[0-9]+}};
// REWRITES-NEXT: right = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d _v9 anon_4 anon_struct_i32\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, left * {{_v[0-9]+}} + right) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
