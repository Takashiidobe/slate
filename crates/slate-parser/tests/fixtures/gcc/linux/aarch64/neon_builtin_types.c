// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES NEON
typedef __Int8x8_t int8x8_t;
typedef __Float16x4_t float16x4_t;
unsigned int fpcr(void) { return __builtin_aarch64_get_fpcr(); }
int8x8_t pair(const signed char *p) {
  int8x8x2_t t = __builtin_aarch64_ld2v8qi((const __builtin_aarch64_simd_qi *)p);
  return t.val[0];
}
void store(__fp16 *p, float16x4_t v) {
  float16x4x2_t t = {{v, v}};
  __builtin_aarch64_st2_lanev4hf((__builtin_aarch64_simd_hf *)p, t, 1);
}

// SLATE-FILECHECK-BEGIN NEON
// NEON: module {
// NEON-NEXT:     target "aarch64-unknown-linux-gnu" {
// NEON-NEXT:         endian = little;
// NEON-NEXT:         pointer [size=8, align=8];
// NEON-NEXT:         stack_alignment = 16;
// NEON-NEXT:         long_double = f128;
// NEON-NEXT:         storage bool [size=1, align=1];
// NEON-NEXT:         storage i8, u8 [size=1, align=1];
// NEON-NEXT:         storage i16, u16 [size=2, align=2];
// NEON-NEXT:         storage i32, u32 [size=4, align=4];
// NEON-NEXT:         storage i64, u64 [size=8, align=8];
// NEON-NEXT:         storage i128, u128 [size=16, align=16];
// NEON-NEXT:         storage bf16 [size=2, align=2];
// NEON-NEXT:         storage f16 [size=2, align=2];
// NEON-NEXT:         storage f32 [size=4, align=4];
// NEON-NEXT:         storage f64 [size=8, align=8];
// NEON-NEXT:         storage f128 [size=16, align=16];
// NEON-NEXT:         storage d32 [size=4, align=4];
// NEON-NEXT:         storage d64 [size=8, align=8];
// NEON-NEXT:         storage d128 [size=16, align=16];
// NEON-NEXT:     }
// NEON-NEXT:     type @type[[TYPE_float16x4x2_t:[0-9]+]] float16x4x2_t = struct {
// NEON-NEXT:         field0 val: array<vector<f16, 4>, 2>;
// NEON-NEXT:     } [size=16, align=8, offsets=[0]];
// NEON-NEXT:     type @type[[TYPE_float16x4x2_t_2:[0-9]+]] float16x4x2_t = @type[[TYPE_float16x4x2_t]];
// NEON-NEXT:     type @type[[TYPE_int8x8x2_t:[0-9]+]] int8x8x2_t = struct {
// NEON-NEXT:         field0 val: array<vector<i8, 8>, 2>;
// NEON-NEXT:     } [size=16, align=8, offsets=[0]];
// NEON-NEXT:     type @type[[TYPE_int8x8x2_t_2:[0-9]+]] int8x8x2_t = @type[[TYPE_int8x8x2_t]];
// NEON-NEXT:     type @type[[TYPE_int8x8_t:[0-9]+]] int8x8_t = vector<i8, 8>;
// NEON-NEXT:     type @type[[TYPE_float16x4_t:[0-9]+]] float16x4_t = vector<f16, 4>;
// NEON-NEXT:     fn %[[VALUE___builtin_aarch64_get_fpcr:[0-9]+]] @__builtin_aarch64_get_fpcr() -> u32 [linkage=external];
// NEON-NEXT:     fn %[[VALUE_fpcr:[0-9]+]] @fpcr() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// NEON-NEXT:         return call<u32>(%[[VALUE___builtin_aarch64_get_fpcr]]);
// NEON-NEXT:     }
// NEON-NEXT:     fn %[[VALUE___builtin_aarch64_ld2v8qi:[0-9]+]] @__builtin_aarch64_ld2v8qi(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> @type[[TYPE_int8x8x2_t]] [linkage=external] [memory=read] [abi=aapcs64(scalar) -> native_c];
// NEON-NEXT:     fn %[[VALUE_pair:[0-9]+]] @pair(%[[VALUE_p:[0-9]+]] p: ptr<const i8>) -> vector<i8, 8> [linkage=external] [abi=aapcs64(scalar) -> direct] [fallthrough=ub_if_used] [c="int8x8_t(const signed char *)"] {
// NEON-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_int8x8x2_t]] [storage=automatic] = copy<@type[[TYPE_int8x8x2_t]], reason=assign>(call<@type[[TYPE_int8x8x2_t]], abi=aapcs64(scalar) -> native_c>(%[[VALUE___builtin_aarch64_ld2v8qi]], read<ptr<const i8>>(%[[VALUE_p]])) [c="int8x8x2_t"]) [c="int8x8x2_t"] [c="int8x8x2_t"];
// NEON-NEXT:         return read<vector<i8, 8>>(deref(ptr_offset<ptr<vector<i8, 8>>, subtract=false>(array_decay<ptr<vector<i8, 8>>, length=Some(2)>(field0(%[[VALUE_t]])), const<i32>(0)))) [c="int8x8_t"];
// NEON-NEXT:     }
// NEON-NEXT:     fn %[[VALUE___builtin_aarch64_st2_lanev4hf:[0-9]+]] @__builtin_aarch64_st2_lanev4hf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<f16>, %[[VALUE2:[0-9]+]] <unnamed>: @type[[TYPE_float16x4x2_t]], %[[VALUE3:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [abi=aapcs64(scalar, native_c, scalar) -> void];
// NEON-NEXT:     fn %[[VALUE_store:[0-9]+]] @store(%[[VALUE_p_2:[0-9]+]] p: ptr<f16>, %[[VALUE_v:[0-9]+]] v: vector<f16, 4> [c="float16x4_t"]) -> void [linkage=external] [abi=aapcs64(scalar, direct) -> void] [fallthrough=ret_void] [c="void(__fp16 *, float16x4_t)"] {
// NEON-NEXT:         let %[[VALUE_t_2:[0-9]+]] t: @type[[TYPE_float16x4x2_t]] [storage=automatic] = aggregate<@type[[TYPE_float16x4x2_t]], zero_fill=false>(field0 = aggregate<array<vector<f16, 4>, 2>, zero_fill=false>(index0 = read<vector<f16, 4>>(%[[VALUE_v]]), index1 = read<vector<f16, 4>>(%[[VALUE_v]]))) [c="float16x4x2_t"];
// NEON-NEXT:         call<void, abi=aapcs64(scalar, native_c, scalar) -> void>(%[[VALUE___builtin_aarch64_st2_lanev4hf]], read<ptr<f16>>(%[[VALUE_p_2]]), copy<@type[[TYPE_float16x4x2_t]], reason=arg>(read<@type[[TYPE_float16x4x2_t]]>(%[[VALUE_t_2]]) [c="float16x4x2_t"]) [c="float16x4x2_t"], const<i32>(1));
// NEON-NEXT:     }
// NEON-NEXT: }
// SLATE-FILECHECK-END NEON
