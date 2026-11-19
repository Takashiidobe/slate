typedef svint8_t fixed_i8 __attribute__((arm_sve_vector_bits(256)));
typedef svuint16_t fixed_u16 __attribute__((arm_sve_vector_bits(256)));
typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(256)));
typedef svuint32_t fixed_u32 __attribute__((arm_sve_vector_bits(256)));
typedef svint64_t fixed_i64 __attribute__((arm_sve_vector_bits(256)));
typedef svfloat16_t fixed_f16 __attribute__((arm_sve_vector_bits(256)));
typedef svbfloat16_t fixed_bf16 __attribute__((arm_sve_vector_bits(256)));
typedef svfloat32_t fixed_f32 __attribute__((arm_sve_vector_bits(256)));
typedef svfloat64_t fixed_f64 __attribute__((arm_sve_vector_bits(256)));
typedef svmfloat8_t fixed_mf8 __attribute__((arm_sve_vector_bits(256)));
typedef svbool_t fixed_bool __attribute__((arm_sve_vector_bits(256)));
typedef int gnu_i32 __attribute__((vector_size(32)));

struct holder {
  fixed_i32 vector;
  fixed_bool predicate;
  int tail;
};

int layout[] = {
    sizeof(fixed_i8),  _Alignof(fixed_i8),  sizeof(fixed_f64),
    sizeof(fixed_bool), _Alignof(fixed_bool), sizeof(struct holder),
    _Alignof(struct holder),
};
int compatible = __builtin_types_compatible_p(fixed_i32, gnu_i32) +
                 __builtin_types_compatible_p(fixed_i32, svint32_t);
fixed_i32 table[2];
fixed_i32 initialized = {1, 2, 3};

fixed_i32 add(fixed_i32 a, fixed_i32 b) { return a + b; }
int lane(fixed_i32 a) { return a[3]; }
fixed_bool predicate_and(fixed_bool a, fixed_bool b) { return a & b; }
gnu_i32 compare(fixed_f32 a, fixed_f32 b) { return a < b; }
svint32_t to_sizeless(fixed_i32 a) { return a; }
fixed_i32 from_sizeless(svint32_t a) { return a; }
gnu_i32 to_gnu(fixed_i32 a) { return a; }
fixed_i32 from_gnu(gnu_i32 a) { return a; }
fixed_i32 reinterpret(fixed_u32 a) { return (fixed_i32)a; }
float half_lane(fixed_f16 a, fixed_bf16 b, fixed_mf8 c, fixed_u16 d, fixed_i64 e) {
  return a[0] + b[1] + c[2] + d[3] + e[0];
}

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -march=armv8.2-a+sve
// SLATE-FILECHECK-DEFINES BITS256
// SLATE-FILECHECK-PREFIX-ARGS BITS256 -msve-vector-bits=256

// SLATE-FILECHECK-BEGIN BITS256
// BITS256: module {
// BITS256-NEXT:     target "aarch64-unknown-linux-gnu" {
// BITS256-NEXT:         endian = little;
// BITS256-NEXT:         pointer [size=8, align=8];
// BITS256-NEXT:         stack_alignment = 16;
// BITS256-NEXT:         long_double = f128;
// BITS256-NEXT:         storage bool [size=1, align=1];
// BITS256-NEXT:         storage i8, u8 [size=1, align=1];
// BITS256-NEXT:         storage i16, u16 [size=2, align=2];
// BITS256-NEXT:         storage i32, u32 [size=4, align=4];
// BITS256-NEXT:         storage i64, u64 [size=8, align=8];
// BITS256-NEXT:         storage i128, u128 [size=16, align=16];
// BITS256-NEXT:         storage bf16 [size=2, align=2];
// BITS256-NEXT:         storage f16 [size=2, align=2];
// BITS256-NEXT:         storage f32 [size=4, align=4];
// BITS256-NEXT:         storage f64 [size=8, align=8];
// BITS256-NEXT:         storage f128 [size=16, align=16];
// BITS256-NEXT:         storage d32 [size=4, align=4];
// BITS256-NEXT:         storage d64 [size=8, align=8];
// BITS256-NEXT:         storage d128 [size=16, align=16];
// BITS256-NEXT:     }
// BITS256-NEXT:     type @type[[TYPE_fixed_i8:[0-9]+]] fixed_i8 = vector<i8, 32, svint8_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_u16:[0-9]+]] fixed_u16 = vector<u16, 16, svuint16_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_i32:[0-9]+]] fixed_i32 = vector<i32, 8, svint32_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_u32:[0-9]+]] fixed_u32 = vector<u32, 8, svuint32_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_i64:[0-9]+]] fixed_i64 = vector<i64, 4, svint64_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_f16:[0-9]+]] fixed_f16 = vector<f16, 16, svfloat16_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_bf16:[0-9]+]] fixed_bf16 = vector<bf16, 16, svbfloat16_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_f32:[0-9]+]] fixed_f32 = vector<f32, 8, svfloat32_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_f64:[0-9]+]] fixed_f64 = vector<f64, 4, svfloat64_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_mf8:[0-9]+]] fixed_mf8 = vector<u8, 32, svmfloat8_t>;
// BITS256-NEXT:     type @type[[TYPE_fixed_bool:[0-9]+]] fixed_bool = vector<i8, 4, svbool_t>;
// BITS256-NEXT:     type @type[[TYPE_gnu_i32:[0-9]+]] gnu_i32 = vector<i32, 8>;
// BITS256-NEXT:     type @type[[TYPE_holder:[0-9]+]] holder = struct {
// BITS256-NEXT:         field0 vector: vector<i32, 8, svint32_t> [c="fixed_i32"];
// BITS256-NEXT:         field1 predicate: vector<i8, 4, svbool_t> [c="fixed_bool"];
// BITS256-NEXT:         field2 tail: i32;
// BITS256-NEXT:     } [size=48, align=16, offsets=[0, 32, 36]];
// BITS256-NEXT:     global %[[VALUE_layout:[0-9]+]] layout: array<i32, 7> [storage=static] = aggregate<array<i32, 7>, zero_fill=false>(index0 = reinterpret<i32>(truncate<u32>(size_of<u64, vector<i8, 32, svint8_t>>(32) [c="__size_t"])), index1 = reinterpret<i32>(truncate<u32>(align_of<u64, vector<i8, 32, svint8_t>>(16) [c="__size_t"])), index2 = reinterpret<i32>(truncate<u32>(size_of<u64, vector<f64, 4, svfloat64_t>>(32) [c="__size_t"])), index3 = reinterpret<i32>(truncate<u32>(size_of<u64, vector<i8, 4, svbool_t>>(4) [c="__size_t"])), index4 = reinterpret<i32>(truncate<u32>(align_of<u64, vector<i8, 4, svbool_t>>(2) [c="__size_t"])), index5 = reinterpret<i32>(truncate<u32>(size_of<u64, @type[[TYPE_holder]]>(48) [c="__size_t"])), index6 = reinterpret<i32>(truncate<u32>(align_of<u64, @type[[TYPE_holder]]>(16) [c="__size_t"]))) [linkage=external];
// BITS256-NEXT:     global %[[VALUE_compatible:[0-9]+]] compatible: i32 [storage=static] = add<i32>(const<i32>(0), const<i32>(0)) [linkage=external];
// BITS256-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<vector<i32, 8, svint32_t>, 2> [storage=static] [linkage=external] [c="fixed_i32[2]"];
// BITS256-NEXT:     global %[[VALUE_initialized:[0-9]+]] initialized: vector<i32, 8, svint32_t> [storage=static] = aggregate<vector<i32, 8, svint32_t>, zero_fill=true>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3)) [linkage=external] [c="fixed_i32"];
// BITS256-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: vector<i32, 8, svint32_t> [c="fixed_i32"], %[[VALUE_b:[0-9]+]] b: vector<i32, 8, svint32_t> [c="fixed_i32"]) -> vector<i32, 8, svint32_t> [linkage=external] [abi=aapcs64(scalable, scalable) -> scalable] [fallthrough=ub_if_used] [c="fixed_i32(fixed_i32, fixed_i32)"] {
// BITS256-NEXT:         return add<vector<i32, 8, svint32_t>>(read<vector<i32, 8, svint32_t>>(%[[VALUE_a]]) [c="fixed_i32"], read<vector<i32, 8, svint32_t>>(%[[VALUE_b]]) [c="fixed_i32"]) [c="fixed_i32"];
// BITS256-NEXT:     }
// BITS256-NEXT:     fn %[[VALUE_lane:[0-9]+]] @lane(%[[VALUE_a_2:[0-9]+]] a: vector<i32, 8, svint32_t> [c="fixed_i32"]) -> i32 [linkage=external] [abi=aapcs64(scalable) -> scalar] [fallthrough=ub_if_used] [c="int(fixed_i32)"] {
// BITS256-NEXT:         return read<i32>(lane(%[[VALUE_a_2]], const<i32>(3)));
// BITS256-NEXT:     }
// BITS256-NEXT:     fn %[[VALUE_predicate_and:[0-9]+]] @predicate_and(%[[VALUE_a_3:[0-9]+]] a: vector<i8, 4, svbool_t> [c="fixed_bool"], %[[VALUE_b_2:[0-9]+]] b: vector<i8, 4, svbool_t> [c="fixed_bool"]) -> vector<i8, 4, svbool_t> [linkage=external] [abi=aapcs64(scalable, scalable) -> scalable] [fallthrough=ub_if_used] [c="fixed_bool(fixed_bool, fixed_bool)"] {
// BITS256-NEXT:         return and<vector<i8, 4, svbool_t>>(read<vector<i8, 4, svbool_t>>(%[[VALUE_a_3]]) [c="fixed_bool"], read<vector<i8, 4, svbool_t>>(%[[VALUE_b_2]]) [c="fixed_bool"]) [c="fixed_bool"];
// BITS256-NEXT:     }
// BITS256-NEXT:     fn %[[VALUE_compare:[0-9]+]] @compare(%[[VALUE_a_4:[0-9]+]] a: vector<f32, 8, svfloat32_t> [c="fixed_f32"], %[[VALUE_b_3:[0-9]+]] b: vector<f32, 8, svfloat32_t> [c="fixed_f32"]) -> vector<i32, 8> [linkage=external] [abi=aapcs64(scalable, scalable) -> sret<align=16>] [fallthrough=ub_if_used] [c="gnu_i32(fixed_f32, fixed_f32)"] {
// BITS256-NEXT:         return lt<vector<f32, 8, svfloat32_t>, result=vector<i32, 8>>(read<vector<f32, 8, svfloat32_t>>(%[[VALUE_a_4]]) [c="fixed_f32"], read<vector<f32, 8, svfloat32_t>>(%[[VALUE_b_3]]) [c="fixed_f32"]) [c="gnu_i32"];
// BITS256-NEXT:     }
// BITS256-NEXT:     fn %[[VALUE_to_sizeless:[0-9]+]] @to_sizeless(%[[VALUE_a_5:[0-9]+]] a: vector<i32, 8, svint32_t> [c="fixed_i32"]) -> sizeless<svint32_t> [linkage=external] [abi=aapcs64(scalable) -> scalar] [fallthrough=ub_if_used] [c="svint32_t(fixed_i32)"] {
// BITS256-NEXT:         return vector_bit_cast<sizeless<svint32_t>>(read<vector<i32, 8, svint32_t>>(%[[VALUE_a_5]]) [c="fixed_i32"]);
// BITS256-NEXT:     }
// BITS256-NEXT:     fn %[[VALUE_from_sizeless:[0-9]+]] @from_sizeless(%[[VALUE_a_6:[0-9]+]] a: sizeless<svint32_t>) -> vector<i32, 8, svint32_t> [linkage=external] [abi=aapcs64(scalar) -> scalable] [fallthrough=ub_if_used] [c="fixed_i32(svint32_t)"] {
// BITS256-NEXT:         return vector_bit_cast<vector<i32, 8, svint32_t>>(read<sizeless<svint32_t>>(%[[VALUE_a_6]])) [c="fixed_i32"];
// BITS256-NEXT:     }
// BITS256-NEXT:     fn %[[VALUE_to_gnu:[0-9]+]] @to_gnu(%[[VALUE_a_7:[0-9]+]] a: vector<i32, 8, svint32_t> [c="fixed_i32"]) -> vector<i32, 8> [linkage=external] [abi=aapcs64(scalable) -> sret<align=16>] [fallthrough=ub_if_used] [c="gnu_i32(fixed_i32)"] {
// BITS256-NEXT:         return vector_bit_cast<vector<i32, 8>>(read<vector<i32, 8, svint32_t>>(%[[VALUE_a_7]]) [c="fixed_i32"]) [c="gnu_i32"];
// BITS256-NEXT:     }
// BITS256-NEXT:     fn %[[VALUE_from_gnu:[0-9]+]] @from_gnu(%[[VALUE_a_8:[0-9]+]] a: vector<i32, 8> [c="gnu_i32"]) -> vector<i32, 8, svint32_t> [linkage=external] [abi=aapcs64(byref<align=16>) -> scalable] [fallthrough=ub_if_used] [c="fixed_i32(gnu_i32)"] {
// BITS256-NEXT:         return vector_bit_cast<vector<i32, 8, svint32_t>>(read<vector<i32, 8>>(%[[VALUE_a_8]]) [c="gnu_i32"]) [c="fixed_i32"];
// BITS256-NEXT:     }
// BITS256-NEXT:     fn %[[VALUE_reinterpret:[0-9]+]] @reinterpret(%[[VALUE_a_9:[0-9]+]] a: vector<u32, 8, svuint32_t> [c="fixed_u32"]) -> vector<i32, 8, svint32_t> [linkage=external] [abi=aapcs64(scalable) -> scalable] [fallthrough=ub_if_used] [c="fixed_i32(fixed_u32)"] {
// BITS256-NEXT:         return vector_bit_cast<vector<i32, 8, svint32_t>>(read<vector<u32, 8, svuint32_t>>(%[[VALUE_a_9]]) [c="fixed_u32"]) [c="fixed_i32"];
// BITS256-NEXT:     }
// BITS256-NEXT:     fn %[[VALUE_half_lane:[0-9]+]] @half_lane(%[[VALUE_a_10:[0-9]+]] a: vector<f16, 16, svfloat16_t> [c="fixed_f16"], %[[VALUE_b_4:[0-9]+]] b: vector<bf16, 16, svbfloat16_t> [c="fixed_bf16"], %[[VALUE_c:[0-9]+]] c: vector<u8, 32, svmfloat8_t> [c="fixed_mf8"], %[[VALUE_d:[0-9]+]] d: vector<u16, 16, svuint16_t> [c="fixed_u16"], %[[VALUE_e:[0-9]+]] e: vector<i64, 4, svint64_t> [c="fixed_i64"]) -> f32 [linkage=external] [abi=aapcs64(scalable, scalable, scalable, scalable, scalable) -> scalar] [fallthrough=ub_if_used] [c="float(fixed_f16, fixed_bf16, fixed_mf8, fixed_u16, fixed_i64)"] {
// BITS256-NEXT:         return float_widen<f32>(add<f16>(add<f16>(add<f16>(add<f16>(read<f16>(lane(%[[VALUE_a_10]], const<i32>(0))), float_narrow<f16>(read<bf16>(lane(%[[VALUE_b_4]], const<i32>(1))))), int_to_float<f16>(reinterpret<i32>(widen<u32>(read<u8>(lane(%[[VALUE_c]], const<i32>(2))))))), int_to_float<f16>(reinterpret<i32>(widen<u32>(read<u16>(lane(%[[VALUE_d]], const<i32>(3))))))), int_to_float<f16>(read<i64>(lane(%[[VALUE_e]], const<i32>(0))))));
// BITS256-NEXT:     }
// BITS256-NEXT: }
// SLATE-FILECHECK-END BITS256
