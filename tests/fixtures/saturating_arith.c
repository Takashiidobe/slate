#include <limits.h>
#include <stdio.h>

typedef unsigned __int128 U128;
typedef _BitInt(200) S200;
typedef unsigned _BitInt(200) U200;
typedef int v4si __attribute__((vector_size(16)));

int main(void) {
  int add_i = __builtin_elementwise_add_sat(INT_MAX - 5, 10);
  printf("%d\n", add_i);

  int add_i_no_sat = __builtin_elementwise_add_sat(2, 3);
  printf("%d\n", add_i_no_sat);

  short sub_s = __builtin_elementwise_sub_sat((short)(SHRT_MIN + 5), (short)10);
  printf("%d\n", sub_s);

  unsigned sub_u = __builtin_elementwise_sub_sat(5u, 10u);
  printf("%u\n", sub_u);

  U128 u128a = (U128)0 - 1;
  U128 add_u128 = __builtin_elementwise_add_sat(u128a, (U128)5);
  printf("%d\n", add_u128 == (U128)0 - 1);

  S200 s200a = -1;
  S200 add_s200 = __builtin_elementwise_add_sat(s200a, (S200)5);
  printf("%d\n", (int)add_s200);

  U200 u200a = 3;
  U200 sub_u200 = __builtin_elementwise_sub_sat(u200a, (U200)10);
  printf("%d\n", sub_u200 == 0);

  v4si va = {2147483647, 1, -2147483647 - 1, 0};
  v4si vb = {10, 1, -10, 0};
  v4si vr = __builtin_elementwise_add_sat(va, vb);
  printf("%d %d %d %d\n", vr[0], vr[1], vr[2], vr[3]);

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
// LOWERING-NEXT:     let mut add_i: i32 = 0;
// LOWERING-NEXT:     let mut add_i_no_sat: i32 = 0;
// LOWERING-NEXT:     let mut sub_s: i16 = 0;
// LOWERING-NEXT:     let mut sub_u: u32 = 0;
// LOWERING-NEXT:     let mut u128a: u128 = 0;
// LOWERING-NEXT:     let mut add_u128: u128 = 0;
// LOWERING-NEXT:     let mut s200a: aligned::Aligned<aligned::A8, bitint::BInt<200, 4, 32>> = aligned::Aligned(bitint::BInt::<200, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut add_s200: aligned::Aligned<aligned::A8, bitint::BInt<200, 4, 32>> = aligned::Aligned(bitint::BInt::<200, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut u200a: aligned::Aligned<aligned::A8, bitint::BUint<200, 4, 32>> = aligned::Aligned(bitint::BUint::<200, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut sub_u200: aligned::Aligned<aligned::A8, bitint::BUint<200, 4, 32>> = aligned::Aligned(bitint::BUint::<200, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut va: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut vb: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut vr: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = i32::MAX as i32;
// LOWERING-NEXT:     add_i = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = add_i;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     add_i_no_sat = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = add_i_no_sat;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = -32768;
// LOWERING-NEXT:     sub_s = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i16 = sub_s;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     sub_u = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = sub_u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     u128a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = u128a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}}.saturating_add({{_v[0-9]+}});
// LOWERING-NEXT:     add_u128 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = add_u128;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u128 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = bitint::BInt::<200, 4, 32>::from_decimal_str("-1");
// LOWERING-NEXT:     *s200a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = *s200a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = bitint::BInt::<200, 4, 32>::from_decimal_str("5");
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = {{_v[0-9]+}}.saturating_add({{_v[0-9]+}});
// LOWERING-NEXT:     *add_s200 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = *add_s200;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_i128() as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("3");
// LOWERING-NEXT:     *u200a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = *u200a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("10");
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = {{_v[0-9]+}}.saturating_sub({{_v[0-9]+}});
// LOWERING-NEXT:     *sub_u200 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = *sub_u200;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("0");
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [2147483647, 1, -2147483648, 0];
// LOWERING-NEXT:     *va = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [10, 1, -10, 0];
// LOWERING-NEXT:     *vb = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = *va;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = *vb;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [{{_v[0-9]+}}[0usize].saturating_add({{_v[0-9]+}}[0usize]), {{_v[0-9]+}}[1usize].saturating_add({{_v[0-9]+}}[1usize]), {{_v[0-9]+}}[2usize].saturating_add({{_v[0-9]+}}[2usize]), {{_v[0-9]+}}[3usize].saturating_add({{_v[0-9]+}}[3usize])];
// LOWERING-NEXT:     *vr = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = *vr;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = *vr;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = *vr;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = *vr;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: let mut add_i: i32 = 0;
// REWRITES-NEXT: let mut add_i_no_sat: i32 = 0;
// REWRITES-NEXT: let mut sub_s: i16 = 0;
// REWRITES-NEXT: let mut sub_u: u32 = 0;
// REWRITES-NEXT: let mut u128a: u128 = 0;
// REWRITES-NEXT: let mut add_u128: u128 = 0;
// REWRITES-NEXT: let mut s200a: aligned::Aligned<aligned::A8, bitint::BInt<200, 4, 32>> = aligned::Aligned(bitint::BInt::<200, 4, 32>::ZERO);
// REWRITES-NEXT: let mut add_s200: aligned::Aligned<aligned::A8, bitint::BInt<200, 4, 32>> = aligned::Aligned(bitint::BInt::<200, 4, 32>::ZERO);
// REWRITES-NEXT: let mut u200a: aligned::Aligned<aligned::A8, bitint::BUint<200, 4, 32>> = aligned::Aligned(bitint::BUint::<200, 4, 32>::ZERO);
// REWRITES-NEXT: let mut sub_u200: aligned::Aligned<aligned::A8, bitint::BUint<200, 4, 32>> = aligned::Aligned(bitint::BUint::<200, 4, 32>::ZERO);
// REWRITES-NEXT: let mut va: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut vb: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut vr: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: add_i = i32::MAX as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, add_i) };
// REWRITES-NEXT: add_i_no_sat = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, add_i_no_sat) };
// REWRITES-NEXT: sub_s = -32768;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, sub_s as i32) };
// REWRITES-NEXT: sub_u = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, sub_u) };
// REWRITES-NEXT: let {{_v[0-9]+}}: u128 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u128 = 1;
// REWRITES-NEXT: u128a = {{_v[0-9]+}} - {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: u128 = u128a;
// REWRITES-NEXT: let {{_v[0-9]+}}: u128 = 5;
// REWRITES-NEXT: add_u128 = {{_v[0-9]+}}.saturating_add({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u128 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u128 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, (add_u128 == {{_v[0-9]+}} - {{_v[0-9]+}}) as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = bitint::BInt::<200, 4, 32>::from_decimal_str("-1");
// REWRITES-NEXT: *s200a = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = *s200a;
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = bitint::BInt::<200, 4, 32>::from_decimal_str("5");
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = {{_v[0-9]+}}.saturating_add({{_v[0-9]+}});
// REWRITES-NEXT: *add_s200 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BInt<200, 4, 32> = *add_s200;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}}.to_i128() as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("3");
// REWRITES-NEXT: *u200a = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = *u200a;
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("10");
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = {{_v[0-9]+}}.saturating_sub({{_v[0-9]+}});
// REWRITES-NEXT: *sub_u200 = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = *sub_u200;
// REWRITES-NEXT: let {{_v[0-9]+}}: bitint::BUint<200, 4, 32> = bitint::BUint::<200, 4, 32>::from_decimal_str("0");
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, ({{_v[0-9]+}} == {{_v[0-9]+}}) as i32) };
// REWRITES-NEXT: *va = [2147483647, 1, -2147483648, 0];
// REWRITES-NEXT: *vb = [10, 1, -10, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = *va;
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = *vb;
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = [{{_v[0-9]+}}[0usize].saturating_add({{_v[0-9]+}}[0usize]), {{_v[0-9]+}}[1usize].saturating_add({{_v[0-9]+}}[1usize]), {{_v[0-9]+}}[2usize].saturating_add({{_v[0-9]+}}[2usize]), {{_v[0-9]+}}[3usize].saturating_add({{_v[0-9]+}}[3usize])];
// REWRITES-NEXT: *vr = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, (*vr)[({{_v[0-9]+}} as usize)], (*vr)[({{_v[0-9]+}} as usize)], (*vr)[({{_v[0-9]+}} as usize)], (*vr)[({{_v[0-9]+}} as usize)]) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
