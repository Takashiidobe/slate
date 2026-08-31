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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { add_with_carry({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(carry)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = carry;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { sub_with_borrow({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(borrow)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = borrow;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = unsafe { sub_with_borrow({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(borrow)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = borrow;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn add_with_carry({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// REWRITES-NEXT: let {{_v[0-9]+}} = {{arg[0-9]+}}.overflowing_add({{arg[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{arg[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = ({{_v[0-9]+}} | {{_v[0-9]+}}) as u32;
// REWRITES-NEXT: }
// REWRITES-NEXT: return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn sub_with_borrow({{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut u32) -> u32 {
// REWRITES-NEXT: let {{_v[0-9]+}} = {{arg[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub({{arg[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = ({{_v[0-9]+}} | {{_v[0-9]+}}) as u32;
// REWRITES-NEXT: }
// REWRITES-NEXT: return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut carry: u32 = 0;
// REWRITES-NEXT: let mut borrow: u32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 4294967295u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { add_with_carry({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(carry)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, carry) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { add_with_carry({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(carry)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, carry) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { sub_with_borrow({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(borrow)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, borrow) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = unsafe { sub_with_borrow({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(borrow)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, borrow) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
