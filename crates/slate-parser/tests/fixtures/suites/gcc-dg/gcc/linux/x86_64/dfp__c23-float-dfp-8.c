/* Test DEC*_SNAN macros.  Test requiring runtime exceptions
   support.  */
/* { dg-do run } */
/* { dg-require-effective-target fenv_exceptions_dfp } */
/* { dg-options "-std=c23" } */

#include <fenv.h>
#include <float.h>

volatile _Decimal32 d32 = DEC32_SNAN;
volatile _Decimal64 d64 = DEC64_SNAN;
volatile _Decimal128 d128 = DEC128_SNAN;

extern void abort (void);
extern void exit (int);

int
main (void)
{
  feclearexcept (FE_ALL_EXCEPT);
  d32 += d32;
  if (!fetestexcept (FE_INVALID))
    abort ();
  feclearexcept (FE_ALL_EXCEPT);
  d32 += d32;
  if (fetestexcept (FE_INVALID))
    abort ();
  feclearexcept (FE_ALL_EXCEPT);
  d64 += d64;
  if (!fetestexcept (FE_INVALID))
    abort ();
  feclearexcept (FE_ALL_EXCEPT);
  d64 += d64;
  if (fetestexcept (FE_INVALID))
    abort ();
  feclearexcept (FE_ALL_EXCEPT);
  d128 += d128;
  if (!fetestexcept (FE_INVALID))
    abort ();
  feclearexcept (FE_ALL_EXCEPT);
  d128 += d128;
  if (fetestexcept (FE_INVALID))
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d32:[0-9]+]] d32: volatile d32 [storage=static] = call<d32, signature=fn(ptr<const i8>) -> d32>(%[[VALUE___builtin_nansd32:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d64:[0-9]+]] d64: volatile d64 [storage=static] = call<d64, signature=fn(ptr<const i8>) -> d64>(%[[VALUE___builtin_nansd64:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]]))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_d128:[0-9]+]] d128: volatile d128 [storage=static] = call<d128, signature=fn(ptr<const i8>) -> d128>(%[[VALUE___builtin_nansd128:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]]))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_feclearexcept:[0-9]+]] @feclearexcept(%[[VALUE___excepts:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fetestexcept:[0-9]+]] @fetestexcept(%[[VALUE___excepts_2:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd32]] @__builtin_nansd32(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> d32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd64]] @__builtin_nansd64(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> d64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd128]] @__builtin_nansd128(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_feclearexcept]], or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: d32 [synthetic] = read<d32, volatile>(%[[VALUE_d32]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: d32 [synthetic] = add<d32, rounding=nearest_even, exceptions=observable, contract=off>(read<d32>(%[[VALUE4]]), read<d32, volatile>(%[[VALUE_d32]]));
// DEFAULT-NEXT:         write<d32, volatile>(%[[VALUE_d32]], read<d32>(%[[VALUE5]]));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fetestexcept]], const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_feclearexcept]], or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: d32 [synthetic] = read<d32, volatile>(%[[VALUE_d32]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: d32 [synthetic] = add<d32, rounding=nearest_even, exceptions=observable, contract=off>(read<d32>(%[[VALUE6]]), read<d32, volatile>(%[[VALUE_d32]]));
// DEFAULT-NEXT:         write<d32, volatile>(%[[VALUE_d32]], read<d32>(%[[VALUE7]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fetestexcept]], const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_feclearexcept]], or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: d64 [synthetic] = read<d64, volatile>(%[[VALUE_d64]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: d64 [synthetic] = add<d64, rounding=nearest_even, exceptions=observable, contract=off>(read<d64>(%[[VALUE8]]), read<d64, volatile>(%[[VALUE_d64]]));
// DEFAULT-NEXT:         write<d64, volatile>(%[[VALUE_d64]], read<d64>(%[[VALUE9]]));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fetestexcept]], const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_feclearexcept]], or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: d64 [synthetic] = read<d64, volatile>(%[[VALUE_d64]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: d64 [synthetic] = add<d64, rounding=nearest_even, exceptions=observable, contract=off>(read<d64>(%[[VALUE10]]), read<d64, volatile>(%[[VALUE_d64]]));
// DEFAULT-NEXT:         write<d64, volatile>(%[[VALUE_d64]], read<d64>(%[[VALUE11]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fetestexcept]], const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_feclearexcept]], or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: d128 [synthetic] = read<d128, volatile>(%[[VALUE_d128]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: d128 [synthetic] = add<d128, rounding=nearest_even, exceptions=observable, contract=off>(read<d128>(%[[VALUE12]]), read<d128, volatile>(%[[VALUE_d128]]));
// DEFAULT-NEXT:         write<d128, volatile>(%[[VALUE_d128]], read<d128>(%[[VALUE13]]));
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fetestexcept]], const<i32>(1)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_feclearexcept]], or<i32>(or<i32>(or<i32>(or<i32>(const<i32>(32), const<i32>(4)), const<i32>(16)), const<i32>(8)), const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: d128 [synthetic] = read<d128, volatile>(%[[VALUE_d128]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: d128 [synthetic] = add<d128, rounding=nearest_even, exceptions=observable, contract=off>(read<d128>(%[[VALUE14]]), read<d128, volatile>(%[[VALUE_d128]]));
// DEFAULT-NEXT:         write<d128, volatile>(%[[VALUE_d128]], read<d128>(%[[VALUE15]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fetestexcept]], const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
