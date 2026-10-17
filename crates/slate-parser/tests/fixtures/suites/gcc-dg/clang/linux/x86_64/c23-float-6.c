/* Test SNAN macros.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors -fsignaling-nans" } */
/* { dg-add-options ieee } */

#include <float.h>

/* These should be defined if and only if signaling NaNs are supported
   for the given types.  If the testsuite gains effective-target
   support for targets not supporting signaling NaNs, or not
   supporting them for all types, this test should be made
   appropriately conditional.  */
#ifndef FLT_SNAN
#error "FLT_SNAN undefined"
#endif
#ifndef DBL_SNAN
#error "DBL_SNAN undefined"
#endif
#ifndef LDBL_SNAN
#error "LDBL_SNAN undefined"
#endif

volatile float f = FLT_SNAN;
volatile double d = DBL_SNAN;
volatile long double ld = LDBL_SNAN;

extern void abort (void);
extern void exit (int);

int
main (void)
{
  (void) _Generic (FLT_SNAN, float : 0);
  (void) _Generic (DBL_SNAN, double : 0);
  (void) _Generic (LDBL_SNAN, long double : 0);
  if (!__builtin_isnan (FLT_SNAN))
    abort ();
  if (!__builtin_isnan (f))
    abort ();
  if (!__builtin_isnan (DBL_SNAN))
    abort ();
  if (!__builtin_isnan (d))
    abort ();
  if (!__builtin_isnan (LDBL_SNAN))
    abort ();
  if (!__builtin_isnan (ld))
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
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: volatile f32 [storage=static] = const<f32>(bits=0x7fa00000) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: volatile f64 [storage=static] = const<f64>(bits=0x7ff4000000000000) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ld:[0-9]+]] ld: volatile f80 [storage=static] = const<f80>(bits=0x7fffa000000000000000) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         const<i32>(0);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(const<f32>(bits=0x7fa00000)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(read<f32, volatile>(%[[VALUE_f]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(const<f64>(bits=0x7ff4000000000000)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(read<f64, volatile>(%[[VALUE_d]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(const<f80>(bits=0x7fffa000000000000000)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(float_class<bool, test=nan>(read<f80, volatile>(%[[VALUE_ld]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
