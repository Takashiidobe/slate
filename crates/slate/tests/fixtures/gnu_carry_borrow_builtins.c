#include <stdio.h>

unsigned add_with_carry(unsigned a, unsigned b, unsigned carry_in,
                        unsigned *carry_out) {
  return __builtin_addc(a, b, carry_in, carry_out);
}

unsigned sub_with_borrow(unsigned a, unsigned b, unsigned borrow_in,
                         unsigned *borrow_out) {
  return __builtin_subc(a, b, borrow_in, borrow_out);
}

int main(void) {
  unsigned carry, borrow;

  unsigned sum = add_with_carry(0xFFFFFFFFu, 1u, 0u, &carry);
  printf("%u %u\n", sum, carry);

  unsigned sum2 = add_with_carry(1u, 1u, 0u, &carry);
  printf("%u %u\n", sum2, carry);

  unsigned diff = sub_with_borrow(0u, 1u, 0u, &borrow);
  printf("%u %u\n", diff, borrow);

  unsigned diff2 = sub_with_borrow(5u, 3u, 0u, &borrow);
  printf("%u %u\n", diff2, borrow);

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
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn add_with_carry({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{arg[0-9]+}}.overflowing_add({{arg[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{arg[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn sub_with_borrow({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{arg[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut carry: u32 = 0;
// LOWERING-NEXT:     let mut borrow: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { add_with_carry({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(carry)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = carry;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { add_with_carry({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(carry)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = carry;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { sub_with_borrow({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(borrow)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = borrow;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { sub_with_borrow({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(borrow)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = borrow;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn add_with_carry({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// REWRITES-NEXT:     let {{_v[0-9]+}} = {{arg[0-9]+}}.overflowing_add({{arg[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// REWRITES-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{arg[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} | ({{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}} as u32;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn sub_with_borrow({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// REWRITES-NEXT:     let {{_v[0-9]+}} = {{arg[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// REWRITES-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} | ({{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}} as u32;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut carry: u32 = 0;
// REWRITES-NEXT:     let mut borrow: u32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u32 = 4294967295u32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%u %u\n".as_ptr(),
// REWRITES-NEXT:             unsafe { add_with_carry({{_v[0-9]+}}, 1, 0, std::ptr::addr_of_mut!(carry)) },
// REWRITES-NEXT:             carry,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%u %u\n".as_ptr(),
// REWRITES-NEXT:             unsafe { add_with_carry(1, 1, 0, std::ptr::addr_of_mut!(carry)) },
// REWRITES-NEXT:             carry,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%u %u\n".as_ptr(),
// REWRITES-NEXT:             unsafe { sub_with_borrow(0, 1, 0, std::ptr::addr_of_mut!(borrow)) },
// REWRITES-NEXT:             borrow,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%u %u\n".as_ptr(),
// REWRITES-NEXT:             unsafe { sub_with_borrow(5, 3, 0, std::ptr::addr_of_mut!(borrow)) },
// REWRITES-NEXT:             borrow,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
