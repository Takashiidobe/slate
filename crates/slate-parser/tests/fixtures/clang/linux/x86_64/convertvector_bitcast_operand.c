typedef int v4si __attribute__((vector_size(16)));
typedef float v4sf __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));

v4sf to_float(v2di x) { return __builtin_convertvector((v4si)x, v4sf); }
v4si same_type(v2di x) { return __builtin_convertvector((v4si)x, v4si); }

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
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
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_v4si:[0-9]+]] v4si = vector<i32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_v4sf:[0-9]+]] v4sf = vector<f32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_v2di:[0-9]+]] v2di = vector<i64, 2>;
// DEFAULT-NEXT:     fn %[[VALUE_to_float:[0-9]+]] @to_float(%[[VALUE_x:[0-9]+]] x: vector<i64, 2> [c="v2di"]) -> vector<f32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] [c="v4sf(v2di)"] {
// DEFAULT-NEXT:         return int_to_float<vector<f32, 4>, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(vector_bit_cast<vector<i32, 4>, reason=explicit>(read<vector<i64, 2>>(%[[VALUE_x]]) [c="v2di"]) [c="v4si"]) [c="v4sf"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_same_type:[0-9]+]] @same_type(%[[VALUE_x_2:[0-9]+]] x: vector<i64, 2> [c="v2di"]) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] [c="v4si(v2di)"] {
// DEFAULT-NEXT:         return vector_bit_cast<vector<i32, 4>, reason=explicit>(read<vector<i64, 2>>(%[[VALUE_x_2]]) [c="v2di"]) [c="v4si"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
