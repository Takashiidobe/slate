#include "header-provenance-headers/local.h"

outer_int from_outer;
nested_int from_nested;
local_int from_local;

int use_wrapper(void) {
    return local_wrapper(1);
}

// SLATE-FILECHECK-ISYSTEM tests/fixtures/inputs/header-provenance-headers/system

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
// DEFAULT-NEXT:     type @type0 local_int = i32;
// DEFAULT-NEXT:     type @type1 nested_int = i64;
// DEFAULT-NEXT:     type @type2 outer_int = i64;
// DEFAULT-NEXT:     global %7 from_outer: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 from_nested: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 from_local: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @system_call(%11 value: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @local_wrapper(%6 value: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%4, read<i32>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @use_wrapper() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(%5, const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
