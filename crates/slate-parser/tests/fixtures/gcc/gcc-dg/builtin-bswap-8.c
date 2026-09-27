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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 __uint32_t = u32;
// DEFAULT-NEXT:     type @type1 uint32_t = u32;
// DEFAULT-NEXT:     fn %20 @__builtin_bswap32(%19 <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @foo1(%3 a: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%20, not<u32>(call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo2(%5 a: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%20, and<u32>(call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%5)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(655360))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo3(%7 a: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%20, or<u32>(call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%7)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(655360))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @foo4(%9 a: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%20, xor<u32>(call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%9)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(655360))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foo5(%11 a: u32, %12 b: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%20, and<u32>(call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%11)), call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%12))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @foo6(%14 a: u32, %15 b: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%20, or<u32>(call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%14)), call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @foo7(%17 a: u32, %18 b: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u32, signature=fn(u32) -> u32>(%20, xor<u32>(call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%17)), call<u32, signature=fn(u32) -> u32>(%20, read<u32>(%18))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
