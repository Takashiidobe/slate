// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

static char array[16];
static __SIZE_TYPE__ whole = __builtin_object_size(array, 0);
static __SIZE_TYPE__ remaining = __builtin_object_size(array + 3, 0);

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
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
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_array:[0-9]+]] array: array<i8, 16> [storage=static] [align=16] [linkage=internal];
// IR-NEXT:     global %[[VALUE_whole:[0-9]+]] whole: u64 [storage=static] = reinterpret<u64>(const<u64>(16) [c="__size_t"]) [linkage=internal];
// IR-NEXT:     global %[[VALUE_remaining:[0-9]+]] remaining: u64 [storage=static] = reinterpret<u64>(const<u64>(13) [c="__size_t"]) [linkage=internal];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
