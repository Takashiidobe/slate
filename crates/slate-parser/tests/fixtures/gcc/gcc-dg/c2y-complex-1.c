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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%9 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %11: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%10), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%11));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%10));
// DEFAULT-NEXT:         let %12: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%12, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%12, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %13: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%12)
// DEFAULT-NEXT:             write<bool>(%13, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%13, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %14: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%13)
// DEFAULT-NEXT:             write<bool>(%14, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%14, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%2))));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%14)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%2))));
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f32>>(%2, aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %16: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %17: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%16), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%17));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%16));
// DEFAULT-NEXT:         let %18: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%18, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%18, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %19: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             write<bool>(%19, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%19, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%3)))));
// DEFAULT-NEXT:         let %20: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%19)
// DEFAULT-NEXT:             write<bool>(%20, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%20, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%2))));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%20)
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%2)))));
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f32>>(%2, real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %22: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %23: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%22), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%23));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%23));
// DEFAULT-NEXT:         let %24: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%24, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%24, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %25: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%24)
// DEFAULT-NEXT:             write<bool>(%25, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%25, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %26: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%25)
// DEFAULT-NEXT:             write<bool>(%26, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%26, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%2))));
// DEFAULT-NEXT:         let %27: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%26)
// DEFAULT-NEXT:             write<bool>(%27, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%27, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%2))));
// DEFAULT-NEXT:         if read<bool>(%27)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f32>>(%2, aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %28: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %29: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%28), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%29));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%29));
// DEFAULT-NEXT:         let %30: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%30, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%30, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %31: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%30)
// DEFAULT-NEXT:             write<bool>(%31, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%31, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%3)))));
// DEFAULT-NEXT:         let %32: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%31)
// DEFAULT-NEXT:             write<bool>(%32, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%32, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%2))));
// DEFAULT-NEXT:         let %33: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%32)
// DEFAULT-NEXT:             write<bool>(%33, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%33, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%2)))));
// DEFAULT-NEXT:         if read<bool>(%33)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f32>>(%2, real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %34: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %35: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%34), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%35));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%34));
// DEFAULT-NEXT:         let %36: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%36, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%36, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %37: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%36)
// DEFAULT-NEXT:             write<bool>(%37, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%37, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %38: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%37)
// DEFAULT-NEXT:             write<bool>(%38, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%38, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%2)))));
// DEFAULT-NEXT:         let %39: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%38)
// DEFAULT-NEXT:             write<bool>(%39, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%39, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%2))));
// DEFAULT-NEXT:         if read<bool>(%39)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f32>>(%2, aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %40: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %41: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%40), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%41));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%40));
// DEFAULT-NEXT:         let %42: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%42, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%42, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %43: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%42)
// DEFAULT-NEXT:             write<bool>(%43, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%43, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%3)))));
// DEFAULT-NEXT:         let %44: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%43)
// DEFAULT-NEXT:             write<bool>(%44, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%44, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%2)))));
// DEFAULT-NEXT:         let %45: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%44)
// DEFAULT-NEXT:             write<bool>(%45, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%45, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%2)))));
// DEFAULT-NEXT:         if read<bool>(%45)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f32>>(%2, real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %46: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %47: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%46), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%47));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%47));
// DEFAULT-NEXT:         let %48: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%48, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%48, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%3)))));
// DEFAULT-NEXT:         let %49: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%48)
// DEFAULT-NEXT:             write<bool>(%49, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%49, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%3))));
// DEFAULT-NEXT:         let %50: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%49)
// DEFAULT-NEXT:             write<bool>(%50, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%50, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%2)))));
// DEFAULT-NEXT:         let %51: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%50)
// DEFAULT-NEXT:             write<bool>(%51, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%51, float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%2))));
// DEFAULT-NEXT:         if read<bool>(%51)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f32>>(%2, aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %52: complex<f32> [synthetic] = read<complex<f32>>(%2);
// DEFAULT-NEXT:         let %53: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%52), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%2, read<complex<f32>>(%53));
// DEFAULT-NEXT:         write<complex<f32>>(%3, read<complex<f32>>(%53));
// DEFAULT-NEXT:         let %54: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%3), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%54, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%54, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%3)))));
// DEFAULT-NEXT:         let %55: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%54)
// DEFAULT-NEXT:             write<bool>(%55, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%55, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%3)))));
// DEFAULT-NEXT:         let %56: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%55)
// DEFAULT-NEXT:             write<bool>(%56, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%56, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_crealf, read<complex<f32>>(%2)))));
// DEFAULT-NEXT:         let %57: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%56)
// DEFAULT-NEXT:             write<bool>(%57, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%57, not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(__builtin_cimagf, read<complex<f32>>(%2)))));
// DEFAULT-NEXT:         if read<bool>(%57)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %58: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %59: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%58), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%59));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%58));
// DEFAULT-NEXT:         let %60: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%60, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%60, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %61: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%60)
// DEFAULT-NEXT:             write<bool>(%61, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%61, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %62: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%61)
// DEFAULT-NEXT:             write<bool>(%62, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%62, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%4))));
// DEFAULT-NEXT:         let %63: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%62)
// DEFAULT-NEXT:             write<bool>(%63, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%63, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%4))));
// DEFAULT-NEXT:         if read<bool>(%63)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f64>>(%4, aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = neg<f64>(const<f64>(0.0))));
// DEFAULT-NEXT:         let %64: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %65: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%64), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%65));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%64));
// DEFAULT-NEXT:         let %66: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%66, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%66, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %67: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%66)
// DEFAULT-NEXT:             write<bool>(%67, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%67, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%5)))));
// DEFAULT-NEXT:         let %68: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%67)
// DEFAULT-NEXT:             write<bool>(%68, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%68, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%4))));
// DEFAULT-NEXT:         let %69: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%68)
// DEFAULT-NEXT:             write<bool>(%69, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%69, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%4)))));
// DEFAULT-NEXT:         if read<bool>(%69)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f64>>(%4, real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %70: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %71: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%70), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%71));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%71));
// DEFAULT-NEXT:         let %72: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%72, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%72, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %73: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%72)
// DEFAULT-NEXT:             write<bool>(%73, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%73, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %74: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%73)
// DEFAULT-NEXT:             write<bool>(%74, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%74, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%4))));
// DEFAULT-NEXT:         let %75: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%74)
// DEFAULT-NEXT:             write<bool>(%75, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%75, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%4))));
// DEFAULT-NEXT:         if read<bool>(%75)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f64>>(%4, aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = neg<f64>(const<f64>(0.0))));
// DEFAULT-NEXT:         let %76: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %77: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%76), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%77));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%77));
// DEFAULT-NEXT:         let %78: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%78, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%78, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %79: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%78)
// DEFAULT-NEXT:             write<bool>(%79, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%79, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%5)))));
// DEFAULT-NEXT:         let %80: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%79)
// DEFAULT-NEXT:             write<bool>(%80, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%80, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%4))));
// DEFAULT-NEXT:         let %81: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%80)
// DEFAULT-NEXT:             write<bool>(%81, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%81, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%4)))));
// DEFAULT-NEXT:         if read<bool>(%81)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f64>>(%4, real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %82: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %83: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%82), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%83));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%82));
// DEFAULT-NEXT:         let %84: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%84, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%84, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %85: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%84)
// DEFAULT-NEXT:             write<bool>(%85, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%85, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %86: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%85)
// DEFAULT-NEXT:             write<bool>(%86, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%86, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%4)))));
// DEFAULT-NEXT:         let %87: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%86)
// DEFAULT-NEXT:             write<bool>(%87, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%87, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%4))));
// DEFAULT-NEXT:         if read<bool>(%87)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f64>>(%4, complex_convert<complex<f64>, reason=assign>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0)))));
// DEFAULT-NEXT:         let %88: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %89: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%88), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%89));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%88));
// DEFAULT-NEXT:         let %90: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%90, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%90, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %91: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%90)
// DEFAULT-NEXT:             write<bool>(%91, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%91, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%5)))));
// DEFAULT-NEXT:         let %92: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%91)
// DEFAULT-NEXT:             write<bool>(%92, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%92, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%4)))));
// DEFAULT-NEXT:         let %93: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%92)
// DEFAULT-NEXT:             write<bool>(%93, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%93, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%4)))));
// DEFAULT-NEXT:         if read<bool>(%93)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f64>>(%4, real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %94: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %95: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%94), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%95));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%95));
// DEFAULT-NEXT:         let %96: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%96, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%96, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%5)))));
// DEFAULT-NEXT:         let %97: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%96)
// DEFAULT-NEXT:             write<bool>(%97, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%97, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%5))));
// DEFAULT-NEXT:         let %98: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%97)
// DEFAULT-NEXT:             write<bool>(%98, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%98, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%4)))));
// DEFAULT-NEXT:         let %99: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%98)
// DEFAULT-NEXT:             write<bool>(%99, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%99, float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%4))));
// DEFAULT-NEXT:         if read<bool>(%99)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f64>>(%4, complex_convert<complex<f64>, reason=assign>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0)))));
// DEFAULT-NEXT:         let %100: complex<f64> [synthetic] = read<complex<f64>>(%4);
// DEFAULT-NEXT:         let %101: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%100), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%4, read<complex<f64>>(%101));
// DEFAULT-NEXT:         write<complex<f64>>(%5, read<complex<f64>>(%101));
// DEFAULT-NEXT:         let %102: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%5), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%102, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%102, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%5)))));
// DEFAULT-NEXT:         let %103: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%102)
// DEFAULT-NEXT:             write<bool>(%103, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%103, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%5)))));
// DEFAULT-NEXT:         let %104: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%103)
// DEFAULT-NEXT:             write<bool>(%104, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%104, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%4)))));
// DEFAULT-NEXT:         let %105: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%104)
// DEFAULT-NEXT:             write<bool>(%105, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%105, not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%4)))));
// DEFAULT-NEXT:         if read<bool>(%105)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %106: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %107: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%106), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%107));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%106));
// DEFAULT-NEXT:         let %108: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%108, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%108, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %109: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%108)
// DEFAULT-NEXT:             write<bool>(%109, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%109, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %110: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%109)
// DEFAULT-NEXT:             write<bool>(%110, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%110, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%6))));
// DEFAULT-NEXT:         let %111: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%110)
// DEFAULT-NEXT:             write<bool>(%111, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%111, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%6))));
// DEFAULT-NEXT:         if read<bool>(%111)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f80>>(%6, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %112: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %113: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%112), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%113));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%112));
// DEFAULT-NEXT:         let %114: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%114, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%114, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %115: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%114)
// DEFAULT-NEXT:             write<bool>(%115, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%115, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%7)))));
// DEFAULT-NEXT:         let %116: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%115)
// DEFAULT-NEXT:             write<bool>(%116, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%116, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%6))));
// DEFAULT-NEXT:         let %117: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%116)
// DEFAULT-NEXT:             write<bool>(%117, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%117, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%6)))));
// DEFAULT-NEXT:         if read<bool>(%117)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f80>>(%6, real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %118: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %119: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%118), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%119));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%119));
// DEFAULT-NEXT:         let %120: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%120, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%120, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %121: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%120)
// DEFAULT-NEXT:             write<bool>(%121, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%121, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %122: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%121)
// DEFAULT-NEXT:             write<bool>(%122, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%122, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%6))));
// DEFAULT-NEXT:         let %123: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%122)
// DEFAULT-NEXT:             write<bool>(%123, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%123, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%6))));
// DEFAULT-NEXT:         if read<bool>(%123)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f80>>(%6, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %124: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %125: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%124), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%125));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%125));
// DEFAULT-NEXT:         let %126: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             write<bool>(%126, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%126, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %127: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%126)
// DEFAULT-NEXT:             write<bool>(%127, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%127, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%7)))));
// DEFAULT-NEXT:         let %128: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%127)
// DEFAULT-NEXT:             write<bool>(%128, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%128, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%6))));
// DEFAULT-NEXT:         let %129: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%128)
// DEFAULT-NEXT:             write<bool>(%129, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%129, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%6)))));
// DEFAULT-NEXT:         if read<bool>(%129)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f80>>(%6, real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %130: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %131: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%130), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%131));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%130));
// DEFAULT-NEXT:         let %132: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%132, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%132, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %133: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%132)
// DEFAULT-NEXT:             write<bool>(%133, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%133, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %134: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%133)
// DEFAULT-NEXT:             write<bool>(%134, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%134, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%6)))));
// DEFAULT-NEXT:         let %135: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%134)
// DEFAULT-NEXT:             write<bool>(%135, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%135, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%6))));
// DEFAULT-NEXT:         if read<bool>(%135)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f80>>(%6, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %136: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %137: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%136), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%137));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%136));
// DEFAULT-NEXT:         let %138: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%138, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%138, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %139: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%138)
// DEFAULT-NEXT:             write<bool>(%139, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%139, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%7)))));
// DEFAULT-NEXT:         let %140: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%139)
// DEFAULT-NEXT:             write<bool>(%140, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%140, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%6)))));
// DEFAULT-NEXT:         let %141: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%140)
// DEFAULT-NEXT:             write<bool>(%141, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%141, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%6)))));
// DEFAULT-NEXT:         if read<bool>(%141)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f80>>(%6, real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %142: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %143: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%142), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%143));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%143));
// DEFAULT-NEXT:         let %144: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%144, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%144, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%7)))));
// DEFAULT-NEXT:         let %145: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%144)
// DEFAULT-NEXT:             write<bool>(%145, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%145, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%7))));
// DEFAULT-NEXT:         let %146: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%145)
// DEFAULT-NEXT:             write<bool>(%146, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%146, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%6)))));
// DEFAULT-NEXT:         let %147: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%146)
// DEFAULT-NEXT:             write<bool>(%147, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%147, float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%6))));
// DEFAULT-NEXT:         if read<bool>(%147)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<complex<f80>>(%6, aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %148: complex<f80> [synthetic] = read<complex<f80>>(%6);
// DEFAULT-NEXT:         let %149: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%148), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%6, read<complex<f80>>(%149));
// DEFAULT-NEXT:         write<complex<f80>>(%7, read<complex<f80>>(%149));
// DEFAULT-NEXT:         let %150: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%7), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%6), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))))
// DEFAULT-NEXT:             write<bool>(%150, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%150, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%7)))));
// DEFAULT-NEXT:         let %151: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%150)
// DEFAULT-NEXT:             write<bool>(%151, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%151, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%7)))));
// DEFAULT-NEXT:         let %152: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%151)
// DEFAULT-NEXT:             write<bool>(%152, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%152, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_creall, read<complex<f80>>(%6)))));
// DEFAULT-NEXT:         let %153: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%152)
// DEFAULT-NEXT:             write<bool>(%153, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%153, not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(__builtin_cimagl, read<complex<f80>>(%6)))));
// DEFAULT-NEXT:         if read<bool>(%153)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
