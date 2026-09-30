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
// DEFAULT-NEXT:     type @type[[TYPE_ldouble_t:[0-9]+]] ldouble_t = f80;
// DEFAULT-NEXT:     type @type[[TYPE_llong:[0-9]+]] llong = i64;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo_float:[0-9]+]] @foo_float(%[[VALUE_x:[0-9]+]] x: i32) -> complex<f32> [linkage=external] [memory=read] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         write<f32>(real(%[[VALUE_r]]), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(1))));
// DEFAULT-NEXT:         write<f32>(imag(%[[VALUE_r]]), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<f32>>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_float:[0-9]+]] @bar_float(%[[VALUE_x_2:[0-9]+]] x: ptr<f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(deref(read<ptr<f32>>(%[[VALUE_x_2]])), complex_to_real<f32, reason=explicit>(call<complex<f32>, signature=fn(i32) -> complex<f32>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_float]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz_float:[0-9]+]] @baz_float(%[[VALUE_x_3:[0-9]+]] x: ptr<f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(deref(read<ptr<f32>>(%[[VALUE_x_3]])), complex_to_imag<f32, reason=explicit>(call<complex<f32>, signature=fn(i32) -> complex<f32>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_float]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo_double:[0-9]+]] @foo_double(%[[VALUE_x_4:[0-9]+]] x: i32) -> complex<f64> [linkage=external] [memory=read] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<f64>(real(%[[VALUE_r_2]]), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(add<i32, overflow=ub>(read<i32>(%[[VALUE_x_4]]), const<i32>(1))));
// DEFAULT-NEXT:         write<f64>(imag(%[[VALUE_r_2]]), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_x_4]]), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<f64>>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_double:[0-9]+]] @bar_double(%[[VALUE_x_5:[0-9]+]] x: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(deref(read<ptr<f64>>(%[[VALUE_x_5]])), complex_to_real<f64, reason=explicit>(call<complex<f64>, signature=fn(i32) -> complex<f64>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_double]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz_double:[0-9]+]] @baz_double(%[[VALUE_x_6:[0-9]+]] x: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(deref(read<ptr<f64>>(%[[VALUE_x_6]])), complex_to_imag<f64, reason=explicit>(call<complex<f64>, signature=fn(i32) -> complex<f64>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_double]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo_ldouble_t:[0-9]+]] @foo_ldouble_t(%[[VALUE_x_7:[0-9]+]] x: i32) -> complex<f80> [linkage=external] [memory=read] [abi=sysv64(scalar) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: complex<f80> [storage=automatic];
// DEFAULT-NEXT:         write<f80>(real(%[[VALUE_r_3]]), int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(add<i32, overflow=ub>(read<i32>(%[[VALUE_x_7]]), const<i32>(1))));
// DEFAULT-NEXT:         write<f80>(imag(%[[VALUE_r_3]]), int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_x_7]]), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<f80>>(%[[VALUE_r_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_ldouble_t:[0-9]+]] @bar_ldouble_t(%[[VALUE_x_8:[0-9]+]] x: ptr<f80>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f80>(deref(read<ptr<f80>>(%[[VALUE_x_8]])), complex_to_real<f80, reason=explicit>(call<complex<f80>, signature=fn(i32) -> complex<f80>, abi=sysv64(scalar) -> coerce<f80, f80>>(%[[VALUE_foo_ldouble_t]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz_ldouble_t:[0-9]+]] @baz_ldouble_t(%[[VALUE_x_9:[0-9]+]] x: ptr<f80>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f80>(deref(read<ptr<f80>>(%[[VALUE_x_9]])), complex_to_imag<f80, reason=explicit>(call<complex<f80>, signature=fn(i32) -> complex<f80>, abi=sysv64(scalar) -> coerce<f80, f80>>(%[[VALUE_foo_ldouble_t]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo_char:[0-9]+]] @foo_char(%[[VALUE_x_10:[0-9]+]] x: i32) -> complex<i8> [linkage=external] [memory=read] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: complex<i8> [storage=automatic];
// DEFAULT-NEXT:         write<i8>(real(%[[VALUE_r_4]]), truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_x_10]]), const<i32>(1))));
// DEFAULT-NEXT:         write<i8>(imag(%[[VALUE_r_4]]), truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_x_10]]), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i8>>(%[[VALUE_r_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_char:[0-9]+]] @bar_char(%[[VALUE_x_11:[0-9]+]] x: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_x_11]])), complex_to_real<i8, reason=explicit>(call<complex<i8>, signature=fn(i32) -> complex<i8>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_char]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz_char:[0-9]+]] @baz_char(%[[VALUE_x_12:[0-9]+]] x: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_x_12]])), complex_to_imag<i8, reason=explicit>(call<complex<i8>, signature=fn(i32) -> complex<i8>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_char]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo_short:[0-9]+]] @foo_short(%[[VALUE_x_13:[0-9]+]] x: i32) -> complex<i16> [linkage=external] [memory=read] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: complex<i16> [storage=automatic];
// DEFAULT-NEXT:         write<i16>(real(%[[VALUE_r_5]]), truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(read<i32>(%[[VALUE_x_13]]), const<i32>(1))));
// DEFAULT-NEXT:         write<i16>(imag(%[[VALUE_r_5]]), truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_x_13]]), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i16>>(%[[VALUE_r_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_short:[0-9]+]] @bar_short(%[[VALUE_x_14:[0-9]+]] x: ptr<i16>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(deref(read<ptr<i16>>(%[[VALUE_x_14]])), complex_to_real<i16, reason=explicit>(call<complex<i16>, signature=fn(i32) -> complex<i16>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_short]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz_short:[0-9]+]] @baz_short(%[[VALUE_x_15:[0-9]+]] x: ptr<i16>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(deref(read<ptr<i16>>(%[[VALUE_x_15]])), complex_to_imag<i16, reason=explicit>(call<complex<i16>, signature=fn(i32) -> complex<i16>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_short]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo_int:[0-9]+]] @foo_int(%[[VALUE_x_16:[0-9]+]] x: i32) -> complex<i32> [linkage=external] [memory=read] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_6:[0-9]+]] r: complex<i32> [storage=automatic];
// DEFAULT-NEXT:         write<i32>(real(%[[VALUE_r_6]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_x_16]]), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(imag(%[[VALUE_r_6]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_x_16]]), const<i32>(1)));
// DEFAULT-NEXT:         return read<complex<i32>>(%[[VALUE_r_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_int:[0-9]+]] @bar_int(%[[VALUE_x_17:[0-9]+]] x: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_x_17]])), complex_to_real<i32, reason=explicit>(call<complex<i32>, signature=fn(i32) -> complex<i32>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_int]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz_int:[0-9]+]] @baz_int(%[[VALUE_x_18:[0-9]+]] x: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_x_18]])), complex_to_imag<i32, reason=explicit>(call<complex<i32>, signature=fn(i32) -> complex<i32>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_int]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo_long:[0-9]+]] @foo_long(%[[VALUE_x_19:[0-9]+]] x: i32) -> complex<i64> [linkage=external] [memory=read] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_7:[0-9]+]] r: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         write<i64>(real(%[[VALUE_r_7]]), widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_x_19]]), const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(imag(%[[VALUE_r_7]]), widen<i64, reason=assign>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_x_19]]), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i64>>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_long:[0-9]+]] @bar_long(%[[VALUE_x_20:[0-9]+]] x: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE_x_20]])), complex_to_real<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_long]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz_long:[0-9]+]] @baz_long(%[[VALUE_x_21:[0-9]+]] x: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE_x_21]])), complex_to_imag<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_long]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo_llong:[0-9]+]] @foo_llong(%[[VALUE_x_22:[0-9]+]] x: i32) -> complex<i64> [linkage=external] [memory=read] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_8:[0-9]+]] r: complex<i64> [storage=automatic];
// DEFAULT-NEXT:         write<i64>(real(%[[VALUE_r_8]]), widen<i64, reason=assign>(add<i32, overflow=ub>(read<i32>(%[[VALUE_x_22]]), const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(imag(%[[VALUE_r_8]]), widen<i64, reason=assign>(sub<i32, overflow=ub>(read<i32>(%[[VALUE_x_22]]), const<i32>(1))));
// DEFAULT-NEXT:         return read<complex<i64>>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar_llong:[0-9]+]] @bar_llong(%[[VALUE_x_23:[0-9]+]] x: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE_x_23]])), complex_to_real<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_llong]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz_llong:[0-9]+]] @baz_llong(%[[VALUE_x_24:[0-9]+]] x: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(deref(read<ptr<i64>>(%[[VALUE_x_24]])), complex_to_imag<i64, reason=explicit>(call<complex<i64>, signature=fn(i32) -> complex<i64>, abi=sysv64(scalar) -> native_c>(%[[VALUE_foo_llong]], const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_var:[0-9]+]] var: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f32>) -> void>(%[[VALUE_bar_float]], addr_of<ptr<f32>>(%[[VALUE_var]]));
// DEFAULT-NEXT:             if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_var]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             write<f32>(%[[VALUE_var]], int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f32>) -> void>(%[[VALUE_baz_float]], addr_of<ptr<f32>>(%[[VALUE_var]]));
// DEFAULT-NEXT:             if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_var]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_var_2:[0-9]+]] var: f64 [storage=automatic] = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f64>) -> void>(%[[VALUE_bar_double]], addr_of<ptr<f64>>(%[[VALUE_var_2]]));
// DEFAULT-NEXT:             if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_var_2]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             write<f64>(%[[VALUE_var_2]], int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f64>) -> void>(%[[VALUE_baz_double]], addr_of<ptr<f64>>(%[[VALUE_var_2]]));
// DEFAULT-NEXT:             if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_var_2]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_var_3:[0-9]+]] var: f80 [storage=automatic] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f80>) -> void>(%[[VALUE_bar_ldouble_t]], addr_of<ptr<f80>>(%[[VALUE_var_3]]));
// DEFAULT-NEXT:             if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_var_3]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             write<f80>(%[[VALUE_var_3]], int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<f80>) -> void>(%[[VALUE_baz_ldouble_t]], addr_of<ptr<f80>>(%[[VALUE_var_3]]));
// DEFAULT-NEXT:             if ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_var_3]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_var_4:[0-9]+]] var: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>) -> void>(%[[VALUE_bar_char]], addr_of<ptr<i8>>(%[[VALUE_var_4]]));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_var_4]])), const<i32>(6))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             write<i8>(%[[VALUE_var_4]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i8>) -> void>(%[[VALUE_baz_char]], addr_of<ptr<i8>>(%[[VALUE_var_4]]));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_var_4]])), const<i32>(4))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_var_5:[0-9]+]] var: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i16>) -> void>(%[[VALUE_bar_short]], addr_of<ptr<i16>>(%[[VALUE_var_5]]));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_var_5]])), const<i32>(6))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             write<i16>(%[[VALUE_var_5]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i16>) -> void>(%[[VALUE_baz_short]], addr_of<ptr<i16>>(%[[VALUE_var_5]]));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_var_5]])), const<i32>(4))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_var_6:[0-9]+]] var: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_bar_int]], addr_of<ptr<i32>>(%[[VALUE_var_6]]));
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_var_6]]), const<i32>(6))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             write<i32>(%[[VALUE_var_6]], const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_baz_int]], addr_of<ptr<i32>>(%[[VALUE_var_6]]));
// DEFAULT-NEXT:             if ne<i32>(read<i32>(%[[VALUE_var_6]]), const<i32>(4))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_var_7:[0-9]+]] var: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i64>) -> void>(%[[VALUE_bar_long]], addr_of<ptr<i64>>(%[[VALUE_var_7]]));
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%[[VALUE_var_7]]), widen<i64, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             write<i64>(%[[VALUE_var_7]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i64>) -> void>(%[[VALUE_baz_long]], addr_of<ptr<i64>>(%[[VALUE_var_7]]));
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%[[VALUE_var_7]]), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_var_8:[0-9]+]] var: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i64>) -> void>(%[[VALUE_bar_llong]], addr_of<ptr<i64>>(%[[VALUE_var_8]]));
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%[[VALUE_var_8]]), widen<i64, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:             write<i64>(%[[VALUE_var_8]], widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<i64>) -> void>(%[[VALUE_baz_llong]], addr_of<ptr<i64>>(%[[VALUE_var_8]]));
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%[[VALUE_var_8]]), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
