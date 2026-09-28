

#include <arm64_neon.h>

int32x4x4_t test_vld1q_s32_x4(int32_t const *a) {
  return vld1q_s32_x4(a);
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:     type @type0 int32_t = i32;
// DEFAULT-NEXT:     type @type1 poly8_t = u8;
// DEFAULT-NEXT:     type @type2 poly16_t = u16;
// DEFAULT-NEXT:     type @type3 poly32_t = u32;
// DEFAULT-NEXT:     type @type4 poly64_t = u64;
// DEFAULT-NEXT:     type @type5 __n64 = union {
// DEFAULT-NEXT:         field0 n64_u64: array<u64, 1>;
// DEFAULT-NEXT:         field1 n64_u32: array<u32, 2>;
// DEFAULT-NEXT:         field2 n64_u16: array<u16, 4>;
// DEFAULT-NEXT:         field3 n64_u8: array<u8, 8>;
// DEFAULT-NEXT:         field4 n64_i64: array<i64, 1>;
// DEFAULT-NEXT:         field5 n64_i32: array<i32, 2>;
// DEFAULT-NEXT:         field6 n64_i16: array<i16, 4>;
// DEFAULT-NEXT:         field7 n64_i8: array<i8, 8>;
// DEFAULT-NEXT:         field8 n64_p64: array<u64, 1>;
// DEFAULT-NEXT:         field9 n64_p32: array<u32, 2>;
// DEFAULT-NEXT:         field10 n64_p16: array<u16, 4>;
// DEFAULT-NEXT:         field11 n64_p8: array<u8, 8>;
// DEFAULT-NEXT:         field12 n64_f32: array<f32, 2>;
// DEFAULT-NEXT:         field13 n64_f64: array<f64, 1>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type6 __n64 = @type5;
// DEFAULT-NEXT:     type @type7 __n128 = union {
// DEFAULT-NEXT:         field0 n128_u64: array<u64, 2>;
// DEFAULT-NEXT:         field1 n128_u32: array<u32, 4>;
// DEFAULT-NEXT:         field2 n128_u16: array<u16, 8>;
// DEFAULT-NEXT:         field3 n128_u8: array<u8, 16>;
// DEFAULT-NEXT:         field4 n128_i64: array<i64, 2>;
// DEFAULT-NEXT:         field5 n128_i32: array<i32, 4>;
// DEFAULT-NEXT:         field6 n128_i16: array<i16, 8>;
// DEFAULT-NEXT:         field7 n128_i8: array<i8, 16>;
// DEFAULT-NEXT:         field8 n128_p64: array<u64, 2>;
// DEFAULT-NEXT:         field9 n128_p32: array<u32, 4>;
// DEFAULT-NEXT:         field10 n128_p16: array<u16, 8>;
// DEFAULT-NEXT:         field11 n128_p8: array<u8, 16>;
// DEFAULT-NEXT:         field12 n128_f32: array<f32, 4>;
// DEFAULT-NEXT:         field13 n128_f64: array<f64, 2>;
// DEFAULT-NEXT:         field14 s: @type8;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type8 = struct {
// DEFAULT-NEXT:         field0 low64: @type5;
// DEFAULT-NEXT:         field1 high64: @type5;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type9 __n128 = @type7;
// DEFAULT-NEXT:     type @type10 __n128x4 = struct {
// DEFAULT-NEXT:         field0 val: array<@type7, 4>;
// DEFAULT-NEXT:     } [size=64, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type11 __n128x4 = @type10;
// DEFAULT-NEXT:     type @type12 int32x4x4_t = @type10;
// DEFAULT-NEXT:     fn %14 @neon_ld1m4_q32(%17 ptr: ptr<const i32>) -> @type10 [linkage=external] [abi=win_arm64(scalar) -> native_c];
// DEFAULT-NEXT:     fn %15 @test_vld1q_s32_x4(%16 a: ptr<const i32>) -> @type10 [linkage=external] [abi=win_arm64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type10, reason=return>(call<@type10, signature=fn(ptr<const i32>) -> @type10, abi=win_arm64(scalar) -> native_c>(%14, pointer_cast<ptr<const i32>, reason=arg>(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<const i32>>(%16)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
