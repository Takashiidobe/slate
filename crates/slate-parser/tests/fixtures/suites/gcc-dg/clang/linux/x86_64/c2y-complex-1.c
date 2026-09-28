/* Test C2Y complex increment and decrement.  */
/* { dg-do run } */
/* { dg-options "-std=c2y -pedantic-errors" } */

extern void abort(void);
extern void exit(int);

_Complex float       a, ax;
_Complex double      b, bx;
_Complex long double c, cx;

int
main() {
  ax = a++;
  if (ax != 0 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = a++;
  if (ax != 0 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = 0;
  ax = ++a;
  if (ax != 1 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = ++a;
  if (ax != 1 || a != 1 || __builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      __builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = 0;
  ax = a--;
  if (ax != 0 || a != -1 || __builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = a--;
  if (ax != 0 || a != -1 || __builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = 0;
  ax = --a;
  if (ax != -1 || a != -1 || !__builtin_signbit(__builtin_crealf(ax)) ||
      __builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      __builtin_signbit(__builtin_cimagf(a)))
    abort();
  a  = __builtin_complex(0.0f, -0.0f);
  ax = --a;
  if (ax != -1 || a != -1 || !__builtin_signbit(__builtin_crealf(ax)) ||
      !__builtin_signbit(__builtin_cimagf(ax)) ||
      !__builtin_signbit(__builtin_crealf(a)) ||
      !__builtin_signbit(__builtin_cimagf(a)))
    abort();

  bx = b++;
  if (bx != 0 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0, -0.0);
  bx = b++;
  if (bx != 0 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = 0;
  bx = ++b;
  if (bx != 1 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0, -0.0);
  bx = ++b;
  if (bx != 1 || b != 1 || __builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      __builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = 0;
  bx = b--;
  if (bx != 0 || b != -1 || __builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0f, -0.0f);
  bx = b--;
  if (bx != 0 || b != -1 || __builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = 0;
  bx = --b;
  if (bx != -1 || b != -1 || !__builtin_signbit(__builtin_creal(bx)) ||
      __builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      __builtin_signbit(__builtin_cimag(b)))
    abort();
  b  = __builtin_complex(0.0f, -0.0f);
  bx = --b;
  if (bx != -1 || b != -1 || !__builtin_signbit(__builtin_creal(bx)) ||
      !__builtin_signbit(__builtin_cimag(bx)) ||
      !__builtin_signbit(__builtin_creal(b)) ||
      !__builtin_signbit(__builtin_cimag(b)))
    abort();

  cx = c++;
  if (cx != 0 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = c++;
  if (cx != 0 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = 0;
  cx = ++c;
  if (cx != 1 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = ++c;
  if (cx != 1 || c != 1 || __builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      __builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = 0;
  cx = c--;
  if (cx != 0 || c != -1 || __builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = c--;
  if (cx != 0 || c != -1 || __builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = 0;
  cx = --c;
  if (cx != -1 || c != -1 || !__builtin_signbit(__builtin_creall(cx)) ||
      __builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      __builtin_signbit(__builtin_cimagl(c)))
    abort();
  c  = __builtin_complex(0.0L, -0.0L);
  cx = --c;
  if (cx != -1 || c != -1 || !__builtin_signbit(__builtin_creall(cx)) ||
      !__builtin_signbit(__builtin_cimagl(cx)) ||
      !__builtin_signbit(__builtin_creall(c)) ||
      !__builtin_signbit(__builtin_cimagl(c)))
    abort();

  exit(0);
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
// DEFAULT-NEXT:     global %2 a: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 ax: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 b: complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 bx: complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 c: complex<f80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 cx: complex<f80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @__builtin_crealf(%10 <unnamed>: complex<f32>) -> f32 [linkage=external] [memory=none] [abi=sysv64(coerce<pair<f32>>) -> scalar];
// DEFAULT-NEXT:     fn %13 @__builtin_cimagf(%12 <unnamed>: complex<f32>) -> f32 [linkage=external] [memory=none] [abi=sysv64(coerce<pair<f32>>) -> scalar];
// DEFAULT-NEXT:     fn %15 @__builtin_creal(%14 <unnamed>: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(coerce<f64, f64>) -> scalar];
// DEFAULT-NEXT:     fn %17 @__builtin_cimag(%16 <unnamed>: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(coerce<f64, f64>) -> scalar];
// DEFAULT-NEXT:     fn %19 @__builtin_creall(%18 <unnamed>: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %21 @__builtin_cimagl(%20 <unnamed>: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %22: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %23: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%22), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%23));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%22));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%3)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%3)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%2)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f32>>(%2, aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %24: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %25: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%24), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%25));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%24));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%3)))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%3))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%2)))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f32>>(%2, real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %26: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %27: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%26), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%27));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%27));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%3)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%3)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%2)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f32>>(%2, aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %28: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %29: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%28), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%29));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%29));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%3)))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%3))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%2)))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f32>>(%2, real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %30: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %31: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%30), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%31));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%30));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%3)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%3)))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%2))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f32>>(%2, aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %32: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %33: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%32), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%33));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%32));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%3)))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%3))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%2))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f32>>(%2, real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %34: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %35: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%34), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%35));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%35));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%3))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%3)))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%2))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%2))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f32>>(%2, aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %36: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %37: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%36), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%37));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%37));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%3))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%3))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%11, read<complex<f32>>(%2))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%13, read<complex<f32>>(%2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %38: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %39: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%38), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%39));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%38));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%5)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%5)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%4)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f64>>(%4, aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = neg<f64>(const<f64>(0.0))));
// DEFAULT-NEXT:         let %40: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %41: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%40), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%41));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%40));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%5)))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%5))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%4)))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%4)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f64>>(%4, real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %42: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %43: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%42), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%43));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%43));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%5)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%5)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%4)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f64>>(%4, aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = neg<f64>(const<f64>(0.0))));
// DEFAULT-NEXT:         let %44: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %45: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%44), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%45));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%45));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%5)))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%5))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%4)))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%4)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f64>>(%4, real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %46: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %47: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%46), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%47));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%46));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%5)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%5)))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%4))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f64>>(%4, complex_convert<complex<f64>, reason=assign>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0)))));
// DEFAULT-NEXT:         let %48: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %49: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%48), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%49));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%48));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%5)))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%5))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%4))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%4)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f64>>(%4, real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %50: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %51: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%50), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%51));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%51));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%5))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%5)))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%4))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%4))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f64>>(%4, complex_convert<complex<f64>, reason=assign>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0)))));
// DEFAULT-NEXT:         let %52: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %53: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%52), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%53));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%53));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%5))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%5))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%15, read<complex<f64>>(%4))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%17, read<complex<f64>>(%4)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %54: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %55: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%54), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%55));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%54));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%7)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%7)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%6)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f80>>(%6, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %56: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %57: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%56), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%57));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%56));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%7)))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%7))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%6)))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f80>>(%6, real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %58: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %59: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%58), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%59));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%59));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%7)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%7)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%6)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f80>>(%6, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %60: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %61: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%60), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%61));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%61));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%7)))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%7))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%6)))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f80>>(%6, real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %62: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %63: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%62), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%63));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%62));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%7)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%7)))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%6))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f80>>(%6, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %64: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %65: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%64), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%65));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%64));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%7)))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%7))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%6))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f80>>(%6, real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %66: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %67: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%66), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%67));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%67));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%7))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%7)))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%6))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<complex<f80>>(%6, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %68: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %69: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%68), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%69));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%69));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%7))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%7))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%19, read<complex<f80>>(%6))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%21, read<complex<f80>>(%6)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
