#include "search_order.h"
int quoted_selected[SEARCH_SELECTED];
#undef SEARCH_SELECTED
#include <search_order.h>
#include <user_only.h>
#include <next.h>
int angled_selected[SEARCH_SELECTED];
int user_selected[USER_SELECTED];
int next_first[NEXT_FIRST];
int next_second[NEXT_SECOND];

// SLATE-FILECHECK-ARGS -Itests/fixtures/include_search/user -iquote tests/fixtures/include_search/quote -isystem tests/fixtures/include_search/shim
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
// DEFAULT-NEXT:     global %0 quoted_selected: array<i32, 77> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %1 angled_selected: array<i32, 66> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %2 user_selected: array<i32, 67> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %3 next_first: array<i32, 68> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %4 next_second: array<i32, 12> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
