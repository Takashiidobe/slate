/* { dg-do compile { target arm*-*-* alpha*-*-* ia64*-*-* i?86-*-* x86_64-*-* s390x-*-* powerpc*-*-* rs6000-*-* } } */
/* { dg-require-effective-target stdint_types } */
/* { dg-require-effective-target lp64 } */
/* { dg-options "-O2 -fdump-rtl-combine" } */

/* The test is similiar to builtin-bswap-7.c but returns 1/2 instead
   of 0/1 to prevent GCC from calculating the return value with
   arithmetic instead of a comparison.  This requires the optimization
   level to be bumped up to -O2 at least for x86_64.  */

#include <stdint.h>

#define BS(X) __builtin_bswap64(X)

int foo1 (uint64_t a)
{
  if (BS (a) == 0xA00000000)
    return 1;
  return 2;
}

int foo2 (uint64_t a)
{
  if (BS (a) != 0xA00000000)
    return 1;
  return 2;
}

int foo3 (uint64_t a, uint64_t b)
{
  if (BS (a) == BS (b))
    return 1;
  return 2;
}

int foo4 (uint64_t a, uint64_t b)
{
  if (BS (a) != BS (b))
    return 1;
  return 2;
}

/* { dg-final { scan-rtl-dump-not "bswapdi" "combine" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_uint64_t:[0-9]+]] uint64_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap64:[0-9]+]] @__builtin_bswap64(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1(%[[VALUE_a:[0-9]+]] a: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_a]])), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(42949672960)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_a_2:[0-9]+]] a: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_a_2]])), reinterpret<u64, reason=usual_arith, fits=always>(const<i64>(42949672960)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3(%[[VALUE_a_3:[0-9]+]] a: u64, %[[VALUE_b:[0-9]+]] b: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_a_3]])), call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_b]])))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4(%[[VALUE_a_4:[0-9]+]] a: u64, %[[VALUE_b_2:[0-9]+]] b: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_a_4]])), call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], read<u64>(%[[VALUE_b_2]])))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
