/* PR tree-optimization/112390 */
/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

#include <limits.h>

int f32_bitand(unsigned int a)
{
  return ((int)a >= 0) & ((a & (unsigned)INT_MAX) != 0);
}

int f32_truthand(unsigned int a)
{
  return ((int)a >= 0) && ((a & (unsigned)INT_MAX) != 0);
}

int f32_bitior(unsigned int a)
{
  return ((int)a < 0) | ((a & (unsigned)INT_MAX) == 0);
}

int f32_truthor(unsigned int a)
{
  return ((int)a < 0) || ((a & (unsigned)INT_MAX) == 0);
}

int f32s_bitand(int a)
{
  return (a >= 0) & ((a & INT_MAX) != 0);
}

int f32s_truthand(int a)
{
  return (a >= 0) && ((a & INT_MAX) != 0);
}

int f32s_bitior(int a)
{
  return (a < 0) | ((a & INT_MAX) == 0);
}

int f32s_truthor(int a)
{
  return (a < 0) || ((a & INT_MAX) == 0);
}

int f64_bitand(unsigned long long a)
{
  return ((long long)a >= 0) & ((a & (unsigned long long)LLONG_MAX) != 0);
}

int f64_truthand(unsigned long long a)
{
  return ((long long)a >= 0) && ((a & (unsigned long long)LLONG_MAX) != 0);
}

int f64_bitior(unsigned long long a)
{
  return ((long long)a < 0) | ((a & (unsigned long long)LLONG_MAX) == 0);
}

int f64_truthor(unsigned long long a)
{
  return ((long long)a < 0) || ((a & (unsigned long long)LLONG_MAX) == 0);
}

int f64s_bitand(long long a)
{
  return (a >= 0) & ((a & LLONG_MAX) != 0);
}

int f64s_truthand(long long a)
{
  return (a >= 0) && ((a & LLONG_MAX) != 0);
}

int f64s_bitior(long long a)
{
  return (a < 0) | ((a & LLONG_MAX) == 0);
}

int f64s_truthor(long long a)
{
  return (a < 0) || ((a & LLONG_MAX) == 0);
}

/* { dg-final { scan-tree-dump-times ">= 0" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-times "< 0" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-times "BIT_AND_EXPR" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-not "bit_and" "optimized" } } */
/* { dg-final { scan-tree-dump-not "bit_ior" "optimized" } } */
/* { dg-final { scan-tree-dump-times "> 0" 8 "optimized" } } */
/* { dg-final { scan-tree-dump-times "<= 0" 8 "optimized" } } */

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
// DEFAULT-NEXT:     fn %0 @f32_bitand(%1 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(from_bool<i32, reason=promotion>(ge<i32>(reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%1)), const<i32>(0))), from_bool<i32, reason=promotion>(ne<u32>(and<u32>(read<u32>(%1), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @f32_truthand(%3 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(ge<i32>(reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%3)), const<i32>(0)), ne<u32>(and<u32>(read<u32>(%3), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f32_bitior(%5 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(from_bool<i32, reason=promotion>(lt<i32>(reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%5)), const<i32>(0))), from_bool<i32, reason=promotion>(eq<u32>(and<u32>(read<u32>(%5), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f32_truthor(%7 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(lt<i32>(reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(%7)), const<i32>(0)), eq<u32>(and<u32>(read<u32>(%7), reinterpret<u32, reason=explicit, fits=always>(const<i32>(2147483647))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @f32s_bitand(%9 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%9), const<i32>(0))), from_bool<i32, reason=promotion>(ne<i32>(and<i32>(read<i32>(%9), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f32s_truthand(%11 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(ge<i32>(read<i32>(%11), const<i32>(0)), ne<i32>(and<i32>(read<i32>(%11), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f32s_bitior(%13 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%13), const<i32>(0))), from_bool<i32, reason=promotion>(eq<i32>(and<i32>(read<i32>(%13), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f32s_truthor(%15 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(lt<i32>(read<i32>(%15), const<i32>(0)), eq<i32>(and<i32>(read<i32>(%15), const<i32>(2147483647)), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f64_bitand(%17 a: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(from_bool<i32, reason=promotion>(ge<i64>(reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%17)), widen<i64, reason=usual_arith>(const<i32>(0)))), from_bool<i32, reason=promotion>(ne<u64>(and<u64>(read<u64>(%17), reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f64_truthand(%19 a: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(ge<i64>(reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%19)), widen<i64, reason=usual_arith>(const<i32>(0))), ne<u64>(and<u64>(read<u64>(%19), reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @f64_bitior(%21 a: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(from_bool<i32, reason=promotion>(lt<i64>(reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%21)), widen<i64, reason=usual_arith>(const<i32>(0)))), from_bool<i32, reason=promotion>(eq<u64>(and<u64>(read<u64>(%21), reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @f64_truthor(%23 a: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(lt<i64>(reinterpret<i64, reason=explicit, fits=unknown>(read<u64>(%23)), widen<i64, reason=usual_arith>(const<i32>(0))), eq<u64>(and<u64>(read<u64>(%23), reinterpret<u64, reason=explicit, fits=always>(const<i64>(9223372036854775807))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f64s_bitand(%25 a: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return and<i32>(from_bool<i32, reason=promotion>(ge<i64>(read<i64>(%25), widen<i64, reason=usual_arith>(const<i32>(0)))), from_bool<i32, reason=promotion>(ne<i64>(and<i64>(read<i64>(%25), const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @f64s_truthand(%27 a: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(ge<i64>(read<i64>(%27), widen<i64, reason=usual_arith>(const<i32>(0))), ne<i64>(and<i64>(read<i64>(%27), const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @f64s_bitior(%29 a: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return or<i32>(from_bool<i32, reason=promotion>(lt<i64>(read<i64>(%29), widen<i64, reason=usual_arith>(const<i32>(0)))), from_bool<i32, reason=promotion>(eq<i64>(and<i64>(read<i64>(%29), const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @f64s_truthor(%31 a: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(logical_or<bool>(lt<i64>(read<i64>(%31), widen<i64, reason=usual_arith>(const<i32>(0))), eq<i64>(and<i64>(read<i64>(%31), const<i64>(9223372036854775807)), widen<i64, reason=usual_arith>(const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
