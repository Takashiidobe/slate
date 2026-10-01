#define GUARDED_NAME guarded_first
#include "multiple-include-guarded.h"
#undef GUARDED_NAME
#define GUARDED_NAME guarded_skipped
#include "multiple-include-guarded.h"
#undef MULTIPLE_INCLUDE_GUARDED_H
#undef GUARDED_NAME
#define GUARDED_NAME guarded_second
#include "multiple-include-guarded.h"

#define TRAILING_NAME trailing_first
#include "multiple-include-trailing.h"
#undef TRAILING_NAME
#define TRAILING_NAME trailing_second
#include "multiple-include-trailing.h"



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
// DEFAULT-NEXT:     global %[[VALUE_guarded_first:[0-9]+]] guarded_first: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_guarded_second:[0-9]+]] guarded_second: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_trailing_first:[0-9]+]] trailing_first: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_trailing_second:[0-9]+]] trailing_second: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
