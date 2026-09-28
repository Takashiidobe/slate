/* { dg-do run } */
/* { dg-options "-O -fdump-tree-optimized -fwrapv" } */
/* { dg-require-effective-target int32 } */

#include <limits.h>
#include <stdbool.h>

bool __attribute__ ((noipa)) test1 (unsigned a, unsigned b)
{
    return (b == 0) | (a < b);
}

bool __attribute__ ((noipa)) test2 (int a, int b)
{
    return (b == INT_MIN) | (a < b);
}

bool __attribute__ ((noipa)) test3 (unsigned a, unsigned b)
{
    return (b != 0) & (a >= b);
}

bool __attribute__ ((noipa)) test4 (int a, int b)
{
    return (b != INT_MIN) & (a >= b);
}

int main()
{
    if (!test1 (1, 0) || !test1 (1, 2) || test1 (2, 1) ||
        !test2 (1, INT_MIN) || !test2 (1, 2) || test2 (2, 1) ||
        test3 (1, 0) || test3 (1, 2) || !test3 (2, 1) ||
        test4 (1, INT_MIN) || test4 (1, 2) || !test4 (2, 1)) {
        __builtin_abort();	
    }    	

    return 0;
}

/* { dg-final { scan-tree-dump-times "\\+ 4294967295;" 2 "optimized" } } */
/* { dg-final { scan-tree-dump-times "\\+ -1;" 2 "optimized" } } */

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fwrapv
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
// DEFAULT-NEXT:     fn %0 @test1(%1 a: u32, %2 b: u32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(or<i32>(from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), from_bool<i32, reason=promotion>(lt<u32>(read<u32>(%1), read<u32>(%2)))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test2(%4 a: i32, %5 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(or<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%5), sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1)))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%4), read<i32>(%5)))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test3(%7 a: u32, %8 b: u32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(from_bool<i32, reason=promotion>(ne<u32>(read<u32>(%8), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), from_bool<i32, reason=promotion>(ge<u32>(read<u32>(%7), read<u32>(%8)))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test4(%10 a: i32, %11 b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%11), sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1)))), from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%10), read<i32>(%11)))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if not<bool>(call<bool, signature=fn(u32, u32) -> bool>(%0, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, not<bool>(call<bool, signature=fn(u32, u32) -> bool>(%0, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, call<bool, signature=fn(u32, u32) -> bool>(%0, reinterpret<u32, reason=arg, fits=always>(const<i32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %16: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             write<bool>(%16, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%16, not<bool>(call<bool, signature=fn(i32, i32) -> bool>(%3, const<i32>(1), sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1)))));
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%17, not<bool>(call<bool, signature=fn(i32, i32) -> bool>(%3, const<i32>(1), const<i32>(2))));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, call<bool, signature=fn(i32, i32) -> bool>(%3, const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, call<bool, signature=fn(u32, u32) -> bool>(%6, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, call<bool, signature=fn(u32, u32) -> bool>(%6, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, not<bool>(call<bool, signature=fn(u32, u32) -> bool>(%6, reinterpret<u32, reason=arg, fits=always>(const<i32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         let %22: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             write<bool>(%22, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%22, call<bool, signature=fn(i32, i32) -> bool>(%9, const<i32>(1), sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:         let %23: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%22)
// DEFAULT-NEXT:             write<bool>(%23, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%23, call<bool, signature=fn(i32, i32) -> bool>(%9, const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:         let %24: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%23)
// DEFAULT-NEXT:             write<bool>(%24, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%24, not<bool>(call<bool, signature=fn(i32, i32) -> bool>(%9, const<i32>(2), const<i32>(1))));
// DEFAULT-NEXT:         if read<bool>(%24)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
