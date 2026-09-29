struct Never;
void takes_never(struct Never);
struct Never returns_never(void);
union Opaque passes_opaque(union Opaque value);

struct Later;
struct Later completed_later(struct Later);
struct Later { long a, b, c; };

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --dump-ir

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
// DEFAULT-NEXT:     type @type0 Never = struct incomplete;
// DEFAULT-NEXT:     type @type1 Opaque = union incomplete;
// DEFAULT-NEXT:     type @type2 Later = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: i64;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     fn %1 @takes_never(%8 <unnamed>: @type0) -> void [linkage=external] [abi=incomplete];
// DEFAULT-NEXT:     fn %2 @returns_never() -> @type0 [linkage=external] [abi=incomplete];
// DEFAULT-NEXT:     fn %5 @passes_opaque(%9 value: @type1) -> @type1 [linkage=external] [abi=incomplete];
// DEFAULT-NEXT:     fn %7 @completed_later(%10 <unnamed>: @type2) -> @type2 [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
