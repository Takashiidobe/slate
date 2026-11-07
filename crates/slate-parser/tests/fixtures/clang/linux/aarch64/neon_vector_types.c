#include <arm_neon.h>

_Static_assert(sizeof(int8x8_t) == 8 && _Alignof(int8x8_t) == 8, "int8x8_t");
_Static_assert(sizeof(int32x4_t) == 16 && _Alignof(int32x4_t) == 16, "int32x4_t");
_Static_assert(sizeof(float16x4_t) == 8, "float16x4_t");
_Static_assert(sizeof(poly8x16_t) == 16, "poly8x16_t");
_Static_assert(sizeof(int32x4x4_t) == 64 && _Alignof(int32x4x4_t) == 16, "int32x4x4_t");

int32x4_t reinterpret(float32x4_t v) { return vreinterpretq_s32_f32(v); }

int32x4_t add(int32x4_t a, int32x4_t b) { return vaddq_s32(a, b); }

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
// DEFAULT-NEXT:     type @type[[TYPE___int8_t:[0-9]+]] __int8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE___uint8_t:[0-9]+]] __uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE___int32_t:[0-9]+]] __int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_int8_t:[0-9]+]] int8_t = i8 [c="__int8_t"];
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32 [c="__int32_t"];
// DEFAULT-NEXT:     type @type[[TYPE_uint8_t:[0-9]+]] uint8_t = u8 [c="__uint8_t"];
// DEFAULT-NEXT:     type @type[[TYPE_float32_t:[0-9]+]] float32_t = f32;
// DEFAULT-NEXT:     type @type[[TYPE_float16_t:[0-9]+]] float16_t = f16;
// DEFAULT-NEXT:     type @type[[TYPE_int8x8_t:[0-9]+]] int8x8_t = vector<i8, 8>;
// DEFAULT-NEXT:     type @type[[TYPE_int32x4_t:[0-9]+]] int32x4_t = vector<i32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_float16x4_t:[0-9]+]] float16x4_t = vector<f16, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_float32x4_t:[0-9]+]] float32x4_t = vector<f32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_int32x4x4_t:[0-9]+]] int32x4x4_t = struct {
// DEFAULT-NEXT:         field0 val: array<vector<i32, 4>, 4> [c="int32x4_t[4]"];
// DEFAULT-NEXT:     } [size=64, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_int32x4x4_t_2:[0-9]+]] int32x4x4_t = @type[[TYPE_int32x4x4_t]];
// DEFAULT-NEXT:     type @type[[TYPE_poly8_t:[0-9]+]] poly8_t = u8 [c="uint8_t"];
// DEFAULT-NEXT:     type @type[[TYPE_poly8x16_t:[0-9]+]] poly8x16_t = vector<u8, 16>;
// DEFAULT-NEXT:     fn %[[VALUE_vaddq_s32:[0-9]+]] @vaddq_s32(%[[VALUE___p0:[0-9]+]] __p0: vector<i32, 4> [c="int32x4_t"], %[[VALUE___p1:[0-9]+]] __p1: vector<i32, 4> [c="int32x4_t"]) -> vector<i32, 4> [linkage=internal] [inline=always] [definition=emitted] [target=+neon] [abi=aapcs64(direct, direct) -> direct] [fallthrough=ub_if_used] [c="int32x4_t(int32x4_t, int32x4_t)"] {
// DEFAULT-NEXT:         let %[[VALUE___ret:[0-9]+]] __ret: vector<i32, 4> [storage=automatic] [c="int32x4_t"];
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE___ret]], add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%[[VALUE___p0]]) [c="int32x4_t"], read<vector<i32, 4>>(%[[VALUE___p1]]) [c="int32x4_t"]) [c="int32x4_t"]) [c="int32x4_t"];
// DEFAULT-NEXT:         return read<vector<i32, 4>>(%[[VALUE___ret]]) [c="int32x4_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_vreinterpretq_s32_f32:[0-9]+]] @vreinterpretq_s32_f32(%[[VALUE___p0_2:[0-9]+]] __p0: vector<f32, 4> [c="float32x4_t"]) -> vector<i32, 4> [linkage=internal] [inline=always] [definition=emitted] [target=+neon] [abi=aapcs64(direct) -> direct] [fallthrough=ub_if_used] [c="int32x4_t(float32x4_t)"] {
// DEFAULT-NEXT:         let %[[VALUE___ret_2:[0-9]+]] __ret: vector<i32, 4> [storage=automatic] [c="int32x4_t"];
// DEFAULT-NEXT:         write<vector<i32, 4>>(%[[VALUE___ret_2]], bit_cast<vector<i32, 4>, reason=explicit>(read<vector<f32, 4>>(%[[VALUE___p0_2]]) [c="float32x4_t"]) [c="int32x4_t"]) [c="int32x4_t"];
// DEFAULT-NEXT:         return read<vector<i32, 4>>(%[[VALUE___ret_2]]) [c="int32x4_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_reinterpret:[0-9]+]] @reinterpret(%[[VALUE_v:[0-9]+]] v: vector<f32, 4> [c="float32x4_t"]) -> vector<i32, 4> [linkage=external] [abi=aapcs64(direct) -> direct] [fallthrough=ub_if_used] [c="int32x4_t(float32x4_t)"] {
// DEFAULT-NEXT:         return call<vector<i32, 4>, signature=fn(vector<f32, 4>) -> vector<i32, 4>, abi=aapcs64(direct) -> direct>(%[[VALUE_vreinterpretq_s32_f32]], read<vector<f32, 4>>(%[[VALUE_v]]) [c="float32x4_t"]) [c="int32x4_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: vector<i32, 4> [c="int32x4_t"], %[[VALUE_b:[0-9]+]] b: vector<i32, 4> [c="int32x4_t"]) -> vector<i32, 4> [linkage=external] [abi=aapcs64(direct, direct) -> direct] [fallthrough=ub_if_used] [c="int32x4_t(int32x4_t, int32x4_t)"] {
// DEFAULT-NEXT:         return call<vector<i32, 4>, signature=fn(vector<i32, 4>, vector<i32, 4>) -> vector<i32, 4>, abi=aapcs64(direct, direct) -> direct>(%[[VALUE_vaddq_s32]], read<vector<i32, 4>>(%[[VALUE_a]]) [c="int32x4_t"], read<vector<i32, 4>>(%[[VALUE_b]]) [c="int32x4_t"]) [c="int32x4_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
