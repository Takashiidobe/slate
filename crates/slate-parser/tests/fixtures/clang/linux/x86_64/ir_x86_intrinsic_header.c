// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

#include <emmintrin.h>

int sign_mask(__m128i bytes) {
  return _mm_movemask_epi8(bytes);
}

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
// IR-NEXT:     type @type[[TYPE___m128i:[0-9]+]] __m128i = vector<i64, 2>;
// IR-NEXT:     type @type[[TYPE___v16qi:[0-9]+]] __v16qi = vector<i8, 16>;
// IR-NEXT:     fn %[[VALUE___builtin_ia32_pmovmskb128:[0-9]+]] @__builtin_ia32_pmovmskb128(%[[VALUE0:[0-9]+]] <unnamed>: vector<i8, 16>) -> i32 [linkage=external] [memory=none] [abi=sysv64(direct) -> scalar];
// IR-NEXT:     fn %[[VALUE__mm_movemask_epi8:[0-9]+]] @_mm_movemask_epi8(%[[VALUE___a:[0-9]+]] __a: vector<i64, 2>) -> i32 [linkage=internal] [inline=always] [definition=emitted] [target=+sse2] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(vector<i8, 16>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE___builtin_ia32_pmovmskb128]], vector_bit_cast<vector<i8, 16>, reason=explicit>(read<vector<i64, 2>>(%[[VALUE___a]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sign_mask:[0-9]+]] @sign_mask(%[[VALUE_bytes:[0-9]+]] bytes: vector<i64, 2>) -> i32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(vector<i64, 2>) -> i32, abi=sysv64(direct) -> scalar>(%[[VALUE__mm_movemask_epi8]], read<vector<i64, 2>>(%[[VALUE_bytes]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
