// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-metadata /Zc:preprocessor
#include <intrin.h>

void nop(void) { __nop(); }
unsigned __int64 counter(unsigned int *aux) { return __rdtscp(aux); }
int saturate(float value) { return _cvt_ftoi_sat(value); }
__m128 convert_vector(__m128 vector, __int64 value) {
  return _mm_cvtsi64x_ss(vector, value);
}
char interlocked_and(volatile char *value, char mask) {
  return _InterlockedAnd8(value, mask);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE___m128:[0-9]+]] __m128 = union {
// IR-NEXT:         field0 m128_f32: array<f32, 4>;
// IR-NEXT:         field1 m128_u64: array<u64, 2>;
// IR-NEXT:         field2 m128_i8: array<i8, 16>;
// IR-NEXT:         field3 m128_i16: array<i16, 8>;
// IR-NEXT:         field4 m128_i32: array<i32, 4>;
// IR-NEXT:         field5 m128_i64: array<i64, 2>;
// IR-NEXT:         field6 m128_u8: array<u8, 16>;
// IR-NEXT:         field7 m128_u16: array<u16, 8>;
// IR-NEXT:         field8 m128_u32: array<u32, 4>;
// IR-NEXT:     } [size=16, align=16, offsets=[0, 0, 0, 0, 0, 0, 0, 0, 0]];
// IR-NEXT:     type @type[[TYPE___m128_2:[0-9]+]] __m128 = @type[[TYPE___m128]] [c="union __m128"];
// IR-NEXT:     fn %[[VALUE__InterlockedAnd8:[0-9]+]] @_InterlockedAnd8(%[[VALUE__Value:[0-9]+]] _Value: ptr<volatile i8> [c="volatile char *"], %[[VALUE__Mask:[0-9]+]] _Mask: i8 [c="char"]) -> i8 [linkage=external] [c="char(volatile char *, char)"] [c_builtin="_InterlockedAnd8"] [c_builtin_kind="language"];
// IR-NEXT:     fn %[[VALUE__cvt_ftoi_sat:[0-9]+]] @_cvt_ftoi_sat(%[[VALUE0:[0-9]+]] <unnamed>: f32 [c="float"]) -> i32 [linkage=external] [c="int(float)"] [c_builtin="_cvt_ftoi_sat"] [c_builtin_kind="builtin"] [c_builtin_header="intrin.h"];
// IR-NEXT:     fn %[[VALUE___nop:[0-9]+]] @__nop() -> void [linkage=external] [c="void(void)"] [c_builtin="__nop"] [c_builtin_kind="builtin"] [c_builtin_header="intrin.h"];
// IR-NEXT:     fn %[[VALUE___rdtscp:[0-9]+]] @__rdtscp(%[[VALUE1:[0-9]+]] <unnamed>: ptr<u32> [c="unsigned int *"]) -> u64 [linkage=external] [c="unsigned long long(unsigned int *)"] [c_builtin="__rdtscp"] [c_builtin_kind="builtin"] [c_builtin_header="intrin.h"];
// IR-NEXT:     fn %[[VALUE__mm_cvtsi64x_ss:[0-9]+]] @_mm_cvtsi64x_ss(%[[VALUE2:[0-9]+]] <unnamed>: @type[[TYPE___m128]] [c="__m128"] [c_canon="union __m128"] [typedef_chain="__m128"], %[[VALUE3:[0-9]+]] <unnamed>: i64 [c="long long"]) -> @type[[TYPE___m128]] [linkage=external] [abi=win64(native_c, scalar) -> native_c] [c="__m128(__m128, long long)"] [c_canon="union __m128(union __m128, long long)"] [typedef_chain="__m128"] [c_builtin="_mm_cvtsi64x_ss"] [c_builtin_kind="builtin"] [c_builtin_header="intrin.h"];
// IR-NEXT:     fn %[[VALUE_nop:[0-9]+]] @nop() -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void)"] {
// IR-NEXT:         call<void>(%[[VALUE___nop]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_counter:[0-9]+]] @counter(%[[VALUE_aux:[0-9]+]] aux: ptr<u32> [c="unsigned int *"]) -> u64 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="unsigned long long"] [c="unsigned long long(unsigned int *)"] {
// IR-NEXT:         return call<u64>(%[[VALUE___rdtscp]], read<ptr<u32>>(%[[VALUE_aux]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_saturate:[0-9]+]] @saturate(%[[VALUE_value:[0-9]+]] value: f32 [c="float"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(float)"] {
// IR-NEXT:         return call<i32>(%[[VALUE__cvt_ftoi_sat]], read<f32>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_convert_vector:[0-9]+]] @convert_vector(%[[VALUE_vector:[0-9]+]] vector: @type[[TYPE___m128]] [c="__m128"] [c_canon="union __m128"] [typedef_chain="__m128"], %[[VALUE_value_2:[0-9]+]] value: i64 [c="long long"]) -> @type[[TYPE___m128]] [linkage=external] [abi=win64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] [c_storage="none"] [c_return="__m128"] [c="__m128(__m128, long long)"] [c_canon="union __m128(union __m128, long long)"] [typedef_chain="__m128"] {
// IR-NEXT:         return copy<@type[[TYPE___m128]], reason=return>(call<@type[[TYPE___m128]], abi=win64(native_c, scalar) -> native_c>(%[[VALUE__mm_cvtsi64x_ss]], copy<@type[[TYPE___m128]], reason=arg>(read<@type[[TYPE___m128]]>(%[[VALUE_vector]]) [c="__m128"]) [c="__m128"], read<i64>(%[[VALUE_value_2]])) [c="__m128"]) [c="__m128"];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_interlocked_and:[0-9]+]] @interlocked_and(%[[VALUE_value_3:[0-9]+]] value: ptr<volatile i8> [c="volatile char *"], %[[VALUE_mask:[0-9]+]] mask: i8 [c="char"]) -> i8 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="char"] [c="char(volatile char *, char)"] {
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i8 [synthetic] = update<i8, result=old, volatile, atomic=seq_cst>(deref(read<ptr<volatile i8>>(%[[VALUE_value_3]])), and<i8>(old<i8>, read<i8>(%[[VALUE_mask]]))) [c_builtin="_InterlockedAnd8"];
// IR-NEXT:         return read<i8>(%[[VALUE4]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
