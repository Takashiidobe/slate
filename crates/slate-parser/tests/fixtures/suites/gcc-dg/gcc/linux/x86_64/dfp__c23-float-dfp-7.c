/* Test DEC*_SNAN macros defined in <float.h> with DFP support.  */
/* { dg-options "-std=c23" } */

#include <float.h>

#ifndef DEC32_SNAN
# error "DEC32_SNAN not defined"
#endif

#ifndef DEC64_SNAN
# error "DEC64_SNAN not defined"
#endif

#ifndef DEC128_SNAN
# error "DEC128_SNAN not defined"
#endif

volatile _Decimal32 d32 = DEC32_SNAN;
volatile _Decimal64 d64 = DEC64_SNAN;
volatile _Decimal128 d128 = DEC128_SNAN;

extern void abort (void);
extern void exit (int);

int
main (void)
{
  (void) _Generic (DEC32_SNAN, _Decimal32 : 0);
  if (!__builtin_isnan (DEC32_SNAN))
    abort ();
  if (!__builtin_isnan (d32))
    abort ();
  (void) _Generic (DEC64_SNAN, _Decimal64 : 0);
  if (!__builtin_isnan (DEC64_SNAN))
    abort ();
  if (!__builtin_isnan (d64))
    abort ();
  (void) _Generic (DEC128_SNAN, _Decimal128 : 0);
  if (!__builtin_isnan (DEC128_SNAN))
    abort ();
  if (!__builtin_isnan (d128))
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
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd32]] @__builtin_nansd32(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> d32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd64]] @__builtin_nansd64(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> d64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansd128]] @__builtin_nansd128(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> d128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(call<d32, signature=fn(ptr<const i8>) -> d32>(%[[VALUE___builtin_nansd32]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_4]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(read<d32, volatile>(%[[VALUE_d32]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(call<d64, signature=fn(ptr<const i8>) -> d64>(%[[VALUE___builtin_nansd64]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_5]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(read<d64, volatile>(%[[VALUE_d64]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(call<d128, signature=fn(ptr<const i8>) -> d128>(%[[VALUE___builtin_nansd128]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_6]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(read<d128, volatile>(%[[VALUE_d128]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
