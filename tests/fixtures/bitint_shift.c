#include <stdio.h>

void abort(void);

// @lowering-fn-begin
// @rewrite-fn-begin
int shift_by_promoted_types(int seed) {
  int                i   = 4;
  unsigned           u   = 3;
  long               l   = 5;
  unsigned long long ull = 6;
  short              s   = 2;

  unsigned _BitInt(129) a   = 1;
  a                         = a << i;
  a                         = a >> u;
  a                       <<= l;
  a                       >>= s;
  a                         = a << ull;
  a                         = a + (unsigned _BitInt(129))seed;
  return (int)a;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int shift_across_limbs(int seed) {
  unsigned _BitInt(129) wide = 1;
  wide                       = wide << 128;
  wide                       = wide >> 127;
  return (int)wide + seed;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int shift_signed_arithmetic(int seed) {
  _BitInt(256) n = -1024;
  n              = n >> 3;
  n              = n << 2;
  return (int)n + seed;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
int shift_by_bitint(int seed) {
  _BitInt(256) amount = 4;
  _BitInt(256) v      = 3;
  v                   = v << amount;
  v                   = v >> amount;
  return (int)v + seed;
}
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  if (shift_by_promoted_types(0) != 1024)
    abort();
  if (shift_across_limbs(0) != 2)
    abort();
  if (shift_signed_arithmetic(0) != -512)
    abort();
  if (shift_by_bitint(0) != 3)
    abort();
  printf("%d %d %d %d\n", shift_by_promoted_types(1), shift_across_limbs(2),
         shift_signed_arithmetic(3), shift_by_bitint(4));
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn shift_by_promoted_types({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 3;
// LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 5;
// LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 6;
// LOWERING-DAG:     let {{__v[0-9]+}}: i16 = 2;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = bitint::BUint::<129, 3, 24>::from_decimal_str("1");
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} << {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} << {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} << {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = bitint::BUint::<129, 3, 24>::from_i128({{arg[0-9]+}} as i128);
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_u128() as i32;
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn shift_across_limbs({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = bitint::BUint::<129, 3, 24>::from_decimal_str("1");
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 128;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} << {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 127;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_u128() as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn shift_signed_arithmetic({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_decimal_str("-1024");
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} << {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: fn shift_by_bitint({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_decimal_str("4");
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_decimal_str("3");
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} << {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG:     return {{__v[0-9]+}};
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn shift_by_promoted_types({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 4;
// REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 3;
// REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 5;
// REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 6;
// REWRITES-DAG:     let {{__v[0-9]+}}: i16 = 2;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = bitint::BUint::<129, 3, 24>::from_decimal_str("1");
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} << {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} << {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} << {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = bitint::BUint::<129, 3, 24>::from_i128({{arg[0-9]+}} as i128);
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_u128() as i32;
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// REWRITES-DAG: fn shift_across_limbs({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = bitint::BUint::<129, 3, 24>::from_decimal_str("1");
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 128;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} << {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 127;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BUint<129, 3, 24> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_u128() as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// REWRITES-DAG: fn shift_signed_arithmetic({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_decimal_str("-1024");
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 3;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} << {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// REWRITES-DAG: fn shift_by_bitint({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_decimal_str("4");
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_decimal_str("3");
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} << {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = {{__v[0-9]+}} >> {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// REWRITES-DAG:     {{__v[0-9]+}}
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
