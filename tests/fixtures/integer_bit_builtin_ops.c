#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
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
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 305419896;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = -12345;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 5;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.reverse_bits();
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.swap_bytes();
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.trailing_zeros() as u32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 {
// COMMON-LOWERING-DAG:         0
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         ({{__v[0-9]+}}.trailing_zeros() as i32) + 1
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 {
// COMMON-LOWERING-DAG:         0
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         ({{__v[0-9]+}}.trailing_zeros() as i32) + 1
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.count_ones() as u32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}}.count_ones() as u32) & 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = (if {{__v[0-9]+}} < 0 { !{{__v[0-9]+}} } else { {{__v[0-9]+}} }.leading_zeros() as i32) - 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.rotate_left({{__v[0-9]+}} as u32);
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.rotate_right({{__v[0-9]+}} as u32);
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-DAG:         printf(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:             {{__v[0-9]+}},
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%u %u %d %d %d %d %d %d %d %u %u\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = b"%u %u %d %d %d %d %d %d %d %u %u\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 305419896;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -12345;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 5;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.reverse_bits();
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.swap_bytes();
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 {
// COMMON-REWRITES-DAG:         0
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         ({{__v[0-9]+}}.trailing_zeros() as i32) + 1
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = (0 as u32) as i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 {
// COMMON-REWRITES-DAG:         0
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         ({{__v[0-9]+}}.trailing_zeros() as i32) + 1
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = (if {{__v[0-9]+}} < 0 { !{{__v[0-9]+}} } else { {{__v[0-9]+}} }.leading_zeros() as i32) - 1;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.rotate_left({{__v[0-9]+}} as u32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.rotate_right({{__v[0-9]+}} as u32);
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         printf(
// COMMON-REWRITES-DAG:             c"%u %u %d %d %d %d %d %d %d %u %u\n".as_ptr(),
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             ({{__v[0-9]+}}.leading_zeros() as u32) as i32,
// COMMON-REWRITES-DAG:             ({{__v[0-9]+}}.trailing_zeros() as u32) as i32,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             ({{__v[0-9]+}}.count_ones() as u32) as i32,
// COMMON-REWRITES-DAG:             (({{__v[0-9]+}}.count_ones() as u32) & 1) as i32,
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:             {{__v[0-9]+}},
// COMMON-REWRITES-DAG:         )
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     std::process::exit(0 as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
