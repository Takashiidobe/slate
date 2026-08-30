#include <stdio.h>

int main(void) {
  unsigned int u  = 0x12345678u;
  unsigned int z  = 0u;
  int          s  = -12345;
  unsigned int sh = 5u;

  unsigned int rev            = __builtin_bitreverse32(u);
  unsigned int swapped        = __builtin_bswap32(u);
  int          leading        = __builtin_clz(u);
  int          trailing       = __builtin_ctz(u);
  int          first_set      = __builtin_ffs((int)u);
  int          zero_first     = __builtin_ffs((int)z);
  int          ones           = __builtin_popcount(u);
  int          odd            = __builtin_parity(u);
  int          redundant_sign = __builtin_clrsb(s);
  unsigned int left           = __builtin_rotateleft32(u, sh);
  unsigned int right          = __builtin_rotateright32(u, sh);

  printf("%u %u %d %d %d %d %d %d %d %u %u\n", rev, swapped, leading, trailing,
         first_set, zero_first, ones, odd, redundant_sign, left, right);
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
// LOWERING-NEXT:     let mut u: u32 = 0;
// LOWERING-NEXT:     let mut z: u32 = 0;
// LOWERING-NEXT:     let mut s: i32 = 0;
// LOWERING-NEXT:     let mut sh: u32 = 0;
// LOWERING-NEXT:     let mut rev: u32 = 0;
// LOWERING-NEXT:     let mut swapped: u32 = 0;
// LOWERING-NEXT:     let mut leading: i32 = 0;
// LOWERING-NEXT:     let mut trailing: i32 = 0;
// LOWERING-NEXT:     let mut first_set: i32 = 0;
// LOWERING-NEXT:     let mut zero_first: i32 = 0;
// LOWERING-NEXT:     let mut ones: i32 = 0;
// LOWERING-NEXT:     let mut odd: i32 = 0;
// LOWERING-NEXT:     let mut redundant_sign: i32 = 0;
// LOWERING-NEXT:     let mut left: u32 = 0;
// LOWERING-NEXT:     let mut right: u32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 305419896;
// LOWERING-NEXT:     u = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 0;
// LOWERING-NEXT:     z = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -12345;
// LOWERING-NEXT:     s = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 5;
// LOWERING-NEXT:     sh = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.reverse_bits();
// LOWERING-NEXT:     rev = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.swap_bytes();
// LOWERING-NEXT:     swapped = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.leading_zeros() as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     leading = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.trailing_zeros() as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     trailing = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} == 0 { 0 } else { ({{_v[0-9]+}}.trailing_zeros() as i32) + 1 };
// LOWERING-NEXT:     first_set = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = z;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} == 0 { 0 } else { ({{_v[0-9]+}}.trailing_zeros() as i32) + 1 };
// LOWERING-NEXT:     zero_first = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.count_ones() as u32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     ones = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.count_ones() & 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     odd = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = s;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = (if {{_v[0-9]+}} < 0 { !{{_v[0-9]+}} } else { {{_v[0-9]+}} }.leading_zeros() as i32) - 1;
// LOWERING-NEXT:     redundant_sign = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = sh;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.rotate_left({{_v[0-9]+}});
// LOWERING-NEXT:     left = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = u;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = sh;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.rotate_right({{_v[0-9]+}});
// LOWERING-NEXT:     right = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u %u %d %d %d %d %d %d %d %u %u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = rev;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = swapped;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = leading;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = trailing;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = first_set;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = zero_first;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = ones;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = odd;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = redundant_sign;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = left;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = right;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: let mut u: u32 = 0;
// REWRITES-NEXT: let mut z: u32 = 0;
// REWRITES-NEXT: let mut s: i32 = 0;
// REWRITES-NEXT: let mut sh: u32 = 0;
// REWRITES-NEXT: let mut rev: u32 = 0;
// REWRITES-NEXT: let mut swapped: u32 = 0;
// REWRITES-NEXT: let mut leading: i32 = 0;
// REWRITES-NEXT: let mut trailing: i32 = 0;
// REWRITES-NEXT: let mut first_set: i32 = 0;
// REWRITES-NEXT: let mut zero_first: i32 = 0;
// REWRITES-NEXT: let mut ones: i32 = 0;
// REWRITES-NEXT: let mut odd: i32 = 0;
// REWRITES-NEXT: let mut redundant_sign: i32 = 0;
// REWRITES-NEXT: let mut left: u32 = 0;
// REWRITES-NEXT: let mut right: u32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: u = 305419896;
// REWRITES-NEXT: z = 0;
// REWRITES-NEXT: s = -12345;
// REWRITES-NEXT: sh = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = u;
// REWRITES-NEXT: rev = {{_v[0-9]+}}.reverse_bits();
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = u;
// REWRITES-NEXT: swapped = {{_v[0-9]+}}.swap_bytes();
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = u;
// REWRITES-NEXT: leading = ({{_v[0-9]+}}.leading_zeros() as u32) as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = u;
// REWRITES-NEXT: trailing = ({{_v[0-9]+}}.trailing_zeros() as u32) as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = u as i32;
// REWRITES-NEXT: first_set = if {{_v[0-9]+}} == 0 { 0 } else { ({{_v[0-9]+}}.trailing_zeros() as i32) + 1 };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = z as i32;
// REWRITES-NEXT: zero_first = if {{_v[0-9]+}} == 0 { 0 } else { ({{_v[0-9]+}}.trailing_zeros() as i32) + 1 };
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = u;
// REWRITES-NEXT: ones = ({{_v[0-9]+}}.count_ones() as u32) as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = u;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.count_ones() & 1;
// REWRITES-NEXT: odd = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = s;
// REWRITES-NEXT: redundant_sign = (if {{_v[0-9]+}} < 0 { !{{_v[0-9]+}} } else { {{_v[0-9]+}} }.leading_zeros() as i32) - 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = u;
// REWRITES-NEXT: left = {{_v[0-9]+}}.rotate_left(sh);
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = u;
// REWRITES-NEXT: right = {{_v[0-9]+}}.rotate_right(sh);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u %u %d %d %d %d %d %d %d %u %u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, rev, swapped, leading, trailing, first_set, zero_first, ones, odd, redundant_sign, left, right) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
