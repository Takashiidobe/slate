#include <arm_fp16.h>

float16_t absolute(float16_t x) { return vabsh_f16(x); }

float16_t from_wide(int64_t x) { return vcvth_f16_s64(x); }

float16_t fixed_point(int32_t x) { return vcvth_n_f16_s32(x, 2); }

void load_quad(int32_t out[16], const int32_t *in) { __builtin_neon_vld1q_x4_v(out, in, 34); }

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
// DEFAULT-NEXT:     type @type[[TYPE___int32_t:[0-9]+]] __int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___int64_t:[0-9]+]] __int64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32 [c="__int32_t"];
// DEFAULT-NEXT:     type @type[[TYPE_int64_t:[0-9]+]] int64_t = i64 [c="__int64_t"];
// DEFAULT-NEXT:     type @type[[TYPE_float16_t:[0-9]+]] float16_t = f16;
// DEFAULT-NEXT:     fn %[[VALUE___builtin_neon_vabsh_f16:[0-9]+]] @__builtin_neon_vabsh_f16(%[[VALUE0:[0-9]+]] <unnamed>: f16) -> f16 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_absolute:[0-9]+]] @absolute(%[[VALUE_x:[0-9]+]] x: f16 [c="float16_t"]) -> f16 [linkage=external] [fallthrough=ub_if_used] [c="float16_t(float16_t)"] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: f16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___ret:[0-9]+]] __ret: f16 [storage=automatic] [c="float16_t"];
// DEFAULT-NEXT:             let %[[VALUE___s0:[0-9]+]] __s0: f16 [storage=automatic] = read<f16>(%[[VALUE_x]]) [c="float16_t"] [c="float16_t"];
// DEFAULT-NEXT:             write<f16>(%[[VALUE___ret]], bit_cast<f16, reason=explicit>(call<f16, signature=fn(f16) -> f16>(%[[VALUE___builtin_neon_vabsh_f16]], read<f16>(%[[VALUE___s0]]))) [c="float16_t"]) [c="float16_t"];
// DEFAULT-NEXT:             write<f16>(%[[VALUE1]], read<f16>(%[[VALUE___ret]]) [c="float16_t"]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f16>(%[[VALUE1]]) [c="float16_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_neon_vcvth_f16_s64:[0-9]+]] @__builtin_neon_vcvth_f16_s64(%[[VALUE2:[0-9]+]] <unnamed>: i64) -> f16 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_from_wide:[0-9]+]] @from_wide(%[[VALUE_x_2:[0-9]+]] x: i64 [c="int64_t"]) -> f16 [linkage=external] [fallthrough=ub_if_used] [c="float16_t(int64_t)"] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: f16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___ret_2:[0-9]+]] __ret: f16 [storage=automatic] [c="float16_t"];
// DEFAULT-NEXT:             let %[[VALUE___s0_2:[0-9]+]] __s0: i64 [storage=automatic] = read<i64>(%[[VALUE_x_2]]) [c="int64_t"] [c="int64_t"];
// DEFAULT-NEXT:             write<f16>(%[[VALUE___ret_2]], bit_cast<f16, reason=explicit>(call<f16, signature=fn(i64) -> f16>(%[[VALUE___builtin_neon_vcvth_f16_s64]], reinterpret<i64, reason=arg>(read<i64>(%[[VALUE___s0_2]]) [c="int64_t"]))) [c="float16_t"]) [c="float16_t"];
// DEFAULT-NEXT:             write<f16>(%[[VALUE3]], read<f16>(%[[VALUE___ret_2]]) [c="float16_t"]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f16>(%[[VALUE3]]) [c="float16_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_neon_vcvth_n_f16_s32:[0-9]+]] @__builtin_neon_vcvth_n_f16_s32(%[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: i32) -> f16 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fixed_point:[0-9]+]] @fixed_point(%[[VALUE_x_3:[0-9]+]] x: i32 [c="int32_t"]) -> f16 [linkage=external] [fallthrough=ub_if_used] [c="float16_t(int32_t)"] {
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: f16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___ret_3:[0-9]+]] __ret: f16 [storage=automatic] [c="float16_t"];
// DEFAULT-NEXT:             let %[[VALUE___s0_3:[0-9]+]] __s0: i32 [storage=automatic] = read<i32>(%[[VALUE_x_3]]) [c="int32_t"] [c="int32_t"];
// DEFAULT-NEXT:             write<f16>(%[[VALUE___ret_3]], bit_cast<f16, reason=explicit>(call<f16, signature=fn(i32, i32) -> f16>(%[[VALUE___builtin_neon_vcvth_n_f16_s32]], reinterpret<i32, reason=arg>(read<i32>(%[[VALUE___s0_3]]) [c="int32_t"]), const<i32>(2))) [c="float16_t"]) [c="float16_t"];
// DEFAULT-NEXT:             write<f16>(%[[VALUE6]], read<f16>(%[[VALUE___ret_3]]) [c="float16_t"]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f16>(%[[VALUE6]]) [c="float16_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_neon_vld1q_x4_v:[0-9]+]] @__builtin_neon_vld1q_x4_v(%[[VALUE7:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE8:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE9:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_load_quad:[0-9]+]] @load_quad(%[[VALUE_out:[0-9]+]] out: ptr<i32> [array=16] [c="int32_t[16]"], %[[VALUE_in:[0-9]+]] in: ptr<const i32> [c="const int32_t *"]) -> void [linkage=external] [fallthrough=ret_void] [c="void(int32_t[16], const int32_t *)"] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<const void>, i32) -> void>(%[[VALUE___builtin_neon_vld1q_x4_v]], pointer_cast<ptr<void>, reason=arg>(read<ptr<i32>>(%[[VALUE_out]]) [c="int32_t *"]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_in]]) [c="const int32_t *"]), const<i32>(34));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
