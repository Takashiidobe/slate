/* Test C2Y complex increment and decrement.  */
/* { dg-do run } */
/* { dg-options "-std=c2y -pedantic-errors" } */

extern void abort (void);
extern void exit (int);

_Complex float a, ax;
_Complex double b, bx;
_Complex long double c, cx;

int
main ()
{
  ax = a++;
  if (ax != 0
      || a != 1
      || __builtin_signbit (__builtin_crealf (ax))
      || __builtin_signbit (__builtin_cimagf (ax))
      || __builtin_signbit (__builtin_crealf (a))
      || __builtin_signbit (__builtin_cimagf (a)))
    abort ();
  a = __builtin_complex (0.0f, -0.0f);
  ax = a++;
  if (ax != 0
      || a != 1
      || __builtin_signbit (__builtin_crealf (ax))
      || !__builtin_signbit (__builtin_cimagf (ax))
      || __builtin_signbit (__builtin_crealf (a))
      || !__builtin_signbit (__builtin_cimagf (a)))
    abort ();
  a = 0;
  ax = ++a;
  if (ax != 1
      || a != 1
      || __builtin_signbit (__builtin_crealf (ax))
      || __builtin_signbit (__builtin_cimagf (ax))
      || __builtin_signbit (__builtin_crealf (a))
      || __builtin_signbit (__builtin_cimagf (a)))
    abort ();
  a = __builtin_complex (0.0f, -0.0f);
  ax = ++a;
  if (ax != 1
      || a != 1
      || __builtin_signbit (__builtin_crealf (ax))
      || !__builtin_signbit (__builtin_cimagf (ax))
      || __builtin_signbit (__builtin_crealf (a))
      || !__builtin_signbit (__builtin_cimagf (a)))
    abort ();
  a = 0;
  ax = a--;
  if (ax != 0
      || a != -1
      || __builtin_signbit (__builtin_crealf (ax))
      || __builtin_signbit (__builtin_cimagf (ax))
      || !__builtin_signbit (__builtin_crealf (a))
      || __builtin_signbit (__builtin_cimagf (a)))
    abort ();
  a = __builtin_complex (0.0f, -0.0f);
  ax = a--;
  if (ax != 0
      || a != -1
      || __builtin_signbit (__builtin_crealf (ax))
      || !__builtin_signbit (__builtin_cimagf (ax))
      || !__builtin_signbit (__builtin_crealf (a))
      || !__builtin_signbit (__builtin_cimagf (a)))
    abort ();
  a = 0;
  ax = --a;
  if (ax != -1
      || a != -1
      || !__builtin_signbit (__builtin_crealf (ax))
      || __builtin_signbit (__builtin_cimagf (ax))
      || !__builtin_signbit (__builtin_crealf (a))
      || __builtin_signbit (__builtin_cimagf (a)))
    abort ();
  a = __builtin_complex (0.0f, -0.0f);
  ax = --a;
  if (ax != -1
      || a != -1
      || !__builtin_signbit (__builtin_crealf (ax))
      || !__builtin_signbit (__builtin_cimagf (ax))
      || !__builtin_signbit (__builtin_crealf (a))
      || !__builtin_signbit (__builtin_cimagf (a)))
    abort ();

  bx = b++;
  if (bx != 0
      || b != 1
      || __builtin_signbit (__builtin_creal (bx))
      || __builtin_signbit (__builtin_cimag (bx))
      || __builtin_signbit (__builtin_creal (b))
      || __builtin_signbit (__builtin_cimag (b)))
    abort ();
  b = __builtin_complex (0.0, -0.0);
  bx = b++;
  if (bx != 0
      || b != 1
      || __builtin_signbit (__builtin_creal (bx))
      || !__builtin_signbit (__builtin_cimag (bx))
      || __builtin_signbit (__builtin_creal (b))
      || !__builtin_signbit (__builtin_cimag (b)))
    abort ();
  b = 0;
  bx = ++b;
  if (bx != 1
      || b != 1
      || __builtin_signbit (__builtin_creal (bx))
      || __builtin_signbit (__builtin_cimag (bx))
      || __builtin_signbit (__builtin_creal (b))
      || __builtin_signbit (__builtin_cimag (b)))
    abort ();
  b = __builtin_complex (0.0, -0.0);
  bx = ++b;
  if (bx != 1
      || b != 1
      || __builtin_signbit (__builtin_creal (bx))
      || !__builtin_signbit (__builtin_cimag (bx))
      || __builtin_signbit (__builtin_creal (b))
      || !__builtin_signbit (__builtin_cimag (b)))
    abort ();
  b = 0;
  bx = b--;
  if (bx != 0
      || b != -1
      || __builtin_signbit (__builtin_creal (bx))
      || __builtin_signbit (__builtin_cimag (bx))
      || !__builtin_signbit (__builtin_creal (b))
      || __builtin_signbit (__builtin_cimag (b)))
    abort ();
  b = __builtin_complex (0.0f, -0.0f);
  bx = b--;
  if (bx != 0
      || b != -1
      || __builtin_signbit (__builtin_creal (bx))
      || !__builtin_signbit (__builtin_cimag (bx))
      || !__builtin_signbit (__builtin_creal (b))
      || !__builtin_signbit (__builtin_cimag (b)))
    abort ();
  b = 0;
  bx = --b;
  if (bx != -1
      || b != -1
      || !__builtin_signbit (__builtin_creal (bx))
      || __builtin_signbit (__builtin_cimag (bx))
      || !__builtin_signbit (__builtin_creal (b))
      || __builtin_signbit (__builtin_cimag (b)))
    abort ();
  b = __builtin_complex (0.0f, -0.0f);
  bx = --b;
  if (bx != -1
      || b != -1
      || !__builtin_signbit (__builtin_creal (bx))
      || !__builtin_signbit (__builtin_cimag (bx))
      || !__builtin_signbit (__builtin_creal (b))
      || !__builtin_signbit (__builtin_cimag (b)))
    abort ();

  cx = c++;
  if (cx != 0
      || c != 1
      || __builtin_signbit (__builtin_creall (cx))
      || __builtin_signbit (__builtin_cimagl (cx))
      || __builtin_signbit (__builtin_creall (c))
      || __builtin_signbit (__builtin_cimagl (c)))
    abort ();
  c = __builtin_complex (0.0L, -0.0L);
  cx = c++;
  if (cx != 0
      || c != 1
      || __builtin_signbit (__builtin_creall (cx))
      || !__builtin_signbit (__builtin_cimagl (cx))
      || __builtin_signbit (__builtin_creall (c))
      || !__builtin_signbit (__builtin_cimagl (c)))
    abort ();
  c = 0;
  cx = ++c;
  if (cx != 1
      || c != 1
      || __builtin_signbit (__builtin_creall (cx))
      || __builtin_signbit (__builtin_cimagl (cx))
      || __builtin_signbit (__builtin_creall (c))
      || __builtin_signbit (__builtin_cimagl (c)))
    abort ();
  c = __builtin_complex (0.0L, -0.0L);
  cx = ++c;
  if (cx != 1
      || c != 1
      || __builtin_signbit (__builtin_creall (cx))
      || !__builtin_signbit (__builtin_cimagl (cx))
      || __builtin_signbit (__builtin_creall (c))
      || !__builtin_signbit (__builtin_cimagl (c)))
    abort ();
  c = 0;
  cx = c--;
  if (cx != 0
      || c != -1
      || __builtin_signbit (__builtin_creall (cx))
      || __builtin_signbit (__builtin_cimagl (cx))
      || !__builtin_signbit (__builtin_creall (c))
      || __builtin_signbit (__builtin_cimagl (c)))
    abort ();
  c = __builtin_complex (0.0L, -0.0L);
  cx = c--;
  if (cx != 0
      || c != -1
      || __builtin_signbit (__builtin_creall (cx))
      || !__builtin_signbit (__builtin_cimagl (cx))
      || !__builtin_signbit (__builtin_creall (c))
      || !__builtin_signbit (__builtin_cimagl (c)))
    abort ();
  c = 0;
  cx = --c;
  if (cx != -1
      || c != -1
      || !__builtin_signbit (__builtin_creall (cx))
      || __builtin_signbit (__builtin_cimagl (cx))
      || !__builtin_signbit (__builtin_creall (c))
      || __builtin_signbit (__builtin_cimagl (c)))
    abort ();
  c = __builtin_complex (0.0L, -0.0L);
  cx = --c;
  if (cx != -1
      || c != -1
      || !__builtin_signbit (__builtin_creall (cx))
      || !__builtin_signbit (__builtin_cimagl (cx))
      || !__builtin_signbit (__builtin_creall (c))
      || !__builtin_signbit (__builtin_cimagl (c)))
    abort ();

  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ax:[0-9]+]] ax: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_bx:[0-9]+]] bx: complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: complex<f80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cx:[0-9]+]] cx: complex<f80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_crealf:[0-9]+]] @__builtin_crealf(%[[VALUE1:[0-9]+]] <unnamed>: complex<f32>) -> f32 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cimagf:[0-9]+]] @__builtin_cimagf(%[[VALUE2:[0-9]+]] <unnamed>: complex<f32>) -> f32 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_creal:[0-9]+]] @__builtin_creal(%[[VALUE3:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cimag:[0-9]+]] @__builtin_cimag(%[[VALUE4:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_creall:[0-9]+]] @__builtin_creall(%[[VALUE5:[0-9]+]] <unnamed>: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cimagl:[0-9]+]] @__builtin_cimagl(%[[VALUE6:[0-9]+]] <unnamed>: complex<f80>) -> f80 [linkage=external] [memory=none] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE7]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE8]]));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_ax]], read<complex<f32>>(%[[VALUE7]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_ax]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_a]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_ax]])))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_ax]])))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_a]])))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_a]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE9]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE10]]));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_ax]], read<complex<f32>>(%[[VALUE9]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_ax]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_a]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_ax]])))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_ax]]))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_a]])))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_a]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE11]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE12]]));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_ax]], read<complex<f32>>(%[[VALUE12]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_ax]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_a]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_ax]])))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_ax]])))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_a]])))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_a]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE13]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE14]]));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_ax]], read<complex<f32>>(%[[VALUE14]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_ax]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_a]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_ax]])))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_ax]]))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_a]])))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_a]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE15]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE16]]));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_ax]], read<complex<f32>>(%[[VALUE15]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_ax]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_a]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_ax]])))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_ax]])))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_a]]))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_a]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE17]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE18]]));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_ax]], read<complex<f32>>(%[[VALUE17]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_ax]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_a]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_ax]])))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_ax]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_a]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_a]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], real_to_complex<complex<f32>, reason=assign>(int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE19]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE20]]));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_ax]], read<complex<f32>>(%[[VALUE20]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_ax]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_a]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_ax]]))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_ax]])))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_a]]))))), float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_a]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0))));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE21]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE22]]));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_ax]], read<complex<f32>>(%[[VALUE22]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_ax]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f32>, exceptions=ignore>(read<complex<f32>>(%[[VALUE_a]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_ax]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_ax]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_crealf]], read<complex<f32>>(%[[VALUE_a]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimagf]], read<complex<f32>>(%[[VALUE_a]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE23]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], read<complex<f64>>(%[[VALUE24]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_bx]], read<complex<f64>>(%[[VALUE23]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_bx]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_b]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_bx]])))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_bx]])))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_b]])))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_b]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = neg<f64>(const<f64>(0.0))));
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE25]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], read<complex<f64>>(%[[VALUE26]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_bx]], read<complex<f64>>(%[[VALUE25]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_bx]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_b]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_bx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_bx]]))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_b]])))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_b]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE27]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], read<complex<f64>>(%[[VALUE28]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_bx]], read<complex<f64>>(%[[VALUE28]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_bx]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_b]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_bx]])))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_bx]])))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_b]])))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_b]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = neg<f64>(const<f64>(0.0))));
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE29]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], read<complex<f64>>(%[[VALUE30]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_bx]], read<complex<f64>>(%[[VALUE30]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_bx]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_b]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_bx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_bx]]))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_b]])))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_b]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE31]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], read<complex<f64>>(%[[VALUE32]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_bx]], read<complex<f64>>(%[[VALUE31]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_bx]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_b]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_bx]])))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_bx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_b]]))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_b]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], complex_convert<complex<f64>, reason=assign>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0)))));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE33]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], read<complex<f64>>(%[[VALUE34]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_bx]], read<complex<f64>>(%[[VALUE33]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_bx]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_b]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_bx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_bx]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_b]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_b]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], real_to_complex<complex<f64>, reason=assign>(int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE35]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], read<complex<f64>>(%[[VALUE36]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_bx]], read<complex<f64>>(%[[VALUE36]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_bx]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_b]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_bx]]))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_bx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_b]]))))), float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_b]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], complex_convert<complex<f64>, reason=assign>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = neg<f32>(const<f32>(0.0)))));
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: complex<f64> [synthetic] = read<complex<f64>>(%[[VALUE_b]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE37]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_b]], read<complex<f64>>(%[[VALUE38]]));
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_bx]], read<complex<f64>>(%[[VALUE38]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_bx]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%[[VALUE_b]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_bx]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_bx]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_b]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_b]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE39]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], read<complex<f80>>(%[[VALUE40]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_cx]], read<complex<f80>>(%[[VALUE39]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_cx]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_c]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_cx]])))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_cx]])))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_c]])))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE41]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], read<complex<f80>>(%[[VALUE42]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_cx]], read<complex<f80>>(%[[VALUE41]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_cx]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_c]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_cx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_cx]]))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_c]])))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_c]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE43]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], read<complex<f80>>(%[[VALUE44]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_cx]], read<complex<f80>>(%[[VALUE44]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_cx]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_c]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_cx]])))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_cx]])))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_c]])))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: complex<f80> [synthetic] = add<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE45]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], read<complex<f80>>(%[[VALUE46]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_cx]], read<complex<f80>>(%[[VALUE46]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_cx]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_c]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_cx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_cx]]))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_c]])))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_c]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE47]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], read<complex<f80>>(%[[VALUE48]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_cx]], read<complex<f80>>(%[[VALUE47]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_cx]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_c]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_cx]])))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_cx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_c]]))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE49]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], read<complex<f80>>(%[[VALUE50]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_cx]], read<complex<f80>>(%[[VALUE49]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_cx]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_c]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_cx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_cx]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_c]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_c]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], real_to_complex<complex<f80>, reason=assign>(int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE51]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], read<complex<f80>>(%[[VALUE52]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_cx]], read<complex<f80>>(%[[VALUE52]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_cx]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_c]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_cx]]))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_cx]])))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_c]]))))), float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_c]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = neg<f80>(const<f80>(0))));
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: complex<f80> [synthetic] = read<complex<f80>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: complex<f80> [synthetic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE53]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_c]], read<complex<f80>>(%[[VALUE54]]));
// DEFAULT-NEXT:         write<complex<f80>>(%[[VALUE_cx]], read<complex<f80>>(%[[VALUE54]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_cx]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1)))), ne<complex<f80>, exceptions=ignore>(read<complex<f80>>(%[[VALUE_c]]), int_to_float<f80, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(1))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_cx]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_cx]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_creall]], read<complex<f80>>(%[[VALUE_c]]))))), not<bool>(float_class<bool, test=sign_bit>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE___builtin_cimagl]], read<complex<f80>>(%[[VALUE_c]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
