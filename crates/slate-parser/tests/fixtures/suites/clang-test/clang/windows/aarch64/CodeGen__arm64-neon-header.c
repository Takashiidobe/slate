

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
// DEFAULT-NEXT:     type @type[[TYPE_int32x4_t:[0-9]+]] int32x4_t = vector<i32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_int32x4x4_t:[0-9]+]] int32x4x4_t = struct {
// DEFAULT-NEXT:         field0 val: array<vector<i32, 4>, 4> [c="int32x4_t[4]"];
// DEFAULT-NEXT:     } [size=64, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_int32x4x4_t_2:[0-9]+]] int32x4x4_t = @type[[TYPE_int32x4x4_t]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_neon_vld1q_x4_v:[0-9]+]] @__builtin_neon_vld1q_x4_v(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_vld1q_s32_x4:[0-9]+]] @test_vld1q_s32_x4(%[[VALUE_a:[0-9]+]] a: ptr<const i32> [c="const int32_t *"]) -> @type[[TYPE_int32x4x4_t]] [linkage=external] [abi=win_arm64(scalar) -> native_c] [fallthrough=ub_if_used] [c="int32x4x4_t(const int32_t *)"] {
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: @type[[TYPE_int32x4x4_t]] [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___ret:[0-9]+]] __ret: @type[[TYPE_int32x4x4_t]] [storage=automatic] [c="int32x4x4_t"];
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, ptr<const void>, i32) -> void>(%[[VALUE___builtin_neon_vld1q_x4_v]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_int32x4x4_t]]>>(%[[VALUE___ret]]) [c="int32x4x4_t *"]), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_a]]) [c="const int32_t *"]), const<i32>(34));
// DEFAULT-NEXT:             write<@type[[TYPE_int32x4x4_t]]>(%[[VALUE3]], read<@type[[TYPE_int32x4x4_t]]>(%[[VALUE___ret]]) [c="int32x4x4_t"]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return copy<@type[[TYPE_int32x4x4_t]], reason=return>(read<@type[[TYPE_int32x4x4_t]]>(%[[VALUE3]]) [c="int32x4x4_t"]) [c="int32x4x4_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
