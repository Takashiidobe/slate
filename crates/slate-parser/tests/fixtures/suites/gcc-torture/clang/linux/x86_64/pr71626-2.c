/* PR middle-end/71626 */
/* { dg-additional-options "-fpic" { target fpic } } */

#include "pr71626-1.c"


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
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = vector<i64, 1>;
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> vector<i64, 1> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64() -> coerce<f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: vector<i64, 1> [storage=automatic] = aggregate<vector<i64, 1>, zero_fill=false>(index0 = ptr_to_int<i64, reason=explicit>(function_decay<ptr<fn() -> vector<i64, 1>>>(%[[VALUE_foo]])));
// DEFAULT-NEXT:         return read<vector<i64, 1>>(%[[VALUE_v]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_v_2:[0-9]+]] v: vector<i64, 1> [storage=automatic] = call<vector<i64, 1>, signature=fn() -> vector<i64, 1>, abi=sysv64() -> coerce<f64>>(%[[VALUE_foo]]);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(lane(%[[VALUE_v_2]], const<i32>(0))), ptr_to_int<i64, reason=explicit>(function_decay<ptr<fn() -> vector<i64, 1>>>(%[[VALUE_foo]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
