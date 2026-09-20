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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 v4si = vector<i32, 4>;
// IR-NEXT:     type @type1 v4su = vector<u32, 4>;
// IR-NEXT:     type @type2 v4sf = vector<f32, 4>;
// IR-NEXT:     type @type3 v2df = vector<f64, 2>;
// IR-NEXT:     type @type4 v3ss = vector<i16, 3>;
// IR-NEXT:     type @type5 holder = struct {
// IR-NEXT:         field0 value: vector<i32, 4>;
// IR-NEXT:         field1 tag: i32;
// IR-NEXT:     } [size=32, align=16, offsets=[0, 16]];
// IR-NEXT:     global %5 global_vector: vector<i32, 4> [storage=static] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4)) [linkage=external];
// IR-NEXT:     global %6 partial_vector: vector<i32, 4> [storage=static] = aggregate<vector<i32, 4>, zero_fill=true>(index0 = const<i32>(1)) [linkage=external];
// IR-NEXT:     global %7 sizes: array<u64, 5> [storage=static] = aggregate<array<u64, 5>, zero_fill=false>(index0 = const<u64>(16), index1 = const<u64>(16), index2 = const<u64>(8), index3 = const<u64>(8), index4 = const<u64>(16)) [linkage=external];
// IR-NEXT:     fn %9 @holder_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(32);
// IR-NEXT:     }
// IR-NEXT:     fn %10 @add(%11 a: vector<i32, 4>, %12 b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%11), read<vector<i32, 4>>(%12));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @divide(%14 a: vector<i32, 4>, %15 b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<vector<i32, 4>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i32, 4>>(%14), read<vector<i32, 4>>(%15));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @modulo(%17 a: vector<i32, 4>, %18 b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return rem<vector<i32, 4>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i32, 4>>(%17), read<vector<i32, 4>>(%18));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @bits(%20 a: vector<i32, 4>, %21 b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return or<vector<i32, 4>, elementwise=true>(and<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%20), read<vector<i32, 4>>(%21)), xor<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%20), read<vector<i32, 4>>(%21)));
// IR-NEXT:     }
// IR-NEXT:     fn %22 @shifts(%23 a: vector<i32, 4>, %24 b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return shr<vector<i32, 4>, elementwise=true, amount_out_of_range=ub, fill=sign_extend>(shl<vector<i32, 4>, elementwise=true, overflow=wrap, amount_out_of_range=ub, negative_left=ub>(read<vector<i32, 4>>(%23), read<vector<i32, 4>>(%24)), vector_splat<vector<i32, 4>, reason=usual_arith>(const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT:     fn %25 @unsigned_shift(%26 a: vector<u32, 4>) -> vector<u32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return shr<vector<u32, 4>, elementwise=true, amount_out_of_range=ub, fill=zero_extend>(read<vector<u32, 4>>(%26), vector_splat<vector<u32, 4>, reason=usual_arith>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT:     fn %27 @float_arithmetic(%28 a: vector<f32, 4>, %29 b: vector<f32, 4>) -> vector<f32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return sub<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore>(mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore>(read<vector<f32, 4>>(%28), read<vector<f32, 4>>(%29)), read<vector<f32, 4>>(%29));
// IR-NEXT:     }
// IR-NEXT:     fn %30 @splat_integer(%31 a: vector<i32, 4>, %32 b: i32) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, scalar) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%31), vector_splat<vector<i32, 4>, reason=usual_arith>(read<i32>(%32)));
// IR-NEXT:     }
// IR-NEXT:     fn %33 @splat_floating(%34 a: vector<f64, 2>) -> vector<f64, 2> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<vector<f64, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore>(read<vector<f64, 2>>(%34), vector_splat<vector<f64, 2>, reason=usual_arith>(const<f64>(2.0)));
// IR-NEXT:     }
// IR-NEXT:     fn %35 @splat_converting(%36 a: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%36), vector_splat<vector<i32, 4>, reason=usual_arith>(float_to_int<i32, reason=usual_arith, out_of_range=ub, exceptions=ignore>(const<f32>(1.0))));
// IR-NEXT:     }
// IR-NEXT:     fn %37 @equal(%38 a: vector<i32, 4>, %39 b: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return eq<vector<i32, 4>, result=vector<i32, 4>>(read<vector<i32, 4>>(%38), read<vector<i32, 4>>(%39));
// IR-NEXT:     }
// IR-NEXT:     fn %40 @float_compare(%41 a: vector<f32, 4>, %42 b: vector<f32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return lt<vector<f32, 4>, result=vector<i32, 4>, exceptions=ignore>(read<vector<f32, 4>>(%41), read<vector<f32, 4>>(%42));
// IR-NEXT:     }
// IR-NEXT:     fn %43 @wide_compare(%44 a: vector<f64, 2>, %45 b: vector<f64, 2>) -> vector<f64, 2> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return vector_bit_cast<vector<f64, 2>, reason=return>(ne<vector<f64, 2>, result=vector<i64, 2>, exceptions=ignore>(read<vector<f64, 2>>(%44), read<vector<f64, 2>>(%45)));
// IR-NEXT:     }
// IR-NEXT:     fn %46 @negate(%47 a: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%47));
// IR-NEXT:     }
// IR-NEXT:     fn %48 @complement(%49 a: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%49));
// IR-NEXT:     }
// IR-NEXT:     fn %50 @float_negate(%51 a: vector<f32, 4>) -> vector<f32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<vector<f32, 4>, elementwise=true>(read<vector<f32, 4>>(%51));
// IR-NEXT:     }
// IR-NEXT:     fn %52 @lax_assign(%53 a: vector<f32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return vector_bit_cast<vector<i32, 4>, reason=return>(read<vector<f32, 4>>(%53));
// IR-NEXT:     }
// IR-NEXT:     fn %54 @explicit_cast(%55 a: vector<u32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return vector_bit_cast<vector<i32, 4>, reason=explicit>(read<vector<u32, 4>>(%55));
// IR-NEXT:     }
// IR-NEXT:     fn %56 @compound(%57 a: ptr<vector<i32, 4>>, %58 b: vector<i32, 4>) -> void [linkage=external] [abi=sysv64(scalar, direct) -> void] [fallthrough=ret_void] {
// IR-NEXT:         let %82: ptr<vector<i32, 4>> [synthetic] = read<ptr<vector<i32, 4>>>(%57);
// IR-NEXT:         let %83: vector<i32, 4> [synthetic] = read<vector<i32, 4>>(deref(read<ptr<vector<i32, 4>>>(%82)));
// IR-NEXT:         let %84: vector<i32, 4> [synthetic] = add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%83), read<vector<i32, 4>>(%58));
// IR-NEXT:         write<vector<i32, 4>>(deref(read<ptr<vector<i32, 4>>>(%82)), read<vector<i32, 4>>(%84));
// IR-NEXT:     }
// IR-NEXT:     fn %59 @select(%60 a: vector<i32, 4>, %61 b: vector<i32, 4>, %62 c: i32) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct, scalar) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<vector<i32, 4>>(ne<i32>(read<i32>(%62), const<i32>(0)), read<vector<i32, 4>>(%60), read<vector<i32, 4>>(%61));
// IR-NEXT:     }
// IR-NEXT:     fn %63 @local() -> vector<i32, 4> [linkage=external] [abi=sysv64() -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         let %64 value: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4));
// IR-NEXT:         return read<vector<i32, 4>>(%64);
// IR-NEXT:     }
// IR-NEXT:     fn %65 @odd_lanes(%66 a: vector<i16, 3>, %67 b: vector<i16, 3>) -> vector<i16, 3> [linkage=external] [abi=sysv64(coerce<f64>, coerce<f64>) -> coerce<f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<vector<i16, 3>, elementwise=true, overflow=wrap>(read<vector<i16, 3>>(%66), read<vector<i16, 3>>(%67));
// IR-NEXT:     }
// IR-NEXT:     fn %68 @read_lane(%69 a: vector<i32, 4>, %70 i: i32) -> i32 [linkage=external] [abi=sysv64(direct, scalar) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(read<i32>(lane(%69, const<i32>(0))), read<i32>(lane(%69, read<i32>(%70))));
// IR-NEXT:     }
// IR-NEXT:     fn %71 @write_lane(%72 a: ptr<vector<i32, 4>>, %73 i: i32, %74 value: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<i32>(lane(deref(read<ptr<vector<i32, 4>>>(%72)), read<i32>(%73)), read<i32>(%74));
// IR-NEXT:         let %85: ptr<vector<i32, 4>> [synthetic] = read<ptr<vector<i32, 4>>>(%72);
// IR-NEXT:         let %86: i32 [synthetic] = const<i32>(0);
// IR-NEXT:         let %87: i32 [synthetic] = read<i32>(lane(deref(read<ptr<vector<i32, 4>>>(%85)), read<i32>(%86)));
// IR-NEXT:         let %88: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%87), read<i32>(%74));
// IR-NEXT:         write<i32>(lane(deref(read<ptr<vector<i32, 4>>>(%85)), read<i32>(%86)), read<i32>(%88));
// IR-NEXT:     }
// IR-NEXT:     fn %75 @value_lane(%76 a: vector<i32, 4>, %77 b: vector<i32, 4>, %78 i: i32) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct, direct, scalar) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         let %79 result: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = lane<i32>(add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%76), read<vector<i32, 4>>(%77)), read<i32>(%78)), index1 = read<i32>(lane(%76, const<i32>(1))), index2 = read<i32>(lane(%77, const<i32>(2))), index3 = const<i32>(0));
// IR-NEXT:         return read<vector<i32, 4>>(%79);
// IR-NEXT:     }
// IR-NEXT:     fn %80 @extended_lane(%81 a: vector<f32, 4>) -> f32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<f32>(lane(%81, const<i32>(3)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
