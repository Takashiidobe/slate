/* PR middle-end/24003 */
/* Contributed by Eric Botcazou <ebotcazou@adacore.com> */

/* { dg-do run } */
/* { dg-options "-std=c99 -O -fno-inline" } */
/* { dg-options "-std=c99 -O -fno-inline -mtune=i686" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

#include <limits.h>

typedef unsigned long uns32_t;
typedef unsigned long long uns64_t;

extern void abort(void);

uns32_t lo (uns64_t p)
{
  return (uns32_t)p;
}

uns64_t concat (uns32_t p1, uns32_t p2)
{
#if LLONG_MAX > 2147483647L
  return ((uns64_t)p1 << 32) | p2;
#else
  return 0;
#endif
}

uns64_t lshift32 (uns64_t p1, uns32_t p2)
{
  return concat (lo (p1), p2);
}

int main(void)
{
#if LLONG_MAX > 2147483647L
  if (lshift32 (0xFFFFFFFF12345678ULL, 0x90ABCDEFUL) != 0x1234567890ABCDEFULL)
    abort ();
#endif

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     type @type0 uns32_t = u64;
// DEFAULT-NEXT:     type @type1 uns64_t = u64;
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @lo(%4 p: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u64>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @concat(%6 p1: u64, %7 p2: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(read<u64>(%6), const<i32>(32)), read<u64>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @lshift32(%9 p1: u64, %10 p2: u64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<u64, signature=fn(u64, u64) -> u64>(%5, call<u64, signature=fn(u64) -> u64>(%3, read<u64>(%9)), read<u64>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(u64, u64) -> u64>(%8, const<u64>(18446744069720004216), const<u64>(2427178479)), const<u64>(1311768467294899695))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
