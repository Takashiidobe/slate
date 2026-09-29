// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef int v4si __attribute__((vector_size(16)));
typedef unsigned v4su __attribute__((vector_size(16)));
typedef float v4sf __attribute__((ext_vector_type(4)));
typedef double v2df __attribute__((vector_size(16)));
typedef short v3ss __attribute__((vector_size(6)));

v4si global_vector = {1, 2, 3, 4};
v4si partial_vector = {1};

unsigned long sizes[] = {
    sizeof(v4si),
    _Alignof(v4si),
    sizeof(v3ss),
    _Alignof(v3ss),
    sizeof(v4sf),
};

struct holder {
    v4si value;
    int tag;
};

unsigned long holder_size(void) {
    return sizeof(struct holder);
}

v4si add(v4si a, v4si b) {
    return a + b;
}

v4si divide(v4si a, v4si b) {
    return a / b;
}

v4si modulo(v4si a, v4si b) {
    return a % b;
}

v4si bits(v4si a, v4si b) {
    return (a & b) | (a ^ b);
}

v4si shifts(v4si a, v4si b) {
    return (a << b) >> 1;
}

v4su unsigned_shift(v4su a) {
    return a >> 1;
}

v4sf float_arithmetic(v4sf a, v4sf b) {
    return a * b - b;
}

v4si splat_integer(v4si a, int b) {
    return a * b;
}

v2df splat_floating(v2df a) {
    return a / 2.0;
}

v4si splat_converting(v4si a) {
    return a + 1.0f;
}

v4si equal(v4si a, v4si b) {
    return a == b;
}

v4si float_compare(v4sf a, v4sf b) {
    return a < b;
}

v2df wide_compare(v2df a, v2df b) {
    return a != b;
}

v4si negate(v4si a) {
    return -a;
}

v4si complement(v4si a) {
    return ~a;
}

v4sf float_negate(v4sf a) {
    return -a;
}

v4si lax_assign(v4sf a) {
    return a;
}

v4si explicit_cast(v4su a) {
    return (v4si)a;
}

void compound(v4si *a, v4si b) {
    *a += b;
}

v4si select(v4si a, v4si b, int c) {
    return c ? a : b;
}

v4si local(void) {
    v4si value = {1, 2, 3, 4};
    return value;
}

v3ss odd_lanes(v3ss a, v3ss b) {
    return a + b;
}

int read_lane(v4si a, int i) {
    return a[0] + a[i];
}

void write_lane(v4si *a, int i, int value) {
    (*a)[i] = value;
    (*a)[0] += value;
}

v4si value_lane(v4si a, v4si b, int i) {
    v4si result = {(a + b)[i], a[1], b[2], 0};
    return result;
}

float extended_lane(v4sf a) {
    return a[3];
}

typedef float v2sf __attribute__((ext_vector_type(2)));

float component(v4sf a) {
    return a.x + a.w;
}

v2sf halves(v4sf a) {
    return a.lo + a.hi;
}

v2sf duplicated_components(v4sf a) {
    return a.xx;
}

v2sf named_components(v4sf a) {
    return a.s13 + a.even;
}

void assign_components(v4sf *a, v2sf b) {
    (*a).x  = 1.0f;
    (*a).zw = b;
}

v4si reverse(v4si a, v4si b) {
    return __builtin_shufflevector(a, b, 3, 2, 5, -1);
}

v2sf narrow_shuffle(v4sf a) {
    return __builtin_shufflevector(a, a, 0, 3);
}

v4sf dynamic_shuffle(v4sf a, v4si mask) {
    return __builtin_shufflevector(a, mask);
}

v4sf convert_to_float(v4si a) {
    return __builtin_convertvector(a, v4sf);
}

v4si convert_to_int(v4sf a) {
    return __builtin_convertvector(a, v4si);
}

v4su convert_signedness(v4si a) {
    return __builtin_convertvector(a, v4su);
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
// IR-NEXT:     type @type[[TYPE_v4si:[0-9]+]] v4si = vector<i32, 4>;
// IR-NEXT:     type @type[[TYPE_v4su:[0-9]+]] v4su = vector<u32, 4>;
// IR-NEXT:     type @type[[TYPE_v4sf:[0-9]+]] v4sf = vector<f32, 4>;
// IR-NEXT:     type @type[[TYPE_v2df:[0-9]+]] v2df = vector<f64, 2>;
// IR-NEXT:     type @type[[TYPE_v3ss:[0-9]+]] v3ss = vector<i16, 3>;
// IR-NEXT:     type @type[[TYPE_holder:[0-9]+]] holder = struct {
// IR-NEXT:         field0 value: vector<i32, 4>;
// IR-NEXT:         field1 tag: i32;
// IR-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// IR-NEXT:     type @type[[TYPE_v2sf:[0-9]+]] v2sf = vector<f32, 2>;
// IR-NEXT:     global %[[VALUE_global_vector:[0-9]+]] global_vector: vector<i32, 4> [storage=static] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4)) [linkage=external];
// IR-NEXT:     global %[[VALUE_partial_vector:[0-9]+]] partial_vector: vector<i32, 4> [storage=static] = aggregate<vector<i32, 4>, zero_fill=true>(index0 = const<i32>(1)) [linkage=external];
// IR-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: array<u64, 5> [storage=static] [align=16] = aggregate<array<u64, 5>, zero_fill=false>(index0 = const<u64>(16), index1 = const<u64>(16), index2 = const<u64>(8), index3 = const<u64>(8), index4 = const<u64>(16)) [linkage=external];
// IR-NEXT:     fn %[[VALUE_holder_size:[0-9]+]] @holder_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(32);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b:[0-9]+]] b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%[[VALUE_a]]), read<vector<i32, 4>>(%[[VALUE_b]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b_2:[0-9]+]] b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<vector<i32, 4>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i32, 4>>(%[[VALUE_a_2]]), read<vector<i32, 4>>(%[[VALUE_b_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_modulo:[0-9]+]] @modulo(%[[VALUE_a_3:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b_3:[0-9]+]] b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return rem<vector<i32, 4>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i32, 4>>(%[[VALUE_a_3]]), read<vector<i32, 4>>(%[[VALUE_b_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_bits:[0-9]+]] @bits(%[[VALUE_a_4:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b_4:[0-9]+]] b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return or<vector<i32, 4>, elementwise=true>(and<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_a_4]]), read<vector<i32, 4>>(%[[VALUE_b_4]])), xor<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_a_4]]), read<vector<i32, 4>>(%[[VALUE_b_4]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_shifts:[0-9]+]] @shifts(%[[VALUE_a_5:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b_5:[0-9]+]] b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return shr<vector<i32, 4>, elementwise=true, amount_out_of_range=ub, fill=sign_extend>(shl<vector<i32, 4>, elementwise=true, overflow=wrap, amount_out_of_range=ub, negative_left=ub>(read<vector<i32, 4>>(%[[VALUE_a_5]]), read<vector<i32, 4>>(%[[VALUE_b_5]])), vector_splat<vector<i32, 4>, reason=usual_arith>(const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unsigned_shift:[0-9]+]] @unsigned_shift(%[[VALUE_a_6:[0-9]+]] a: vector<u32, 4>) -> vector<u32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return shr<vector<u32, 4>, elementwise=true, amount_out_of_range=ub, fill=zero_extend>(read<vector<u32, 4>>(%[[VALUE_a_6]]), vector_splat<vector<u32, 4>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_float_arithmetic:[0-9]+]] @float_arithmetic(%[[VALUE_a_7:[0-9]+]] a: vector<f32, 4>, %[[VALUE_b_6:[0-9]+]] b: vector<f32, 4>) -> vector<f32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_a_7]]), read<vector<f32, 4>>(%[[VALUE_b_6]])), read<vector<f32, 4>>(%[[VALUE_b_6]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_splat_integer:[0-9]+]] @splat_integer(%[[VALUE_a_8:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b_7:[0-9]+]] b: i32) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, scalar) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%[[VALUE_a_8]]), vector_splat<vector<i32, 4>, reason=usual_arith>(read<i32>(%[[VALUE_b_7]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_splat_floating:[0-9]+]] @splat_floating(%[[VALUE_a_9:[0-9]+]] a: vector<f64, 2>) -> vector<f64, 2> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f64, 2>>(%[[VALUE_a_9]]), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_splat_converting:[0-9]+]] @splat_converting(%[[VALUE_a_10:[0-9]+]] a: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%[[VALUE_a_10]]), vector_splat<vector<i32, 4>, reason=usual_arith>(float_to_int<i32, reason=usual_arith, out_of_range=ub, exceptions=ignore>(const<f32>(1.0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_equal:[0-9]+]] @equal(%[[VALUE_a_11:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b_8:[0-9]+]] b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return eq<vector<i32, 4>, result=vector<i32, 4>>(read<vector<i32, 4>>(%[[VALUE_a_11]]), read<vector<i32, 4>>(%[[VALUE_b_8]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_float_compare:[0-9]+]] @float_compare(%[[VALUE_a_12:[0-9]+]] a: vector<f32, 4>, %[[VALUE_b_9:[0-9]+]] b: vector<f32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return lt<vector<f32, 4>, result=vector<i32, 4>, exceptions=ignore>(read<vector<f32, 4>>(%[[VALUE_a_12]]), read<vector<f32, 4>>(%[[VALUE_b_9]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_wide_compare:[0-9]+]] @wide_compare(%[[VALUE_a_13:[0-9]+]] a: vector<f64, 2>, %[[VALUE_b_10:[0-9]+]] b: vector<f64, 2>) -> vector<f64, 2> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return vector_bit_cast<vector<f64, 2>, reason=return>(ne<vector<f64, 2>, result=vector<i64, 2>, exceptions=ignore>(read<vector<f64, 2>>(%[[VALUE_a_13]]), read<vector<f64, 2>>(%[[VALUE_b_10]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_negate:[0-9]+]] @negate(%[[VALUE_a_14:[0-9]+]] a: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%[[VALUE_a_14]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complement:[0-9]+]] @complement(%[[VALUE_a_15:[0-9]+]] a: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%[[VALUE_a_15]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_float_negate:[0-9]+]] @float_negate(%[[VALUE_a_16:[0-9]+]] a: vector<f32, 4>) -> vector<f32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<vector<f32, 4>, elementwise=true>(read<vector<f32, 4>>(%[[VALUE_a_16]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_lax_assign:[0-9]+]] @lax_assign(%[[VALUE_a_17:[0-9]+]] a: vector<f32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return vector_bit_cast<vector<i32, 4>, reason=return>(read<vector<f32, 4>>(%[[VALUE_a_17]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_explicit_cast:[0-9]+]] @explicit_cast(%[[VALUE_a_18:[0-9]+]] a: vector<u32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return vector_bit_cast<vector<i32, 4>, reason=explicit>(read<vector<u32, 4>>(%[[VALUE_a_18]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_compound:[0-9]+]] @compound(%[[VALUE_a_19:[0-9]+]] a: ptr<vector<i32, 4>>, %[[VALUE_b_11:[0-9]+]] b: vector<i32, 4>) -> void [linkage=external] [abi=sysv64(scalar, direct) -> void] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<vector<i32, 4>> [synthetic] = read<ptr<vector<i32, 4>>>(%[[VALUE_a_19]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: vector<i32, 4> [synthetic] = read<vector<i32, 4>>(deref(read<ptr<vector<i32, 4>>>(%[[VALUE0]])));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: vector<i32, 4> [synthetic] = add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%[[VALUE1]]), read<vector<i32, 4>>(%[[VALUE_b_11]]));
// IR-NEXT:         write<vector<i32, 4>>(deref(read<ptr<vector<i32, 4>>>(%[[VALUE0]])), read<vector<i32, 4>>(%[[VALUE2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_select:[0-9]+]] @select(%[[VALUE_a_20:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b_12:[0-9]+]] b: vector<i32, 4>, %[[VALUE_c:[0-9]+]] c: i32) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct, scalar) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<vector<i32, 4>>(ne<i32>(read<i32>(%[[VALUE_c]]), const<i32>(0)), read<vector<i32, 4>>(%[[VALUE_a_20]]), read<vector<i32, 4>>(%[[VALUE_b_12]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_local:[0-9]+]] @local() -> vector<i32, 4> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_value:[0-9]+]] value: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4));
// IR-NEXT:         return read<vector<i32, 4>>(%[[VALUE_value]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_odd_lanes:[0-9]+]] @odd_lanes(%[[VALUE_a_21:[0-9]+]] a: vector<i16, 3>, %[[VALUE_b_13:[0-9]+]] b: vector<i16, 3>) -> vector<i16, 3> [linkage=external] [abi=sysv64(coerce<f64>, coerce<f64>) -> coerce<f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<vector<i16, 3>, elementwise=true, overflow=wrap>(read<vector<i16, 3>>(%[[VALUE_a_21]]), read<vector<i16, 3>>(%[[VALUE_b_13]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_read_lane:[0-9]+]] @read_lane(%[[VALUE_a_22:[0-9]+]] a: vector<i32, 4>, %[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [abi=sysv64(direct, scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(lane(%[[VALUE_a_22]], const<i32>(0))), read<i32>(lane(%[[VALUE_a_22]], read<i32>(%[[VALUE_i]]))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_write_lane:[0-9]+]] @write_lane(%[[VALUE_a_23:[0-9]+]] a: ptr<vector<i32, 4>>, %[[VALUE_i_2:[0-9]+]] i: i32, %[[VALUE_value_2:[0-9]+]] value: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<i32>(lane(deref(read<ptr<vector<i32, 4>>>(%[[VALUE_a_23]])), read<i32>(%[[VALUE_i_2]])), read<i32>(%[[VALUE_value_2]]));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<vector<i32, 4>> [synthetic] = read<ptr<vector<i32, 4>>>(%[[VALUE_a_23]]);
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = const<i32>(0);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(lane(deref(read<ptr<vector<i32, 4>>>(%[[VALUE3]])), read<i32>(%[[VALUE4]])));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), read<i32>(%[[VALUE_value_2]]));
// IR-NEXT:         write<i32>(lane(deref(read<ptr<vector<i32, 4>>>(%[[VALUE3]])), read<i32>(%[[VALUE4]])), read<i32>(%[[VALUE6]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_value_lane:[0-9]+]] @value_lane(%[[VALUE_a_24:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b_14:[0-9]+]] b: vector<i32, 4>, %[[VALUE_i_3:[0-9]+]] i: i32) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct, scalar) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_result:[0-9]+]] result: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = lane<i32>(add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%[[VALUE_a_24]]), read<vector<i32, 4>>(%[[VALUE_b_14]])), read<i32>(%[[VALUE_i_3]])), index1 = read<i32>(lane(%[[VALUE_a_24]], const<i32>(1))), index2 = read<i32>(lane(%[[VALUE_b_14]], const<i32>(2))), index3 = const<i32>(0));
// IR-NEXT:         return read<vector<i32, 4>>(%[[VALUE_result]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_extended_lane:[0-9]+]] @extended_lane(%[[VALUE_a_25:[0-9]+]] a: vector<f32, 4>) -> f32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<f32>(lane(%[[VALUE_a_25]], const<i32>(3)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_component:[0-9]+]] @component(%[[VALUE_a_26:[0-9]+]] a: vector<f32, 4>) -> f32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(lane(%[[VALUE_a_26]], const<i32>(0))), read<f32>(lane(%[[VALUE_a_26]], const<i32>(3))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_halves:[0-9]+]] @halves(%[[VALUE_a_27:[0-9]+]] a: vector<f32, 4>) -> vector<f32, 2> [linkage=external] [abi=sysv64(direct) -> coerce<f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<vector<f32, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 2>>(swizzle<lanes=[0, 1]>(%[[VALUE_a_27]])), read<vector<f32, 2>>(swizzle<lanes=[2, 3]>(%[[VALUE_a_27]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_duplicated_components:[0-9]+]] @duplicated_components(%[[VALUE_a_28:[0-9]+]] a: vector<f32, 4>) -> vector<f32, 2> [linkage=external] [abi=sysv64(direct) -> coerce<f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return shuffle<vector<f32, 2>, mask=[0, 0]>(read<vector<f32, 4>>(%[[VALUE_a_28]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_named_components:[0-9]+]] @named_components(%[[VALUE_a_29:[0-9]+]] a: vector<f32, 4>) -> vector<f32, 2> [linkage=external] [abi=sysv64(direct) -> coerce<f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<vector<f32, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 2>>(swizzle<lanes=[1, 3]>(%[[VALUE_a_29]])), read<vector<f32, 2>>(swizzle<lanes=[0, 2]>(%[[VALUE_a_29]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_assign_components:[0-9]+]] @assign_components(%[[VALUE_a_30:[0-9]+]] a: ptr<vector<f32, 4>>, %[[VALUE_b_15:[0-9]+]] b: vector<f32, 2>) -> void [linkage=external] [abi=sysv64(scalar, coerce<f64>) -> void] [fallthrough=ret_void] {
// IR-NEXT:         write<f32>(lane(deref(read<ptr<vector<f32, 4>>>(%[[VALUE_a_30]])), const<i32>(0)), const<f32>(1.0));
// IR-NEXT:         write<vector<f32, 2>>(swizzle<lanes=[2, 3]>(deref(read<ptr<vector<f32, 4>>>(%[[VALUE_a_30]]))), read<vector<f32, 2>>(%[[VALUE_b_15]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_reverse:[0-9]+]] @reverse(%[[VALUE_a_31:[0-9]+]] a: vector<i32, 4>, %[[VALUE_b_16:[0-9]+]] b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return shuffle<vector<i32, 4>, mask=[3, 2, 5, undef]>(read<vector<i32, 4>>(%[[VALUE_a_31]]), read<vector<i32, 4>>(%[[VALUE_b_16]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_narrow_shuffle:[0-9]+]] @narrow_shuffle(%[[VALUE_a_32:[0-9]+]] a: vector<f32, 4>) -> vector<f32, 2> [linkage=external] [abi=sysv64(direct) -> coerce<f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return shuffle<vector<f32, 2>, mask=[0, 3]>(read<vector<f32, 4>>(%[[VALUE_a_32]]), read<vector<f32, 4>>(%[[VALUE_a_32]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_dynamic_shuffle:[0-9]+]] @dynamic_shuffle(%[[VALUE_a_33:[0-9]+]] a: vector<f32, 4>, %[[VALUE_mask:[0-9]+]] mask: vector<i32, 4>) -> vector<f32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return shuffle<vector<f32, 4>, mask=dynamic(read<vector<i32, 4>>(%[[VALUE_mask]]))>(read<vector<f32, 4>>(%[[VALUE_a_33]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_convert_to_float:[0-9]+]] @convert_to_float(%[[VALUE_a_34:[0-9]+]] a: vector<i32, 4>) -> vector<f32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return int_to_float<vector<f32, 4>, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<vector<i32, 4>>(%[[VALUE_a_34]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_convert_to_int:[0-9]+]] @convert_to_int(%[[VALUE_a_35:[0-9]+]] a: vector<f32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_to_int<vector<i32, 4>, reason=explicit, out_of_range=ub, exceptions=ignore>(read<vector<f32, 4>>(%[[VALUE_a_35]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_convert_signedness:[0-9]+]] @convert_signedness(%[[VALUE_a_36:[0-9]+]] a: vector<i32, 4>) -> vector<u32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return reinterpret<vector<u32, 4>, reason=explicit, fits=unknown>(read<vector<i32, 4>>(%[[VALUE_a_36]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
