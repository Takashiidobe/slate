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
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: let leading: i32 = u.leading_zeros() as i32;
// REWRITES-DAG: let trailing: i32 = u.trailing_zeros() as i32;
// REWRITES-DAG: let first_set: i32 = if (u as i32) == 0 { 0 } else { ((u as i32).trailing_zeros() as i32) + 1 };
// REWRITES-DAG: let ones: i32 = u.count_ones() as i32;
// REWRITES-DAG: let odd: i32 = (u.count_ones() & 1) as i32;
// REWRITES-DAG: let redundant_sign: i32 = (if s < 0 { !s } else { s }.leading_zeros() as i32) - 1;
// REWRITES-NOT: let _v
// REWRITES: {{^}}}
