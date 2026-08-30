#include <stdio.h>

int main(void) {
  unsigned int u  = 0x12345678u;
  unsigned int z  = 0u;
  int          s  = -12345;
  unsigned int sh = 5u;

  // @lowering-begin
  // @rewrite-begin
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
  // @rewrite-end
  // @lowering-end

  printf("%u %u %d %d %d %d %d %d %d %u %u\n", rev, swapped, leading, trailing,
         first_set, zero_first, ones, odd, redundant_sign, left, right);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = u;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.reverse_bits();
// LOWERING-DAG: rev = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = u;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.swap_bytes();
// LOWERING-DAG: swapped = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = u;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.leading_zeros() as u32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: leading = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = u;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.trailing_zeros() as u32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: trailing = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = u;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} == 0 { 0 } else { ({{_v[0-9]+}}.trailing_zeros() as i32) + 1 };
// LOWERING-DAG: first_set = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = z;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} == 0 { 0 } else { ({{_v[0-9]+}}.trailing_zeros() as i32) + 1 };
// LOWERING-DAG: zero_first = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = u;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.count_ones() as u32;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: ones = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = u;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}}.count_ones() as u32) & 1;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-DAG: odd = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = s;
// LOWERING-DAG: let {{_v[0-9]+}}: i32 = (if {{_v[0-9]+}} < 0 { !{{_v[0-9]+}} } else { {{_v[0-9]+}} }.leading_zeros() as i32) - 1;
// LOWERING-DAG: redundant_sign = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = u;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = sh;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.rotate_left({{_v[0-9]+}} as u32);
// LOWERING-DAG: left = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = u;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = sh;
// LOWERING-DAG: let {{_v[0-9]+}}: u32 = {{_v[0-9]+}}.rotate_right({{_v[0-9]+}} as u32);
// LOWERING-DAG: right = {{_v[0-9]+}};
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = u;
// REWRITES-DAG: rev = {{_v[0-9]+}}.reverse_bits();
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = u;
// REWRITES-DAG: swapped = {{_v[0-9]+}}.swap_bytes();
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = u;
// REWRITES-DAG: leading = ({{_v[0-9]+}}.leading_zeros() as u32) as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = u;
// REWRITES-DAG: trailing = ({{_v[0-9]+}}.trailing_zeros() as u32) as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = u as i32;
// REWRITES-DAG: first_set = if {{_v[0-9]+}} == 0 { 0 } else { ({{_v[0-9]+}}.trailing_zeros() as i32) + 1 };
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = z as i32;
// REWRITES-DAG: zero_first = if {{_v[0-9]+}} == 0 { 0 } else { ({{_v[0-9]+}}.trailing_zeros() as i32) + 1 };
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = u;
// REWRITES-DAG: ones = ({{_v[0-9]+}}.count_ones() as u32) as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = u;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = ({{_v[0-9]+}}.count_ones() as u32) & 1;
// REWRITES-DAG: odd = {{_v[0-9]+}} as i32;
// REWRITES-DAG: let {{_v[0-9]+}}: i32 = s;
// REWRITES-DAG: redundant_sign = (if {{_v[0-9]+}} < 0 { !{{_v[0-9]+}} } else { {{_v[0-9]+}} }.leading_zeros() as i32) - 1;
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = u;
// REWRITES-DAG: left = {{_v[0-9]+}}.rotate_left(sh as u32);
// REWRITES-DAG: let {{_v[0-9]+}}: u32 = u;
// REWRITES-DAG: right = {{_v[0-9]+}}.rotate_right(sh as u32);
// SLATE-FILECHECK-END rewrites
