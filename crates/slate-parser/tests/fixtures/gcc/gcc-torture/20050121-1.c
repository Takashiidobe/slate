/* PR middle-end/19551 */

extern void abort();

#define T(type, name)                                                          \
  __attribute__((pure)) _Complex type foo_##name(int x) {                      \
    _Complex type r;                                                           \
    __real r = x + 1;                                                          \
    __imag r = x - 1;                                                          \
    return r;                                                                  \
  }                                                                            \
                                                                               \
  void bar_##name(type *x) { *x = __real foo_##name(5); }                      \
                                                                               \
  void baz_##name(type *x) { *x = __imag foo_##name(5); }

typedef long double ldouble_t;
typedef long long   llong;

T(float, float)
T(double, double)
T(long double, ldouble_t)
T(char, char)
T(short, short)
T(int, int)
T(long, long)
T(long long, llong)
#undef T

int main(void) {
#define T(type, name)                                                          \
  {                                                                            \
    type var = 0;                                                              \
    bar_##name(&var);                                                          \
    if (var != 6)                                                              \
      abort();                                                                 \
    var = 0;                                                                   \
    baz_##name(&var);                                                          \
    if (var != 4)                                                              \
      abort();                                                                 \
  }
  T(float, float)
  T(double, double)
  T(long double, ldouble_t)
  T(char, char)
  T(short, short)
  T(int, int)
  T(long, long)
  T(long long, llong)
  return 0;
}



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
// DEFAULT-NEXT:     type @type0 ldouble_t = f80;
// DEFAULT-NEXT:     type @type1 llong = i64;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @foo_float(%4 x: i32) -> complex<f32> [linkage=external] [abi=sysv64(scalar) -> coerce<pair<f32>>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 r: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         write<f32>(real(%5), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(add<i32, overflow=ub>(read<i32>(%4), const<i32>(1))));
// DEFAULT-NEXT:         write<f32>(imag(%5), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<f32>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar_float(%7 x: ptr<f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(deref(read<ptr<f32>>(%7)), complex_to_real<f32, reason=explicit>(call<complex<f32>, signature=fn(i32) -> complex<f32>, abi=sysv64(scalar) -> coerce<pair<f32>>>(%3, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_real<f32, reason=explicit>(call<complex<f32>, signature=fn(i32) -> complex<f32>, abi=sysv64(scalar) -> coerce<pair<f32>>>(%3, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @baz_float(%9 x: ptr<f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(deref(read<ptr<f32>>(%9)), complex_to_imag<f32, reason=explicit>(call<complex<f32>, signature=fn(i32) -> complex<f32>, abi=sysv64(scalar) -> coerce<pair<f32>>>(%3, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_imag<f32, reason=explicit>(call<complex<f32>, signature=fn(i32) -> complex<f32>, abi=sysv64(scalar) -> coerce<pair<f32>>>(%3, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foo_double(%11 x: i32) -> complex<f64> [linkage=external] [abi=sysv64(scalar) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 r: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<f64>(real(%12), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(add<i32, overflow=ub>(read<i32>(%11), const<i32>(1))));
// DEFAULT-NEXT:         write<f64>(imag(%12), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(read<i32>(%11), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<f64>>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @bar_double(%14 x: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(deref(read<ptr<f64>>(%14)), complex_to_real<f64, reason=explicit>(call<complex<f64>, signature=fn(i32) -> complex<f64>, abi=sysv64(scalar) -> coerce<f64, f64>>(%10, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_real<f64, reason=explicit>(call<complex<f64>, signature=fn(i32) -> complex<f64>, abi=sysv64(scalar) -> coerce<f64, f64>>(%10, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @baz_double(%16 x: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(deref(read<ptr<f64>>(%16)), complex_to_imag<f64, reason=explicit>(call<complex<f64>, signature=fn(i32) -> complex<f64>, abi=sysv64(scalar) -> coerce<f64, f64>>(%10, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_imag<f64, reason=explicit>(call<complex<f64>, signature=fn(i32) -> complex<f64>, abi=sysv64(scalar) -> coerce<f64, f64>>(%10, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @foo_ldouble_t(%18 x: i32) -> complex<f80> [linkage=external] [abi=sysv64(scalar) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 r: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         write<f80>(real(%19), int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(add<i32, overflow=ub>(read<i32>(%18), const<i32>(1))));
// DEFAULT-NEXT:         write<f80>(imag(%19), int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(read<i32>(%18), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<f80>>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @bar_ldouble_t(%21 x: ptr<f80>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f80>(deref(read<ptr<f80>>(%21)), complex_to_real<f80, reason=explicit>(call<complex<f80>, signature=fn(i32) -> complex<f80>, abi=sysv64(scalar) -> coerce<f80, f80>>(%17, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_real<f80, reason=explicit>(call<complex<f80>, signature=fn(i32) -> complex<f80>, abi=sysv64(scalar) -> coerce<f80, f80>>(%17, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @baz_ldouble_t(%23 x: ptr<f80>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f80>(deref(read<ptr<f80>>(%23)), complex_to_imag<f80, reason=explicit>(call<complex<f80>, signature=fn(i32) -> complex<f80>, abi=sysv64(scalar) -> coerce<f80, f80>>(%17, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_imag<f80, reason=explicit>(call<complex<f80>, signature=fn(i32) -> complex<f80>, abi=sysv64(scalar) -> coerce<f80, f80>>(%17, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @foo_char(%25 x: i32) -> complex<i8> [linkage=external] [abi=sysv64(scalar) -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26 r: complex<i8> [storage=automatic];
// DEFAULT-NEXT:         write<i8>(real(%26), truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%25), const<i32>(1))));
// DEFAULT-NEXT:         write<i8>(imag(%26), truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(read<i32>(%25), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i8>>(%26);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @bar_char(%28 x: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%28)), complex_to_real<i8, reason=explicit>(call<complex<i8>, signature=fn(i32) -> complex<i8>, abi=sysv64(scalar) -> coerce<i64>>(%24, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_real<i8, reason=explicit>(call<complex<i8>, signature=fn(i32) -> complex<i8>, abi=sysv64(scalar) -> coerce<i64>>(%24, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @baz_char(%30 x: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%30)), complex_to_imag<i8, reason=explicit>(call<complex<i8>, signature=fn(i32) -> complex<i8>, abi=sysv64(scalar) -> coerce<i64>>(%24, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_imag<i8, reason=explicit>(call<complex<i8>, signature=fn(i32) -> complex<i8>, abi=sysv64(scalar) -> coerce<i64>>(%24, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @foo_short(%32 x: i32) -> complex<i16> [linkage=external] [abi=sysv64(scalar) -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 r: complex<i16> [storage=automatic];
// DEFAULT-NEXT:         write<i16>(real(%33), truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%32), const<i32>(1))));
// DEFAULT-NEXT:         write<i16>(imag(%33), truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(read<i32>(%32), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i16>>(%33);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @bar_short(%35 x: ptr<i16>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(deref(read<ptr<i16>>(%35)), complex_to_real<i16, reason=explicit>(call<complex<i16>, signature=fn(i32) -> complex<i16>, abi=sysv64(scalar) -> coerce<i64>>(%31, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_real<i16, reason=explicit>(call<complex<i16>, signature=fn(i32) -> complex<i16>, abi=sysv64(scalar) -> coerce<i64>>(%31, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @baz_short(%37 x: ptr<i16>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(deref(read<ptr<i16>>(%37)), complex_to_imag<i16, reason=explicit>(call<complex<i16>, signature=fn(i32) -> complex<i16>, abi=sysv64(scalar) -> coerce<i64>>(%31, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_imag<i16, reason=explicit>(call<complex<i16>, signature=fn(i32) -> complex<i16>, abi=sysv64(scalar) -> coerce<i64>>(%31, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @foo_int(%39 x: i32) -> complex<i32> [linkage=external] [abi=sysv64(scalar) -> coerce<i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %40 r: complex<i32> [storage=automatic];
// DEFAULT-NEXT:         write<i32>(real(%40), add<i32, overflow=ub>(read<i32>(%39), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(imag(%40), sub<i32, overflow=ub>(read<i32>(%39), const<i32>(1)));
// DEFAULT-NEXT:         return read<complex<i32>>(%40);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @bar_int(%42 x: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%42)), complex_to_real<i32, reason=explicit>(call<complex<i32>, signature=fn(i32) -> complex<i32>, abi=sysv64(scalar) -> coerce<i64>>(%38, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_real<i32, reason=explicit>(call<complex<i32>, signature=fn(i32) -> complex<i32>, abi=sysv64(scalar) -> coerce<i64>>(%38, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @baz_int(%44 x: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%44)), complex_to_imag<i32, reason=explicit>(call<complex<i32>, signature=fn(i32) -> complex<i32>, abi=sysv64(scalar) -> coerce<i64>>(%38, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_imag<i32, reason=explicit>(call<complex<i32>, signature=fn(i32) -> complex<i32>, abi=sysv64(scalar) -> coerce<i64>>(%38, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @foo_long(%46 x: i32) -> complex<i64> [linkage=external] [abi=sysv64(scalar) -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %47 r: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         write<i64>(real(%47), widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%46), const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(imag(%47), widen<i64, reason=assign>(sub<i32, overflow=ub>(read<i32>(%46), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i64>>(%47);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @bar_long(%49 x: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%49)), complex_to_real<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> coerce<i64, i64>>(%45, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_real<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> coerce<i64, i64>>(%45, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @baz_long(%51 x: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%51)), complex_to_imag<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> coerce<i64, i64>>(%45, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_imag<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> coerce<i64, i64>>(%45, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @foo_llong(%53 x: i32) -> complex<i64> [linkage=external] [abi=sysv64(scalar) -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %54 r: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         write<i64>(real(%54), widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%53), const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(imag(%54), widen<i64, reason=assign>(sub<i32, overflow=ub>(read<i32>(%53), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i64>>(%54);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @bar_llong(%56 x: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%56)), complex_to_real<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> coerce<i64, i64>>(%52, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_real<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> coerce<i64, i64>>(%52, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @baz_llong(%58 x: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%58)), complex_to_imag<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> coerce<i64, i64>>(%52, const<i32>(5))));
// DEFAULT-NEXT:         complex_to_imag<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> coerce<i64, i64>>(%52, const<i32>(5)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %60 var: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f32>) -> void>(%6, addr_of<ptr<f32>>(%60));
// DEFAULT-NEXT:             if ne<f32, exceptions=ignore>(read<f32>(%60), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             write<f32>(%60, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f32>) -> void>(%8, addr_of<ptr<f32>>(%60));
// DEFAULT-NEXT:             if ne<f32, exceptions=ignore>(read<f32>(%60), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %61 var: f64 [storage=automatic] = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f64>) -> void>(%13, addr_of<ptr<f64>>(%61));
// DEFAULT-NEXT:             if ne<f64, exceptions=ignore>(read<f64>(%61), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             write<f64>(%61, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f64>) -> void>(%15, addr_of<ptr<f64>>(%61));
// DEFAULT-NEXT:             if ne<f64, exceptions=ignore>(read<f64>(%61), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %62 var: f80 [storage=automatic] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f80>) -> void>(%20, addr_of<ptr<f80>>(%62));
// DEFAULT-NEXT:             if ne<f80, exceptions=ignore>(read<f80>(%62), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             write<f80>(%62, int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f80>) -> void>(%22, addr_of<ptr<f80>>(%62));
// DEFAULT-NEXT:             if ne<f80, exceptions=ignore>(read<f80>(%62), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %63 var: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>) -> void>(%27, addr_of<ptr<i8>>(%63));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(%63)), const<i32>(6))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             write<i8>(%63, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>) -> void>(%29, addr_of<ptr<i8>>(%63));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(%63)), const<i32>(4))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %64 var: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i16>) -> void>(%34, addr_of<ptr<i16>>(%64));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i16>(%64)), const<i32>(6))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             write<i16>(%64, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i16>) -> void>(%36, addr_of<ptr<i16>>(%64));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i16>(%64)), const<i32>(4))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %65 var: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i32>) -> void>(%41, addr_of<ptr<i32>>(%65));
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%65), const<i32>(6))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             write<i32>(%65, const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i32>) -> void>(%43, addr_of<ptr<i32>>(%65));
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%65), const<i32>(4))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %66 var: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i64>) -> void>(%48, addr_of<ptr<i64>>(%66));
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%66), widen<i64, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             write<i64>(%66, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i64>) -> void>(%50, addr_of<ptr<i64>>(%66));
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%66), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %67 var: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i64>) -> void>(%55, addr_of<ptr<i64>>(%67));
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%67), widen<i64, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:             write<i64>(%67, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i64>) -> void>(%57, addr_of<ptr<i64>>(%67));
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%67), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
