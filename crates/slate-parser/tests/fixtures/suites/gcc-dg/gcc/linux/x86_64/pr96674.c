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
// DEFAULT-NEXT:     fn %[[VALUE_test1:[0-9]+]] @test1(%[[VALUE_a:[0-9]+]] a: u32, %[[VALUE_b:[0-9]+]] b: u32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(or<i32>(from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%[[VALUE_b]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), from_bool<i32, reason=promotion>(lt<u32>(read<u32>(%[[VALUE_a]]), read<u32>(%[[VALUE_b]])))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test2:[0-9]+]] @test2(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(or<i32>(from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_b_2]]), sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1)))), from_bool<i32, reason=promotion>(lt<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]])))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test3:[0-9]+]] @test3(%[[VALUE_a_3:[0-9]+]] a: u32, %[[VALUE_b_3:[0-9]+]] b: u32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(from_bool<i32, reason=promotion>(ne<u32>(read<u32>(%[[VALUE_b_3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))), from_bool<i32, reason=promotion>(ge<u32>(read<u32>(%[[VALUE_a_3]]), read<u32>(%[[VALUE_b_3]])))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test4:[0-9]+]] @test4(%[[VALUE_a_4:[0-9]+]] a: i32, %[[VALUE_b_4:[0-9]+]] b: i32) -> bool [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return ne<i32, reason=return>(and<i32>(from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%[[VALUE_b_4]]), sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1)))), from_bool<i32, reason=promotion>(ge<i32>(read<i32>(%[[VALUE_a_4]]), read<i32>(%[[VALUE_b_4]])))), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if not<bool>(call<bool, signature=fn(u32, u32) -> bool>(%[[VALUE_test1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], not<bool>(call<bool, signature=fn(u32, u32) -> bool>(%[[VALUE_test1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], call<bool, signature=fn(u32, u32) -> bool>(%[[VALUE_test1]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], not<bool>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_test2]], const<i32>(1), sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], not<bool>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_test2]], const<i32>(1), const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_test2]], const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], call<bool, signature=fn(u32, u32) -> bool>(%[[VALUE_test3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], call<bool, signature=fn(u32, u32) -> bool>(%[[VALUE_test3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(1)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], not<bool>(call<bool, signature=fn(u32, u32) -> bool>(%[[VALUE_test3]], reinterpret<u32, reason=arg, fits=always>(const<i32>(2)), reinterpret<u32, reason=arg, fits=always>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_test4]], const<i32>(1), sub<i32, overflow=wrap>(neg<i32, overflow=wrap>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_test4]], const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], not<bool>(call<bool, signature=fn(i32, i32) -> bool>(%[[VALUE_test4]], const<i32>(2), const<i32>(1))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
