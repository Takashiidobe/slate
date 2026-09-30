/* { dg-do compile { target arm*-*-* alpha*-*-* i?86-*-* powerpc*-*-* rs6000-*-* x86_64-*-* s390*-*-* } } */
/* { dg-require-effective-target stdint_types } */
/* { dg-options "-O2 -fdump-rtl-combine" } */

#include <stdint.h>

#define BS(X) __builtin_bswap32(X)

uint32_t foo1 (uint32_t a)
{
  return BS (~ BS (a));
}

uint32_t foo2 (uint32_t a)
{
  return BS (BS (a) & 0xA0000);
}

uint32_t foo3 (uint32_t a)
{
  return BS (BS (a) | 0xA0000);
}

uint32_t foo4 (uint32_t a)
{
  return BS (BS (a) ^ 0xA0000);
}

uint32_t foo5 (uint32_t a, uint32_t b)
{
  return BS (BS (a) & BS (b));
}

uint32_t foo6 (uint32_t a, uint32_t b)
{
  return BS (BS (a) | BS (b));
}

uint32_t foo7 (uint32_t a, uint32_t b)
{
  return BS (BS (a) ^ BS (b));
}

/* { dg-final { scan-rtl-dump-not "bswapsi" "combine" } } */

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32;
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap32:[0-9]+]] @__builtin_bswap32(%[[VALUE0:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1(%[[VALUE_a:[0-9]+]] a: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], not<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_a]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_a_2:[0-9]+]] a: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], and<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_a_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(655360))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3(%[[VALUE_a_3:[0-9]+]] a: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], or<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_a_3]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(655360))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4(%[[VALUE_a_4:[0-9]+]] a: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], xor<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_a_4]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(655360))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo5:[0-9]+]] @foo5(%[[VALUE_a_5:[0-9]+]] a: u32, %[[VALUE_b:[0-9]+]] b: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], and<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_a_5]])), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_b]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo6:[0-9]+]] @foo6(%[[VALUE_a_6:[0-9]+]] a: u32, %[[VALUE_b_2:[0-9]+]] b: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], or<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_a_6]])), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_b_2]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo7:[0-9]+]] @foo7(%[[VALUE_a_7:[0-9]+]] a: u32, %[[VALUE_b_3:[0-9]+]] b: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], xor<u32>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_a_7]])), call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], read<u32>(%[[VALUE_b_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
