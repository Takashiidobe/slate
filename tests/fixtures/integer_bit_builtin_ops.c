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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 305419896;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = -12345;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 5;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.reverse_bits();
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.swap_bytes();
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.leading_zeros() as u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.trailing_zeros() as u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 {
// LOWERING-DAG:         0
// LOWERING-DAG:     } else {
// LOWERING-DAG:         ({{__v[0-9]+}}.trailing_zeros() as i32) + 1
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 {
// LOWERING-DAG:         0
// LOWERING-DAG:     } else {
// LOWERING-DAG:         ({{__v[0-9]+}}.trailing_zeros() as i32) + 1
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.count_ones() as u32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = ({{__v[0-9]+}}.count_ones() as u32) & 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = (if {{__v[0-9]+}} < 0 { !{{__v[0-9]+}} } else { {{__v[0-9]+}} }.leading_zeros() as i32) - 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.rotate_left({{__v[0-9]+}} as u32);
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.rotate_right({{__v[0-9]+}} as u32);
// LOWERING-DAG:     let {{__v[0-9]+}}: *mut i8 = b"%u %u %d %d %d %d %d %d %d %u %u\n\0".as_ptr() as *mut i8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-DAG:         printf(
// LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:             {{__v[0-9]+}},
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 305419896;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -12345;
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 5;
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.reverse_bits();
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.swap_bytes();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 {
// REWRITES-DAG:         0
// REWRITES-DAG:     } else {
// REWRITES-DAG:         ({{__v[0-9]+}}.trailing_zeros() as i32) + 1
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = (0 as u32) as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} == 0 {
// REWRITES-DAG:         0
// REWRITES-DAG:     } else {
// REWRITES-DAG:         ({{__v[0-9]+}}.trailing_zeros() as i32) + 1
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = (if {{__v[0-9]+}} < 0 { !{{__v[0-9]+}} } else { {{__v[0-9]+}} }.leading_zeros() as i32) - 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.rotate_left({{__v[0-9]+}} as u32);
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.rotate_right({{__v[0-9]+}} as u32);
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             c"%u %u %d %d %d %d %d %d %d %u %u\n".as_ptr(),
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             ({{__v[0-9]+}}.leading_zeros() as u32) as i32,
// REWRITES-DAG:             ({{__v[0-9]+}}.trailing_zeros() as u32) as i32,
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             ({{__v[0-9]+}}.count_ones() as u32) as i32,
// REWRITES-DAG:             (({{__v[0-9]+}}.count_ones() as u32) & 1) as i32,
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:             {{__v[0-9]+}},
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
