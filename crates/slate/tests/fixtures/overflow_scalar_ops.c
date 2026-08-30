#include <stdio.h>

int main(void) {
  int          ri  = 0;
  unsigned int ru  = 0;
  long long    rll = 0;

  int add_i = __builtin_add_overflow(2147483647, 1, &ri);
  printf("%d %d\n", add_i, ri);

  int sub_i = __builtin_sub_overflow((-2147483647 - 1), 1, &ri);
  printf("%d %d\n", sub_i, ri);

  int mul_i = __builtin_mul_overflow(1073741824, 2, &ri);
  printf("%d %d\n", mul_i, ri);

  int add_u = __builtin_add_overflow(4294967295u, 1u, &ru);
  printf("%d %u\n", add_u, ru);

  int mul_ll = __builtin_mul_overflow(3037000500LL, 3037000500LL, &rll);
  printf("%d %lld\n", mul_ll, rll);

  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut ri: i32 = 0;
// LOWERING-NEXT:     let mut ru: u32 = 0;
// LOWERING-NEXT:     let mut rll: i64 = 0;
// LOWERING-NEXT:     let mut add_i: i32 = 0;
// LOWERING-NEXT:     let mut sub_i: i32 = 0;
// LOWERING-NEXT:     let mut mul_i: i32 = 0;
// LOWERING-NEXT:     let mut add_u: i32 = 0;
// LOWERING-NEXT:     let mut mul_ll: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     ri = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     ru = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     rll = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2147483647;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     ri = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     add_i = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = add_i;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = ri;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -2147483647;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     ri = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     sub_i = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sub_i;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = ri;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1073741824;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.0 as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// LOWERING-NEXT:     ri = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     mul_i = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = mul_i;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = ri;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 4294967295u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.0 as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// LOWERING-NEXT:     ru = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     add_u = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = add_u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = ru;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3037000500i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3037000500i64;
// LOWERING-NEXT:     let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}}.0 as i64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -9223372036854775808 || {{_v[0-9]+}}.0 > 9223372036854775807);
// LOWERING-NEXT:     rll = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     mul_ll = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %lld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = mul_ll;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = rll;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut ri: i32 = 0;
// REWRITES-NEXT: let mut ru: u32 = 0;
// REWRITES-NEXT: let mut rll: i64 = 0;
// REWRITES-NEXT: let mut add_i: i32 = 0;
// REWRITES-NEXT: let mut sub_i: i32 = 0;
// REWRITES-NEXT: let mut mul_i: i32 = 0;
// REWRITES-NEXT: let mut add_u: i32 = 0;
// REWRITES-NEXT: let mut mul_ll: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: ri = 0;
// REWRITES-NEXT: ru = 0;
// REWRITES-NEXT: rll = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2147483647;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// REWRITES-NEXT: ri = {{_v[0-9]+}}.0 as i32;
// REWRITES-NEXT: add_i = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, add_i, ri) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -2147483647;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_sub({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// REWRITES-NEXT: ri = {{_v[0-9]+}}.0 as i32;
// REWRITES-NEXT: sub_i = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, sub_i, ri) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1073741824;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -2147483648 || {{_v[0-9]+}}.0 > 2147483647);
// REWRITES-NEXT: ri = {{_v[0-9]+}}.0 as i32;
// REWRITES-NEXT: mul_i = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, mul_i, ri) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 4294967295u32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_add({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || {{_v[0-9]+}}.0 > 4294967295;
// REWRITES-NEXT: ru = {{_v[0-9]+}}.0 as u32;
// REWRITES-NEXT: add_u = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, add_u, ru) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 3037000500i64;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 3037000500i64;
// REWRITES-NEXT: let {{_v[0-9]+}} = {{_v[0-9]+}}.overflowing_mul({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.1 || ({{_v[0-9]+}}.0 < -9223372036854775808 || {{_v[0-9]+}}.0 > 9223372036854775807);
// REWRITES-NEXT: rll = {{_v[0-9]+}}.0 as i64;
// REWRITES-NEXT: mul_ll = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %lld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, mul_ll, rll) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
