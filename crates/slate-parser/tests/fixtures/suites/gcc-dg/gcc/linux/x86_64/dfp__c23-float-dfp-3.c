/* Test DFP macros defined in <float.h> with DFP support.  TR 24732
   feature test macro causes *_SUBNORMAL_MIN macros to be defined.  */
/* { dg-do compile } */
/* { dg-options "-std=c23" } */

#define __STDC_WANT_DEC_FP__
#include <float.h>

#ifndef DEC32_MANT_DIG
# error "DEC32_MANT_DIG not defined"
#endif

#ifndef DEC64_MANT_DIG
# error "DEC64_MANT_DIG not defined"
#endif

#ifndef DEC128_MANT_DIG
# error "DEC128_MANT_DIG not defined"
#endif

#ifndef DEC32_MIN_EXP
# error "DEC32_MIN_EXP not defined"
#endif

#ifndef DEC64_MIN_EXP
# error "DEC64_MIN_EXP not defined"
#endif

#ifndef DEC128_MIN_EXP
# error "DEC128_MIN_EXP not defined"
#endif

#ifndef DEC32_MAX_EXP
# error "DEC32_MAX_EXP not defined"
#endif

#ifndef DEC64_MAX_EXP
# error "DEC64_MAX_EXP not defined"
#endif

#ifndef DEC128_MAX_EXP
# error "DEC128_MAX_EXP not defined"
#endif

#ifndef DEC32_MAX
# error "DEC32_MAX not defined"
#endif

#ifndef DEC64_MAX
# error "DEC64_MAX not defined"
#endif

#ifndef DEC128_MAX
# error "DEC128_MAX not defined"
#endif

#ifndef DEC32_EPSILON
# error "DEC32_EPSILON not defined"
#endif

#ifndef DEC64_EPSILON
# error "DEC64_EPSILON not defined"
#endif

#ifndef DEC128_EPSILON
# error "DEC128_EPSILON not defined"
#endif

#ifndef DEC32_MIN
# error "DEC32_MIN not defined"
#endif

#ifndef DEC64_MIN
# error "DEC64_MIN not defined"
#endif

#ifndef DEC128_MIN
# error "DEC128_MIN not defined"
#endif

#ifndef DEC32_TRUE_MIN
# error "DEC32_TRUE_MIN not defined"
#endif

#ifndef DEC64_TRUE_MIN
# error "DEC64_TRUE_MIN not defined"
#endif

#ifndef DEC128_TRUE_MIN
# error "DEC128_TRUE_MIN not defined"
#endif

#ifndef DEC32_SUBNORMAL_MIN
# error "DEC32_SUBNORMAL_MIN not defined"
#endif

#ifndef DEC64_SUBNORMAL_MIN
# error "DEC64_SUBNORMAL_MIN not defined"
#endif

#ifndef DEC128_SUBNORMAL_MIN
# error "DEC128_SUBNORMAL_MIN not defined"
#endif

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
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
