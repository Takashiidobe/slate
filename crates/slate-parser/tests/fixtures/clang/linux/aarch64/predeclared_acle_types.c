typedef __SVFloat32_t sv_f32;
typedef __SVFloat64_t sv_f64;
typedef __SVBool_t sv_bool;
typedef __SVCount_t sv_count;
typedef __Float32x4_t f32x4;
typedef __Uint8x16_t u8x16;
typedef __mfp8 mf8;

sv_f32 scale(sv_f32, sv_f64, sv_bool);
f32x4 lanes(f32x4, u8x16);

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f128;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_sv_f32:[0-9]+]] sv_f32 = sizeless<svfloat32_t>;
// DEFAULT-NEXT:     type @type[[TYPE_sv_f64:[0-9]+]] sv_f64 = sizeless<svfloat64_t>;
// DEFAULT-NEXT:     type @type[[TYPE_sv_bool:[0-9]+]] sv_bool = sizeless<svbool_t>;
// DEFAULT-NEXT:     type @type[[TYPE_sv_count:[0-9]+]] sv_count = sizeless<svcount_t>;
// DEFAULT-NEXT:     type @type[[TYPE_f32x4:[0-9]+]] f32x4 = vector<f32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_u8x16:[0-9]+]] u8x16 = vector<u8, 16>;
// DEFAULT-NEXT:     type @type[[TYPE_mf8:[0-9]+]] mf8 = u8;
// DEFAULT-NEXT:     fn %[[VALUE_scale:[0-9]+]] @scale(%[[VALUE0:[0-9]+]] <unnamed>: sizeless<svfloat32_t> [c="sv_f32"], %[[VALUE1:[0-9]+]] <unnamed>: sizeless<svfloat64_t> [c="sv_f64"], %[[VALUE2:[0-9]+]] <unnamed>: sizeless<svbool_t> [c="sv_bool"]) -> sizeless<svfloat32_t> [linkage=external] [c="sv_f32(sv_f32, sv_f64, sv_bool)"];
// DEFAULT-NEXT:     fn %[[VALUE_lanes:[0-9]+]] @lanes(%[[VALUE3:[0-9]+]] <unnamed>: vector<f32, 4> [c="f32x4"], %[[VALUE4:[0-9]+]] <unnamed>: vector<u8, 16> [c="u8x16"]) -> vector<f32, 4> [linkage=external] [abi=aapcs64(direct, direct) -> direct] [c="f32x4(f32x4, u8x16)"];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
