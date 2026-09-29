// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

_Fract fract;
_Accum accum;
unsigned _Fract unsigned_fract;
unsigned _Accum unsigned_accum;
_Sat short _Fract saturating_short_fract;
_Sat long _Accum saturating_long_accum;

unsigned long sizes[8] = {
    sizeof(short _Fract),      _Alignof(short _Fract),
    sizeof(_Accum),            _Alignof(_Accum),
    sizeof(long _Accum),       _Alignof(long _Accum),
    sizeof(long long _Accum),  _Alignof(long long _Accum),
};

struct fields {
    _Fract f;
    _Sat unsigned _Accum a;
};

_Accum add(_Accum a, _Accum b) {
    return a + b;
}

_Accum divide(_Accum a, _Accum b) {
    return a / b;
}

_Accum negate(_Accum a) {
    return -a;
}

_Sat _Accum saturating_add(_Sat _Accum a, _Sat _Accum b) {
    return a + b;
}

_Sat _Accum saturating_mul(_Sat _Accum a, _Sat _Accum b) {
    return a * b;
}

_Sat _Accum saturating_divide(_Sat _Accum a, _Sat _Accum b) {
    return a / b;
}

_Sat _Accum saturating_negate(_Sat _Accum a) {
    return -a;
}

// a saturating operand makes the common type saturating, and the wider kind
// and rank win
_Sat _Accum common_type(_Fract f, _Sat long _Accum a) {
    return f + a;
}

_Accum unsigned_common_type(unsigned _Accum u, _Accum s) {
    return u + s;
}

_Accum from_integer(int n) {
    return n;
}

_Accum from_floating(double d) {
    return d;
}

int to_integer(_Accum a) {
    return a;
}

double to_floating(_Accum a) {
    return a;
}

_Fract narrow(_Accum a) {
    return a;
}

_Accum widen(_Fract f) {
    return f;
}

_Accum explicit_casts(double d, int n, _Accum a) {
    return (_Accum)d + (_Accum)n + (_Accum)(_Sat _Fract)a;
}

_Accum scale_by_integer(_Accum a, int n) {
    return a * n;
}

double promote_to_double(_Accum a, double d) {
    return a + d;
}

int relational(_Accum a, _Accum b) {
    return a < b;
}

int equality(_Sat _Accum a, _Sat _Accum b) {
    return a == b;
}

_Accum shifts(_Accum a, int n) {
    return (a << n) + (a >> 1);
}

int truth(_Accum a, _Accum b) {
    if (a) {
        return 1;
    }
    return !b && a;
}

_Accum compound(_Accum a) {
    a += 2;
    a *= a;
    a -= a;
    return a;
}

_Sat _Accum saturating_compound(_Sat _Accum a, int n) {
    a /= n;
    return a;
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
// IR-NEXT:     type @type[[TYPE_fields:[0-9]+]] fields = struct {
// IR-NEXT:         field0 f: fixed<i16, 15>;
// IR-NEXT:         field1 a: sat_fixed<u32, 16>;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     global %[[VALUE_fract:[0-9]+]] fract: fixed<i16, 15> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_accum:[0-9]+]] accum: fixed<i32, 15> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_unsigned_fract:[0-9]+]] unsigned_fract: fixed<u16, 16> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_unsigned_accum:[0-9]+]] unsigned_accum: fixed<u32, 16> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_saturating_short_fract:[0-9]+]] saturating_short_fract: sat_fixed<i8, 7> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_saturating_long_accum:[0-9]+]] saturating_long_accum: sat_fixed<i64, 31> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: array<u64, 8> [storage=static] [align=16] = aggregate<array<u64, 8>, zero_fill=false>(index0 = const<u64>(1), index1 = const<u64>(1), index2 = const<u64>(4), index3 = const<u64>(4), index4 = const<u64>(8), index5 = const<u64>(8), index6 = const<u64>(16), index7 = const<u64>(16)) [linkage=external];
// IR-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: fixed<i32, 15>, %[[VALUE_b:[0-9]+]] b: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%[[VALUE_a]]), read<fixed<i32, 15>>(%[[VALUE_b]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: fixed<i32, 15>, %[[VALUE_b_2:[0-9]+]] b: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<fixed<i32, 15>, overflow=ub, rounding=toward_zero, by_zero=ub>(read<fixed<i32, 15>>(%[[VALUE_a_2]]), read<fixed<i32, 15>>(%[[VALUE_b_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_negate:[0-9]+]] @negate(%[[VALUE_a_3:[0-9]+]] a: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%[[VALUE_a_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_saturating_add:[0-9]+]] @saturating_add(%[[VALUE_a_4:[0-9]+]] a: sat_fixed<i32, 15>, %[[VALUE_b_3:[0-9]+]] b: sat_fixed<i32, 15>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero>(read<sat_fixed<i32, 15>>(%[[VALUE_a_4]]), read<sat_fixed<i32, 15>>(%[[VALUE_b_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_saturating_mul:[0-9]+]] @saturating_mul(%[[VALUE_a_5:[0-9]+]] a: sat_fixed<i32, 15>, %[[VALUE_b_4:[0-9]+]] b: sat_fixed<i32, 15>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero>(read<sat_fixed<i32, 15>>(%[[VALUE_a_5]]), read<sat_fixed<i32, 15>>(%[[VALUE_b_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_saturating_divide:[0-9]+]] @saturating_divide(%[[VALUE_a_6:[0-9]+]] a: sat_fixed<i32, 15>, %[[VALUE_b_5:[0-9]+]] b: sat_fixed<i32, 15>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero, by_zero=ub>(read<sat_fixed<i32, 15>>(%[[VALUE_a_6]]), read<sat_fixed<i32, 15>>(%[[VALUE_b_5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_saturating_negate:[0-9]+]] @saturating_negate(%[[VALUE_a_7:[0-9]+]] a: sat_fixed<i32, 15>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero>(read<sat_fixed<i32, 15>>(%[[VALUE_a_7]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_common_type:[0-9]+]] @common_type(%[[VALUE_f:[0-9]+]] f: fixed<i16, 15>, %[[VALUE_a_8:[0-9]+]] a: sat_fixed<i64, 31>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_convert<sat_fixed<i32, 15>, reason=return, overflow=saturate, rounding=toward_zero>(add<sat_fixed<i64, 31>, overflow=saturate, rounding=toward_zero>(fixed_convert<sat_fixed<i64, 31>, reason=usual_arith>(read<fixed<i16, 15>>(%[[VALUE_f]])), read<sat_fixed<i64, 31>>(%[[VALUE_a_8]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_unsigned_common_type:[0-9]+]] @unsigned_common_type(%[[VALUE_u:[0-9]+]] u: fixed<u32, 16>, %[[VALUE_s:[0-9]+]] s: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(fixed_convert<fixed<i32, 15>, reason=usual_arith, overflow=ub, rounding=toward_zero>(read<fixed<u32, 16>>(%[[VALUE_u]])), read<fixed<i32, 15>>(%[[VALUE_s]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_from_integer:[0-9]+]] @from_integer(%[[VALUE_n:[0-9]+]] n: i32) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return int_to_fixed<fixed<i32, 15>, reason=return, overflow=ub, rounding=toward_zero>(read<i32>(%[[VALUE_n]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_from_floating:[0-9]+]] @from_floating(%[[VALUE_d:[0-9]+]] d: f64) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_to_fixed<fixed<i32, 15>, reason=return, overflow=ub, rounding=toward_zero>(read<f64>(%[[VALUE_d]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_to_integer:[0-9]+]] @to_integer(%[[VALUE_a_9:[0-9]+]] a: fixed<i32, 15>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_to_int<i32, reason=return, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%[[VALUE_a_9]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_to_floating:[0-9]+]] @to_floating(%[[VALUE_a_10:[0-9]+]] a: fixed<i32, 15>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_to_float<f64, reason=return, rounding=nearest_even, exceptions=ignore>(read<fixed<i32, 15>>(%[[VALUE_a_10]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_narrow:[0-9]+]] @narrow(%[[VALUE_a_11:[0-9]+]] a: fixed<i32, 15>) -> fixed<i16, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_convert<fixed<i16, 15>, reason=return, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%[[VALUE_a_11]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_f_2:[0-9]+]] f: fixed<i16, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_convert<fixed<i32, 15>, reason=return>(read<fixed<i16, 15>>(%[[VALUE_f_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_explicit_casts:[0-9]+]] @explicit_casts(%[[VALUE_d_2:[0-9]+]] d: f64, %[[VALUE_n_2:[0-9]+]] n: i32, %[[VALUE_a_12:[0-9]+]] a: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(float_to_fixed<fixed<i32, 15>, reason=explicit, overflow=ub, rounding=toward_zero>(read<f64>(%[[VALUE_d_2]])), int_to_fixed<fixed<i32, 15>, reason=explicit, overflow=ub, rounding=toward_zero>(read<i32>(%[[VALUE_n_2]]))), fixed_convert<fixed<i32, 15>, reason=explicit>(fixed_convert<sat_fixed<i16, 15>, reason=explicit, overflow=saturate, rounding=toward_zero>(read<fixed<i32, 15>>(%[[VALUE_a_12]]))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_scale_by_integer:[0-9]+]] @scale_by_integer(%[[VALUE_a_13:[0-9]+]] a: fixed<i32, 15>, %[[VALUE_n_3:[0-9]+]] n: i32) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%[[VALUE_a_13]]), int_to_fixed<fixed<i32, 15>, reason=usual_arith, overflow=ub, rounding=toward_zero>(read<i32>(%[[VALUE_n_3]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_promote_to_double:[0-9]+]] @promote_to_double(%[[VALUE_a_14:[0-9]+]] a: fixed<i32, 15>, %[[VALUE_d_3:[0-9]+]] d: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(fixed_to_float<f64, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(read<fixed<i32, 15>>(%[[VALUE_a_14]])), read<f64>(%[[VALUE_d_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_relational:[0-9]+]] @relational(%[[VALUE_a_15:[0-9]+]] a: fixed<i32, 15>, %[[VALUE_b_6:[0-9]+]] b: fixed<i32, 15>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(lt<fixed<i32, 15>>(read<fixed<i32, 15>>(%[[VALUE_a_15]]), read<fixed<i32, 15>>(%[[VALUE_b_6]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_equality:[0-9]+]] @equality(%[[VALUE_a_16:[0-9]+]] a: sat_fixed<i32, 15>, %[[VALUE_b_7:[0-9]+]] b: sat_fixed<i32, 15>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<sat_fixed<i32, 15>>(read<sat_fixed<i32, 15>>(%[[VALUE_a_16]]), read<sat_fixed<i32, 15>>(%[[VALUE_b_7]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_shifts:[0-9]+]] @shifts(%[[VALUE_a_17:[0-9]+]] a: fixed<i32, 15>, %[[VALUE_n_4:[0-9]+]] n: i32) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(shl<fixed<i32, 15>, overflow=ub, rounding=toward_zero, amount_out_of_range=ub>(read<fixed<i32, 15>>(%[[VALUE_a_17]]), read<i32>(%[[VALUE_n_4]])), shr<fixed<i32, 15>, overflow=ub, rounding=toward_zero, amount_out_of_range=ub>(read<fixed<i32, 15>>(%[[VALUE_a_17]]), const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_truth:[0-9]+]] @truth(%[[VALUE_a_18:[0-9]+]] a: fixed<i32, 15>, %[[VALUE_b_8:[0-9]+]] b: fixed<i32, 15>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<fixed<i32, 15>>(read<fixed<i32, 15>>(%[[VALUE_a_18]]), const<fixed<i32, 15>>(0))
// IR-NEXT:             {
// IR-NEXT:                 return const<i32>(1);
// IR-NEXT:             }
// IR-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(not<bool>(ne<fixed<i32, 15>>(read<fixed<i32, 15>>(%[[VALUE_b_8]]), const<fixed<i32, 15>>(0))), ne<fixed<i32, 15>>(read<fixed<i32, 15>>(%[[VALUE_a_18]]), const<fixed<i32, 15>>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_compound:[0-9]+]] @compound(%[[VALUE_a_19:[0-9]+]] a: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: fixed<i32, 15> [synthetic] = read<fixed<i32, 15>>(%[[VALUE_a_19]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: fixed<i32, 15> [synthetic] = add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%[[VALUE0]]), int_to_fixed<fixed<i32, 15>, reason=usual_arith, overflow=ub, rounding=toward_zero>(const<i32>(2)));
// IR-NEXT:         write<fixed<i32, 15>>(%[[VALUE_a_19]], read<fixed<i32, 15>>(%[[VALUE1]]));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: fixed<i32, 15> [synthetic] = read<fixed<i32, 15>>(%[[VALUE_a_19]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: fixed<i32, 15> [synthetic] = mul<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%[[VALUE2]]), read<fixed<i32, 15>>(%[[VALUE_a_19]]));
// IR-NEXT:         write<fixed<i32, 15>>(%[[VALUE_a_19]], read<fixed<i32, 15>>(%[[VALUE3]]));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: fixed<i32, 15> [synthetic] = read<fixed<i32, 15>>(%[[VALUE_a_19]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: fixed<i32, 15> [synthetic] = sub<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%[[VALUE4]]), read<fixed<i32, 15>>(%[[VALUE_a_19]]));
// IR-NEXT:         write<fixed<i32, 15>>(%[[VALUE_a_19]], read<fixed<i32, 15>>(%[[VALUE5]]));
// IR-NEXT:         return read<fixed<i32, 15>>(%[[VALUE_a_19]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_saturating_compound:[0-9]+]] @saturating_compound(%[[VALUE_a_20:[0-9]+]] a: sat_fixed<i32, 15>, %[[VALUE_n_5:[0-9]+]] n: i32) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: sat_fixed<i32, 15> [synthetic] = read<sat_fixed<i32, 15>>(%[[VALUE_a_20]]);
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: sat_fixed<i32, 15> [synthetic] = div<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero, by_zero=ub>(read<sat_fixed<i32, 15>>(%[[VALUE6]]), int_to_fixed<sat_fixed<i32, 15>, reason=usual_arith, overflow=saturate, rounding=toward_zero>(read<i32>(%[[VALUE_n_5]])));
// IR-NEXT:         write<sat_fixed<i32, 15>>(%[[VALUE_a_20]], read<sat_fixed<i32, 15>>(%[[VALUE7]]));
// IR-NEXT:         return read<sat_fixed<i32, 15>>(%[[VALUE_a_20]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
