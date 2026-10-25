// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir /DHASH#4 /DBOTH=1#2 /DEMPTY= /DPLAIN /D SEPARATE=5 /DGONE /UGONE /UREDEFINED /DREDEFINED=6 /U_WIN64 /U _MSC_VER /FItests/fixtures/inputs/msvc_options/forced.h /I tests/fixtures/inputs/msvc_options
#include <user.h>

#define STRING(text) #text
#define EXPANDED_STRING(text) STRING(text)

#if defined(GONE) || defined(_WIN64) || defined(_MSC_VER)
#error "/U removes a /D and predefines"
#endif

int hash = HASH;
const char both[] = EXPANDED_STRING(BOTH);
int empty[EMPTY 1];
int plain = PLAIN;
int separate = SEPARATE;
int redefined = REDEFINED;
int forced = FORCED_VALUE;
int user = USER_VALUE;

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
// IR-NEXT:     global %[[VALUE_hash:[0-9]+]] hash: i32 [storage=static] = const<i32>(4) [linkage=external];
// IR-NEXT:     global %[[VALUE_both:[0-9]+]] both: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([49, 61, 50, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_empty:[0-9]+]] empty: array<i32, 1> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT:     global %[[VALUE_separate:[0-9]+]] separate: i32 [storage=static] = const<i32>(5) [linkage=external];
// IR-NEXT:     global %[[VALUE_redefined:[0-9]+]] redefined: i32 [storage=static] = const<i32>(6) [linkage=external];
// IR-NEXT:     global %[[VALUE_forced:[0-9]+]] forced: i32 [storage=static] = const<i32>(7) [linkage=external];
// IR-NEXT:     global %[[VALUE_user:[0-9]+]] user: i32 [storage=static] = const<i32>(8) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
