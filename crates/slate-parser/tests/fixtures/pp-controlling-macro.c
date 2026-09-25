#ifdef A
#include "controlling-macro.h"
#endif
#include "controlling-macro.h"
#include "controlling-macro.h"
#include "partially-guarded.h"
guarded_t value;
trailing_t trailing;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES A A
// SLATE-FILECHECK-DEFINES SKIP PARTIALLY_GUARDED_H

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
// DEFAULT-NEXT:     type @type0 guarded_t = i32;
// DEFAULT-NEXT:     type @type1 trailing_t = i32;
// DEFAULT-NEXT:     global %2 value: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 trailing: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN A
// A: module {
// A-NEXT:     target "x86_64-unknown-linux-gnu" {
// A-NEXT:         endian = little;
// A-NEXT:         pointer [size=8, align=8];
// A-NEXT:         stack_alignment = 16;
// A-NEXT:         long_double = f80;
// A-NEXT:         storage bool [size=1, align=1];
// A-NEXT:         storage i8, u8 [size=1, align=1];
// A-NEXT:         storage i16, u16 [size=2, align=2];
// A-NEXT:         storage i32, u32 [size=4, align=4];
// A-NEXT:         storage i64, u64 [size=8, align=8];
// A-NEXT:         storage i128, u128 [size=16, align=16];
// A-NEXT:         storage bf16 [size=2, align=2];
// A-NEXT:         storage f16 [size=2, align=2];
// A-NEXT:         storage f32 [size=4, align=4];
// A-NEXT:         storage f64 [size=8, align=8];
// A-NEXT:         storage f80 [size=16, align=16];
// A-NEXT:         storage f128 [size=16, align=16];
// A-NEXT:         storage d32 [size=4, align=4];
// A-NEXT:         storage d64 [size=8, align=8];
// A-NEXT:         storage d128 [size=16, align=16];
// A-NEXT:     }
// A-NEXT:     type @type0 guarded_t = i32;
// A-NEXT:     type @type1 trailing_t = i32;
// A-NEXT:     global %2 value: i32 [storage=static] [linkage=external];
// A-NEXT:     global %3 trailing: i32 [storage=static] [linkage=external];
// A-NEXT: }
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN SKIP
// SKIP: module {
// SKIP-NEXT:     target "x86_64-unknown-linux-gnu" {
// SKIP-NEXT:         endian = little;
// SKIP-NEXT:         pointer [size=8, align=8];
// SKIP-NEXT:         stack_alignment = 16;
// SKIP-NEXT:         long_double = f80;
// SKIP-NEXT:         storage bool [size=1, align=1];
// SKIP-NEXT:         storage i8, u8 [size=1, align=1];
// SKIP-NEXT:         storage i16, u16 [size=2, align=2];
// SKIP-NEXT:         storage i32, u32 [size=4, align=4];
// SKIP-NEXT:         storage i64, u64 [size=8, align=8];
// SKIP-NEXT:         storage i128, u128 [size=16, align=16];
// SKIP-NEXT:         storage bf16 [size=2, align=2];
// SKIP-NEXT:         storage f16 [size=2, align=2];
// SKIP-NEXT:         storage f32 [size=4, align=4];
// SKIP-NEXT:         storage f64 [size=8, align=8];
// SKIP-NEXT:         storage f80 [size=16, align=16];
// SKIP-NEXT:         storage f128 [size=16, align=16];
// SKIP-NEXT:         storage d32 [size=4, align=4];
// SKIP-NEXT:         storage d64 [size=8, align=8];
// SKIP-NEXT:         storage d128 [size=16, align=16];
// SKIP-NEXT:     }
// SKIP-NEXT:     type @type0 guarded_t = i32;
// SKIP-NEXT:     type @type1 trailing_t = i32;
// SKIP-NEXT:     global %2 value: i32 [storage=static] [linkage=external];
// SKIP-NEXT:     global %3 trailing: i32 [storage=static] [linkage=external];
// SKIP-NEXT: }
// SLATE-FILECHECK-END SKIP
