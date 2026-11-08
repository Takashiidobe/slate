// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir /Zc:preprocessor

#include "include_backslash_paths\sub\inner.h"

#if !__has_include("include_backslash_paths\sub\inner.h")
#error backslash __has_include failed
#endif

int value(void) { return inner_value(); }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_outer_value:[0-9]+]] @outer_value() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(41);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_inner_value:[0-9]+]] @inner_value() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32>(call<i32>(%[[VALUE_outer_value]]), const<i32>(1));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_value:[0-9]+]] @value() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32>(%[[VALUE_inner_value]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
