// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT

#define str(x) #x
#define first(x, y) #x

const char lone[] = str(\);
const char escaped[] = str(\\);
const char odd_run[] = str(\\\);
const char before_identifier[] = str(a \n);
const char lone_argument[] = first(\, ignored);
int caf\u00e9 = 1;

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
// DEFAULT-NEXT:     global %[[VALUE_lone:[0-9]+]] lone: array<i8, 1> [storage=static] [const] = code_units<array<i8, 1>>([0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_escaped:[0-9]+]] escaped: array<i8, 2> [storage=static] [const] = code_units<array<i8, 2>>([92, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_odd_run:[0-9]+]] odd_run: array<i8, 2> [storage=static] [const] = code_units<array<i8, 2>>([92, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_before_identifier:[0-9]+]] before_identifier: array<i8, 4> [storage=static] [const] = code_units<array<i8, 4>>([97, 32, 10, 0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_lone_argument:[0-9]+]] lone_argument: array<i8, 1> [storage=static] [const] = code_units<array<i8, 1>>([0]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE0:[0-9]+]] caf\u00e9: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
