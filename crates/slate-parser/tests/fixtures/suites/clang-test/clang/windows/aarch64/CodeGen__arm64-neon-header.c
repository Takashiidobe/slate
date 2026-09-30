

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
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_poly8_t:[0-9]+]] poly8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_poly16_t:[0-9]+]] poly16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_poly32_t:[0-9]+]] poly32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_poly64_t:[0-9]+]] poly64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE___n64:[0-9]+]] __n64 = union {
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
// DEFAULT-NEXT:     type @type[[TYPE___n64_2:[0-9]+]] __n64 = @type[[TYPE___n64]];
// DEFAULT-NEXT:     type @type[[TYPE___n128:[0-9]+]] __n128 = union {
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
// DEFAULT-NEXT:         field14 s: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 low64: @type[[TYPE___n64]];
// DEFAULT-NEXT:         field1 high64: @type[[TYPE___n64]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE___n128_2:[0-9]+]] __n128 = @type[[TYPE___n128]];
// DEFAULT-NEXT:     type @type[[TYPE___n128x4:[0-9]+]] __n128x4 = struct {
// DEFAULT-NEXT:         field0 val: array<@type[[TYPE___n128]], 4>;
// DEFAULT-NEXT:     } [size=64, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE___n128x4_2:[0-9]+]] __n128x4 = @type[[TYPE___n128x4]];
// DEFAULT-NEXT:     type @type[[TYPE_int32x4x4_t:[0-9]+]] int32x4x4_t = @type[[TYPE___n128x4]];
// DEFAULT-NEXT:     fn %[[VALUE_neon_ld1m4_q32:[0-9]+]] @neon_ld1m4_q32(%[[VALUE_ptr:[0-9]+]] ptr: ptr<const i32>) -> @type[[TYPE___n128x4]] [linkage=external] [abi=win_arm64(scalar) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_test_vld1q_s32_x4:[0-9]+]] @test_vld1q_s32_x4(%[[VALUE_a:[0-9]+]] a: ptr<const i32>) -> @type[[TYPE___n128x4]] [linkage=external] [abi=win_arm64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return copy<@type[[TYPE___n128x4]], reason=return>(call<@type[[TYPE___n128x4]], signature=fn(ptr<const i32>) -> @type[[TYPE___n128x4]], abi=win_arm64(scalar) -> native_c>(%[[VALUE_neon_ld1m4_q32]], pointer_cast<ptr<const i32>, reason=arg>(pointer_cast<ptr<i32>, reason=explicit>(read<ptr<const i32>>(%[[VALUE_a]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
