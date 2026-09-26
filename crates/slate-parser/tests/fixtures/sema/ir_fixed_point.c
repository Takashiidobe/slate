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
// IR-NEXT:     type @type0 fields = struct {
// IR-NEXT:         field0 f: fixed<i16, 15>;
// IR-NEXT:         field1 a: sat_fixed<u32, 16>;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     global %0 fract: fixed<i16, 15> [storage=static] [linkage=external];
// IR-NEXT:     global %1 accum: fixed<i32, 15> [storage=static] [linkage=external];
// IR-NEXT:     global %2 unsigned_fract: fixed<u16, 16> [storage=static] [linkage=external];
// IR-NEXT:     global %3 unsigned_accum: fixed<u32, 16> [storage=static] [linkage=external];
// IR-NEXT:     global %4 saturating_short_fract: sat_fixed<i8, 7> [storage=static] [linkage=external];
// IR-NEXT:     global %5 saturating_long_accum: sat_fixed<i64, 31> [storage=static] [linkage=external];
// IR-NEXT:     global %6 sizes: array<u64, 8> [storage=static] [align=16] = aggregate<array<u64, 8>, zero_fill=false>(index0 = const<u64>(1), index1 = const<u64>(1), index2 = const<u64>(4), index3 = const<u64>(4), index4 = const<u64>(8), index5 = const<u64>(8), index6 = const<u64>(16), index7 = const<u64>(16)) [linkage=external];
// IR-NEXT:     fn %8 @add(%9 a: fixed<i32, 15>, %10 b: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%9), read<fixed<i32, 15>>(%10));
// IR-NEXT:     }
// IR-NEXT:     fn %11 @divide(%12 a: fixed<i32, 15>, %13 b: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<fixed<i32, 15>, overflow=ub, rounding=toward_zero, by_zero=ub>(read<fixed<i32, 15>>(%12), read<fixed<i32, 15>>(%13));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @negate(%15 a: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%15));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @saturating_add(%17 a: sat_fixed<i32, 15>, %18 b: sat_fixed<i32, 15>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero>(read<sat_fixed<i32, 15>>(%17), read<sat_fixed<i32, 15>>(%18));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @saturating_mul(%20 a: sat_fixed<i32, 15>, %21 b: sat_fixed<i32, 15>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero>(read<sat_fixed<i32, 15>>(%20), read<sat_fixed<i32, 15>>(%21));
// IR-NEXT:     }
// IR-NEXT:     fn %22 @saturating_divide(%23 a: sat_fixed<i32, 15>, %24 b: sat_fixed<i32, 15>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return div<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero, by_zero=ub>(read<sat_fixed<i32, 15>>(%23), read<sat_fixed<i32, 15>>(%24));
// IR-NEXT:     }
// IR-NEXT:     fn %25 @saturating_negate(%26 a: sat_fixed<i32, 15>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero>(read<sat_fixed<i32, 15>>(%26));
// IR-NEXT:     }
// IR-NEXT:     fn %27 @common_type(%28 f: fixed<i16, 15>, %29 a: sat_fixed<i64, 31>) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_convert<sat_fixed<i32, 15>, reason=return, overflow=saturate, rounding=toward_zero>(add<sat_fixed<i64, 31>, overflow=saturate, rounding=toward_zero>(fixed_convert<sat_fixed<i64, 31>, reason=usual_arith>(read<fixed<i16, 15>>(%28)), read<sat_fixed<i64, 31>>(%29)));
// IR-NEXT:     }
// IR-NEXT:     fn %30 @unsigned_common_type(%31 u: fixed<u32, 16>, %32 s: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(fixed_convert<fixed<i32, 15>, reason=usual_arith, overflow=ub, rounding=toward_zero>(read<fixed<u32, 16>>(%31)), read<fixed<i32, 15>>(%32));
// IR-NEXT:     }
// IR-NEXT:     fn %33 @from_integer(%34 n: i32) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return int_to_fixed<fixed<i32, 15>, reason=return, overflow=ub, rounding=toward_zero>(read<i32>(%34));
// IR-NEXT:     }
// IR-NEXT:     fn %35 @from_floating(%36 d: f64) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_to_fixed<fixed<i32, 15>, reason=return, overflow=ub, rounding=toward_zero>(read<f64>(%36));
// IR-NEXT:     }
// IR-NEXT:     fn %37 @to_integer(%38 a: fixed<i32, 15>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_to_int<i32, reason=return, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%38));
// IR-NEXT:     }
// IR-NEXT:     fn %39 @to_floating(%40 a: fixed<i32, 15>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_to_float<f64, reason=return, rounding=nearest_even, exceptions=ignore>(read<fixed<i32, 15>>(%40));
// IR-NEXT:     }
// IR-NEXT:     fn %41 @narrow(%42 a: fixed<i32, 15>) -> fixed<i16, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_convert<fixed<i16, 15>, reason=return, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%42));
// IR-NEXT:     }
// IR-NEXT:     fn %43 @widen(%44 f: fixed<i16, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return fixed_convert<fixed<i32, 15>, reason=return>(read<fixed<i16, 15>>(%44));
// IR-NEXT:     }
// IR-NEXT:     fn %45 @explicit_casts(%46 d: f64, %47 n: i32, %48 a: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(float_to_fixed<fixed<i32, 15>, reason=explicit, overflow=ub, rounding=toward_zero>(read<f64>(%46)), int_to_fixed<fixed<i32, 15>, reason=explicit, overflow=ub, rounding=toward_zero>(read<i32>(%47))), fixed_convert<fixed<i32, 15>, reason=explicit>(fixed_convert<sat_fixed<i16, 15>, reason=explicit, overflow=saturate, rounding=toward_zero>(read<fixed<i32, 15>>(%48))));
// IR-NEXT:     }
// IR-NEXT:     fn %49 @scale_by_integer(%50 a: fixed<i32, 15>, %51 n: i32) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%50), int_to_fixed<fixed<i32, 15>, reason=usual_arith, overflow=ub, rounding=toward_zero>(read<i32>(%51)));
// IR-NEXT:     }
// IR-NEXT:     fn %52 @promote_to_double(%53 a: fixed<i32, 15>, %54 d: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(fixed_to_float<f64, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(read<fixed<i32, 15>>(%53)), read<f64>(%54));
// IR-NEXT:     }
// IR-NEXT:     fn %55 @relational(%56 a: fixed<i32, 15>, %57 b: fixed<i32, 15>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(lt<fixed<i32, 15>>(read<fixed<i32, 15>>(%56), read<fixed<i32, 15>>(%57)));
// IR-NEXT:     }
// IR-NEXT:     fn %58 @equality(%59 a: sat_fixed<i32, 15>, %60 b: sat_fixed<i32, 15>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(eq<sat_fixed<i32, 15>>(read<sat_fixed<i32, 15>>(%59), read<sat_fixed<i32, 15>>(%60)));
// IR-NEXT:     }
// IR-NEXT:     fn %61 @shifts(%62 a: fixed<i32, 15>, %63 n: i32) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(shl<fixed<i32, 15>, overflow=ub, rounding=toward_zero, amount_out_of_range=ub>(read<fixed<i32, 15>>(%62), read<i32>(%63)), shr<fixed<i32, 15>, overflow=ub, rounding=toward_zero, amount_out_of_range=ub>(read<fixed<i32, 15>>(%62), const<i32>(1)));
// IR-NEXT:     }
// IR-NEXT:     fn %64 @truth(%65 a: fixed<i32, 15>, %66 b: fixed<i32, 15>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         if ne<fixed<i32, 15>>(read<fixed<i32, 15>>(%65), const<fixed<i32, 15>>(0))
// IR-NEXT:             {
// IR-NEXT:                 return const<i32>(1);
// IR-NEXT:             }
// IR-NEXT:         return from_bool<i32, reason=return>(logical_and<bool>(not<bool>(ne<fixed<i32, 15>>(read<fixed<i32, 15>>(%66), const<fixed<i32, 15>>(0))), ne<fixed<i32, 15>>(read<fixed<i32, 15>>(%65), const<fixed<i32, 15>>(0))));
// IR-NEXT:     }
// IR-NEXT:     fn %67 @compound(%68 a: fixed<i32, 15>) -> fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %72: fixed<i32, 15> [synthetic] = read<fixed<i32, 15>>(%68);
// IR-NEXT:         let %73: fixed<i32, 15> [synthetic] = add<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%72), int_to_fixed<fixed<i32, 15>, reason=usual_arith, overflow=ub, rounding=toward_zero>(const<i32>(2)));
// IR-NEXT:         write<fixed<i32, 15>>(%68, read<fixed<i32, 15>>(%73));
// IR-NEXT:         let %74: fixed<i32, 15> [synthetic] = read<fixed<i32, 15>>(%68);
// IR-NEXT:         let %75: fixed<i32, 15> [synthetic] = mul<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%74), read<fixed<i32, 15>>(%68));
// IR-NEXT:         write<fixed<i32, 15>>(%68, read<fixed<i32, 15>>(%75));
// IR-NEXT:         let %76: fixed<i32, 15> [synthetic] = read<fixed<i32, 15>>(%68);
// IR-NEXT:         let %77: fixed<i32, 15> [synthetic] = sub<fixed<i32, 15>, overflow=ub, rounding=toward_zero>(read<fixed<i32, 15>>(%76), read<fixed<i32, 15>>(%68));
// IR-NEXT:         write<fixed<i32, 15>>(%68, read<fixed<i32, 15>>(%77));
// IR-NEXT:         return read<fixed<i32, 15>>(%68);
// IR-NEXT:     }
// IR-NEXT:     fn %69 @saturating_compound(%70 a: sat_fixed<i32, 15>, %71 n: i32) -> sat_fixed<i32, 15> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %78: sat_fixed<i32, 15> [synthetic] = read<sat_fixed<i32, 15>>(%70);
// IR-NEXT:         let %79: sat_fixed<i32, 15> [synthetic] = div<sat_fixed<i32, 15>, overflow=saturate, rounding=toward_zero, by_zero=ub>(read<sat_fixed<i32, 15>>(%78), int_to_fixed<sat_fixed<i32, 15>, reason=usual_arith, overflow=saturate, rounding=toward_zero>(read<i32>(%71)));
// IR-NEXT:         write<sat_fixed<i32, 15>>(%70, read<sat_fixed<i32, 15>>(%79));
// IR-NEXT:         return read<sat_fixed<i32, 15>>(%70);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
