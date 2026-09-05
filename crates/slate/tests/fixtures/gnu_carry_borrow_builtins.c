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
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe fn add_with_carry({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}} = {{arg[0-9]+}}.overflowing_add({{arg[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{arg[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe fn sub_with_borrow({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}} = {{arg[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} | {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}} as u32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut carry: u32 = 0;
// COMMON-LOWERING-NEXT:     let mut borrow: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { add_with_carry({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(carry)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = carry;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { add_with_carry({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(carry)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = carry;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 =
// COMMON-LOWERING-NEXT:         unsafe { sub_with_borrow({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(borrow)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = borrow;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 =
// COMMON-LOWERING-NEXT:         unsafe { sub_with_borrow({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(borrow)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = borrow;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u %u\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u %u\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u %u\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%u %u\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe fn add_with_carry({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}} = {{arg[0-9]+}}.overflowing_add({{arg[0-9]+}});
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{arg[0-9]+}});
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{arg[0-9]+}} = ({{__v[0-9]+}} | ({{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295)) as u32;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe fn sub_with_borrow({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}} = {{arg[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{arg[0-9]+}} = ({{__v[0-9]+}} | ({{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295)) as u32;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut carry: u32 = 0;
// COMMON-REWRITES-NEXT:     let mut borrow: u32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = 4294967295u32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%u %u\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe { add_with_carry({{__v[0-9]+}}, 1, 0, std::ptr::addr_of_mut!(carry)) },
// COMMON-REWRITES-NEXT:             carry,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%u %u\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe { add_with_carry(1, 1, 0, std::ptr::addr_of_mut!(carry)) },
// COMMON-REWRITES-NEXT:             carry,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%u %u\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe { sub_with_borrow(0, 1, 0, std::ptr::addr_of_mut!(borrow)) },
// COMMON-REWRITES-NEXT:             borrow,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%u %u\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe { sub_with_borrow(5, 3, 0, std::ptr::addr_of_mut!(borrow)) },
// COMMON-REWRITES-NEXT:             borrow,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
