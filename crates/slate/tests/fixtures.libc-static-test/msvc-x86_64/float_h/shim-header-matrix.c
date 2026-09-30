#include <float.h>

extern int slate_oracle___fpe_flt_rounds(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___fpe_flt_rounds), __typeof__(__fpe_flt_rounds)),
    "float.h:__fpe_flt_rounds declaration differs from oracle");

static __typeof__(__fpe_flt_rounds) *const slate_reference___fpe_flt_rounds = &__fpe_flt_rounds;

extern int * slate_oracle___fpecode(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___fpecode), __typeof__(__fpecode)),
    "float.h:__fpecode declaration differs from oracle");

static __typeof__(__fpecode) *const slate_reference___fpecode = &__fpecode;

extern double slate_oracle__chgsign(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__chgsign), __typeof__(_chgsign)),
    "float.h:_chgsign declaration differs from oracle");

static __typeof__(_chgsign) *const slate_reference__chgsign = &_chgsign;

extern unsigned int slate_oracle__clearfp(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__clearfp), __typeof__(_clearfp)),
    "float.h:_clearfp declaration differs from oracle");

static __typeof__(_clearfp) *const slate_reference__clearfp = &_clearfp;

extern unsigned int slate_oracle__control87(unsigned int, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__control87), __typeof__(_control87)),
    "float.h:_control87 declaration differs from oracle");

static __typeof__(_control87) *const slate_reference__control87 = &_control87;

extern unsigned int slate_oracle__controlfp(unsigned int, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__controlfp), __typeof__(_controlfp)),
    "float.h:_controlfp declaration differs from oracle");

static __typeof__(_controlfp) *const slate_reference__controlfp = &_controlfp;

extern int slate_oracle__controlfp_s(unsigned int *, unsigned int, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__controlfp_s), __typeof__(_controlfp_s)),
    "float.h:_controlfp_s declaration differs from oracle");

static __typeof__(_controlfp_s) *const slate_reference__controlfp_s = &_controlfp_s;

extern double slate_oracle__copysign(double, double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__copysign), __typeof__(_copysign)),
    "float.h:_copysign declaration differs from oracle");

static __typeof__(_copysign) *const slate_reference__copysign = &_copysign;

extern int slate_oracle__finite(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__finite), __typeof__(_finite)),
    "float.h:_finite declaration differs from oracle");

static __typeof__(_finite) *const slate_reference__finite = &_finite;

extern int slate_oracle__fpclass(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fpclass), __typeof__(_fpclass)),
    "float.h:_fpclass declaration differs from oracle");

static __typeof__(_fpclass) *const slate_reference__fpclass = &_fpclass;

extern void slate_oracle__fpreset(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fpreset), __typeof__(_fpreset)),
    "float.h:_fpreset declaration differs from oracle");

static __typeof__(_fpreset) *const slate_reference__fpreset = &_fpreset;

extern int slate_oracle__isnan(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__isnan), __typeof__(_isnan)),
    "float.h:_isnan declaration differs from oracle");

static __typeof__(_isnan) *const slate_reference__isnan = &_isnan;

extern double slate_oracle__logb(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__logb), __typeof__(_logb)),
    "float.h:_logb declaration differs from oracle");

static __typeof__(_logb) *const slate_reference__logb = &_logb;

extern double slate_oracle__nextafter(double, double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__nextafter), __typeof__(_nextafter)),
    "float.h:_nextafter declaration differs from oracle");

static __typeof__(_nextafter) *const slate_reference__nextafter = &_nextafter;

extern double slate_oracle__scalb(double, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__scalb), __typeof__(_scalb)),
    "float.h:_scalb declaration differs from oracle");

static __typeof__(_scalb) *const slate_reference__scalb = &_scalb;

extern float slate_oracle__scalbf(float, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__scalbf), __typeof__(_scalbf)),
    "float.h:_scalbf declaration differs from oracle");

static __typeof__(_scalbf) *const slate_reference__scalbf = &_scalbf;

extern void slate_oracle__set_controlfp(unsigned int, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_controlfp), __typeof__(_set_controlfp)),
    "float.h:_set_controlfp declaration differs from oracle");

static __typeof__(_set_controlfp) *const slate_reference__set_controlfp = &_set_controlfp;

extern unsigned int slate_oracle__statusfp(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__statusfp), __typeof__(_statusfp)),
    "float.h:_statusfp declaration differs from oracle");

static __typeof__(_statusfp) *const slate_reference__statusfp = &_statusfp;

extern void slate_oracle_fpreset(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fpreset), __typeof__(fpreset)),
    "float.h:fpreset declaration differs from oracle");

static __typeof__(fpreset) *const slate_reference_fpreset = &fpreset;

#ifndef CW_DEFAULT
#error "float.h:CW_DEFAULT macro is missing from libc-shim"
#endif

#ifndef DBL_DECIMAL_DIG
#error "float.h:DBL_DECIMAL_DIG macro is missing from libc-shim"
#endif

#ifndef DBL_DIG
#error "float.h:DBL_DIG macro is missing from libc-shim"
#endif

#ifndef DBL_EPSILON
#error "float.h:DBL_EPSILON macro is missing from libc-shim"
#endif

#ifndef DBL_HAS_SUBNORM
#error "float.h:DBL_HAS_SUBNORM macro is missing from libc-shim"
#endif

#ifndef DBL_MANT_DIG
#error "float.h:DBL_MANT_DIG macro is missing from libc-shim"
#endif

#ifndef DBL_MAX
#error "float.h:DBL_MAX macro is missing from libc-shim"
#endif

#ifndef DBL_MAX_10_EXP
#error "float.h:DBL_MAX_10_EXP macro is missing from libc-shim"
#endif

#ifndef DBL_MAX_EXP
#error "float.h:DBL_MAX_EXP macro is missing from libc-shim"
#endif

#ifndef DBL_MIN
#error "float.h:DBL_MIN macro is missing from libc-shim"
#endif

#ifndef DBL_MIN_10_EXP
#error "float.h:DBL_MIN_10_EXP macro is missing from libc-shim"
#endif

#ifndef DBL_MIN_EXP
#error "float.h:DBL_MIN_EXP macro is missing from libc-shim"
#endif

#ifndef DBL_RADIX
#error "float.h:DBL_RADIX macro is missing from libc-shim"
#endif

#ifndef DBL_ROUNDS
#error "float.h:DBL_ROUNDS macro is missing from libc-shim"
#endif

#ifndef DBL_TRUE_MIN
#error "float.h:DBL_TRUE_MIN macro is missing from libc-shim"
#endif

#ifndef DECIMAL_DIG
#error "float.h:DECIMAL_DIG macro is missing from libc-shim"
#endif

#ifndef EM_AMBIGUIOUS
#error "float.h:EM_AMBIGUIOUS macro is missing from libc-shim"
#endif

#ifndef EM_AMBIGUOUS
#error "float.h:EM_AMBIGUOUS macro is missing from libc-shim"
#endif

#ifndef EM_DENORMAL
#error "float.h:EM_DENORMAL macro is missing from libc-shim"
#endif

#ifndef EM_INEXACT
#error "float.h:EM_INEXACT macro is missing from libc-shim"
#endif

#ifndef EM_INVALID
#error "float.h:EM_INVALID macro is missing from libc-shim"
#endif

#ifndef EM_OVERFLOW
#error "float.h:EM_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef EM_UNDERFLOW
#error "float.h:EM_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef EM_ZERODIVIDE
#error "float.h:EM_ZERODIVIDE macro is missing from libc-shim"
#endif

#ifndef FLT_DECIMAL_DIG
#error "float.h:FLT_DECIMAL_DIG macro is missing from libc-shim"
#endif

#ifndef FLT_DIG
#error "float.h:FLT_DIG macro is missing from libc-shim"
#endif

#ifndef FLT_EPSILON
#error "float.h:FLT_EPSILON macro is missing from libc-shim"
#endif

#ifndef FLT_EVAL_METHOD
#error "float.h:FLT_EVAL_METHOD macro is missing from libc-shim"
#endif

#ifndef FLT_GUARD
#error "float.h:FLT_GUARD macro is missing from libc-shim"
#endif

#ifndef FLT_HAS_SUBNORM
#error "float.h:FLT_HAS_SUBNORM macro is missing from libc-shim"
#endif

#ifndef FLT_MANT_DIG
#error "float.h:FLT_MANT_DIG macro is missing from libc-shim"
#endif

#ifndef FLT_MAX
#error "float.h:FLT_MAX macro is missing from libc-shim"
#endif

#ifndef FLT_MAX_10_EXP
#error "float.h:FLT_MAX_10_EXP macro is missing from libc-shim"
#endif

#ifndef FLT_MAX_EXP
#error "float.h:FLT_MAX_EXP macro is missing from libc-shim"
#endif

#ifndef FLT_MIN
#error "float.h:FLT_MIN macro is missing from libc-shim"
#endif

#ifndef FLT_MIN_10_EXP
#error "float.h:FLT_MIN_10_EXP macro is missing from libc-shim"
#endif

#ifndef FLT_MIN_EXP
#error "float.h:FLT_MIN_EXP macro is missing from libc-shim"
#endif

#ifndef FLT_NORMALIZE
#error "float.h:FLT_NORMALIZE macro is missing from libc-shim"
#endif

#ifndef FLT_RADIX
#error "float.h:FLT_RADIX macro is missing from libc-shim"
#endif

#ifndef FLT_ROUNDS
#error "float.h:FLT_ROUNDS macro is missing from libc-shim"
#endif

#ifndef FLT_TRUE_MIN
#error "float.h:FLT_TRUE_MIN macro is missing from libc-shim"
#endif

#ifndef FPE_DENORMAL
#error "float.h:FPE_DENORMAL macro is missing from libc-shim"
#endif

#ifndef FPE_EXPLICITGEN
#error "float.h:FPE_EXPLICITGEN macro is missing from libc-shim"
#endif

#ifndef FPE_INEXACT
#error "float.h:FPE_INEXACT macro is missing from libc-shim"
#endif

#ifndef FPE_INVALID
#error "float.h:FPE_INVALID macro is missing from libc-shim"
#endif

#ifndef FPE_OVERFLOW
#error "float.h:FPE_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef FPE_SQRTNEG
#error "float.h:FPE_SQRTNEG macro is missing from libc-shim"
#endif

#ifndef FPE_STACKOVERFLOW
#error "float.h:FPE_STACKOVERFLOW macro is missing from libc-shim"
#endif

#ifndef FPE_STACKUNDERFLOW
#error "float.h:FPE_STACKUNDERFLOW macro is missing from libc-shim"
#endif

#ifndef FPE_UNDERFLOW
#error "float.h:FPE_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef FPE_UNEMULATED
#error "float.h:FPE_UNEMULATED macro is missing from libc-shim"
#endif

#ifndef FPE_ZERODIVIDE
#error "float.h:FPE_ZERODIVIDE macro is missing from libc-shim"
#endif

#ifndef IC_AFFINE
#error "float.h:IC_AFFINE macro is missing from libc-shim"
#endif

#ifndef IC_PROJECTIVE
#error "float.h:IC_PROJECTIVE macro is missing from libc-shim"
#endif

#ifndef LDBL_DIG
#error "float.h:LDBL_DIG macro is missing from libc-shim"
#endif

#ifndef LDBL_EPSILON
#error "float.h:LDBL_EPSILON macro is missing from libc-shim"
#endif

#ifndef LDBL_HAS_SUBNORM
#error "float.h:LDBL_HAS_SUBNORM macro is missing from libc-shim"
#endif

#ifndef LDBL_MANT_DIG
#error "float.h:LDBL_MANT_DIG macro is missing from libc-shim"
#endif

#ifndef LDBL_MAX
#error "float.h:LDBL_MAX macro is missing from libc-shim"
#endif

#ifndef LDBL_MAX_10_EXP
#error "float.h:LDBL_MAX_10_EXP macro is missing from libc-shim"
#endif

#ifndef LDBL_MAX_EXP
#error "float.h:LDBL_MAX_EXP macro is missing from libc-shim"
#endif

#ifndef LDBL_MIN
#error "float.h:LDBL_MIN macro is missing from libc-shim"
#endif

#ifndef LDBL_MIN_10_EXP
#error "float.h:LDBL_MIN_10_EXP macro is missing from libc-shim"
#endif

#ifndef LDBL_MIN_EXP
#error "float.h:LDBL_MIN_EXP macro is missing from libc-shim"
#endif

#ifndef LDBL_RADIX
#error "float.h:LDBL_RADIX macro is missing from libc-shim"
#endif

#ifndef LDBL_ROUNDS
#error "float.h:LDBL_ROUNDS macro is missing from libc-shim"
#endif

#ifndef LDBL_TRUE_MIN
#error "float.h:LDBL_TRUE_MIN macro is missing from libc-shim"
#endif

#ifndef MCW_EM
#error "float.h:MCW_EM macro is missing from libc-shim"
#endif

#ifndef MCW_IC
#error "float.h:MCW_IC macro is missing from libc-shim"
#endif

#ifndef MCW_PC
#error "float.h:MCW_PC macro is missing from libc-shim"
#endif

#ifndef MCW_RC
#error "float.h:MCW_RC macro is missing from libc-shim"
#endif

#ifndef PC_24
#error "float.h:PC_24 macro is missing from libc-shim"
#endif

#ifndef PC_53
#error "float.h:PC_53 macro is missing from libc-shim"
#endif

#ifndef PC_64
#error "float.h:PC_64 macro is missing from libc-shim"
#endif

#ifndef RC_CHOP
#error "float.h:RC_CHOP macro is missing from libc-shim"
#endif

#ifndef RC_DOWN
#error "float.h:RC_DOWN macro is missing from libc-shim"
#endif

#ifndef RC_NEAR
#error "float.h:RC_NEAR macro is missing from libc-shim"
#endif

#ifndef RC_UP
#error "float.h:RC_UP macro is missing from libc-shim"
#endif

#ifndef SW_DENORMAL
#error "float.h:SW_DENORMAL macro is missing from libc-shim"
#endif

#ifndef SW_INEXACT
#error "float.h:SW_INEXACT macro is missing from libc-shim"
#endif

#ifndef SW_INVALID
#error "float.h:SW_INVALID macro is missing from libc-shim"
#endif

#ifndef SW_OVERFLOW
#error "float.h:SW_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef SW_SQRTNEG
#error "float.h:SW_SQRTNEG macro is missing from libc-shim"
#endif

#ifndef SW_STACKOVERFLOW
#error "float.h:SW_STACKOVERFLOW macro is missing from libc-shim"
#endif

#ifndef SW_STACKUNDERFLOW
#error "float.h:SW_STACKUNDERFLOW macro is missing from libc-shim"
#endif

#ifndef SW_UNDERFLOW
#error "float.h:SW_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef SW_UNEMULATED
#error "float.h:SW_UNEMULATED macro is missing from libc-shim"
#endif

#ifndef SW_ZERODIVIDE
#error "float.h:SW_ZERODIVIDE macro is missing from libc-shim"
#endif

#ifndef _CRT_MANAGED_FP_DEPRECATE
#error "float.h:_CRT_MANAGED_FP_DEPRECATE macro is missing from libc-shim"
#endif

#ifndef _CW_DEFAULT
#error "float.h:_CW_DEFAULT macro is missing from libc-shim"
#endif

#ifndef _DBL_RADIX
#error "float.h:_DBL_RADIX macro is missing from libc-shim"
#endif

#ifndef _DBL_ROUNDS
#error "float.h:_DBL_ROUNDS macro is missing from libc-shim"
#endif

#ifndef _DN_FLUSH
#error "float.h:_DN_FLUSH macro is missing from libc-shim"
#endif

#ifndef _DN_FLUSH_OPERANDS_SAVE_RESULTS
#error "float.h:_DN_FLUSH_OPERANDS_SAVE_RESULTS macro is missing from libc-shim"
#endif

#ifndef _DN_SAVE
#error "float.h:_DN_SAVE macro is missing from libc-shim"
#endif

#ifndef _DN_SAVE_OPERANDS_FLUSH_RESULTS
#error "float.h:_DN_SAVE_OPERANDS_FLUSH_RESULTS macro is missing from libc-shim"
#endif

#ifndef _EM_AMBIGUIOUS
#error "float.h:_EM_AMBIGUIOUS macro is missing from libc-shim"
#endif

#ifndef _EM_AMBIGUOUS
#error "float.h:_EM_AMBIGUOUS macro is missing from libc-shim"
#endif

#ifndef _EM_DENORMAL
#error "float.h:_EM_DENORMAL macro is missing from libc-shim"
#endif

#ifndef _EM_INEXACT
#error "float.h:_EM_INEXACT macro is missing from libc-shim"
#endif

#ifndef _EM_INVALID
#error "float.h:_EM_INVALID macro is missing from libc-shim"
#endif

#ifndef _EM_OVERFLOW
#error "float.h:_EM_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef _EM_UNDERFLOW
#error "float.h:_EM_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef _EM_ZERODIVIDE
#error "float.h:_EM_ZERODIVIDE macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_ND
#error "float.h:_FPCLASS_ND macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_NINF
#error "float.h:_FPCLASS_NINF macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_NN
#error "float.h:_FPCLASS_NN macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_NZ
#error "float.h:_FPCLASS_NZ macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_PD
#error "float.h:_FPCLASS_PD macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_PINF
#error "float.h:_FPCLASS_PINF macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_PN
#error "float.h:_FPCLASS_PN macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_PZ
#error "float.h:_FPCLASS_PZ macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_QNAN
#error "float.h:_FPCLASS_QNAN macro is missing from libc-shim"
#endif

#ifndef _FPCLASS_SNAN
#error "float.h:_FPCLASS_SNAN macro is missing from libc-shim"
#endif

#ifndef _FPE_DENORMAL
#error "float.h:_FPE_DENORMAL macro is missing from libc-shim"
#endif

#ifndef _FPE_EXPLICITGEN
#error "float.h:_FPE_EXPLICITGEN macro is missing from libc-shim"
#endif

#ifndef _FPE_INEXACT
#error "float.h:_FPE_INEXACT macro is missing from libc-shim"
#endif

#ifndef _FPE_INVALID
#error "float.h:_FPE_INVALID macro is missing from libc-shim"
#endif

#ifndef _FPE_MULTIPLE_FAULTS
#error "float.h:_FPE_MULTIPLE_FAULTS macro is missing from libc-shim"
#endif

#ifndef _FPE_MULTIPLE_TRAPS
#error "float.h:_FPE_MULTIPLE_TRAPS macro is missing from libc-shim"
#endif

#ifndef _FPE_OVERFLOW
#error "float.h:_FPE_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef _FPE_SQRTNEG
#error "float.h:_FPE_SQRTNEG macro is missing from libc-shim"
#endif

#ifndef _FPE_STACKOVERFLOW
#error "float.h:_FPE_STACKOVERFLOW macro is missing from libc-shim"
#endif

#ifndef _FPE_STACKUNDERFLOW
#error "float.h:_FPE_STACKUNDERFLOW macro is missing from libc-shim"
#endif

#ifndef _FPE_UNDERFLOW
#error "float.h:_FPE_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef _FPE_UNEMULATED
#error "float.h:_FPE_UNEMULATED macro is missing from libc-shim"
#endif

#ifndef _FPE_ZERODIVIDE
#error "float.h:_FPE_ZERODIVIDE macro is missing from libc-shim"
#endif

#ifndef _IC_AFFINE
#error "float.h:_IC_AFFINE macro is missing from libc-shim"
#endif

#ifndef _IC_PROJECTIVE
#error "float.h:_IC_PROJECTIVE macro is missing from libc-shim"
#endif

#ifndef _INC_FLOAT
#error "float.h:_INC_FLOAT macro is missing from libc-shim"
#endif

#ifndef _LDBL_RADIX
#error "float.h:_LDBL_RADIX macro is missing from libc-shim"
#endif

#ifndef _LDBL_ROUNDS
#error "float.h:_LDBL_ROUNDS macro is missing from libc-shim"
#endif

#ifndef _MCW_DN
#error "float.h:_MCW_DN macro is missing from libc-shim"
#endif

#ifndef _MCW_EM
#error "float.h:_MCW_EM macro is missing from libc-shim"
#endif

#ifndef _MCW_IC
#error "float.h:_MCW_IC macro is missing from libc-shim"
#endif

#ifndef _MCW_PC
#error "float.h:_MCW_PC macro is missing from libc-shim"
#endif

#ifndef _MCW_RC
#error "float.h:_MCW_RC macro is missing from libc-shim"
#endif

#ifndef _PC_24
#error "float.h:_PC_24 macro is missing from libc-shim"
#endif

#ifndef _PC_53
#error "float.h:_PC_53 macro is missing from libc-shim"
#endif

#ifndef _PC_64
#error "float.h:_PC_64 macro is missing from libc-shim"
#endif

#ifndef _RC_CHOP
#error "float.h:_RC_CHOP macro is missing from libc-shim"
#endif

#ifndef _RC_DOWN
#error "float.h:_RC_DOWN macro is missing from libc-shim"
#endif

#ifndef _RC_NEAR
#error "float.h:_RC_NEAR macro is missing from libc-shim"
#endif

#ifndef _RC_UP
#error "float.h:_RC_UP macro is missing from libc-shim"
#endif

#ifndef _SW_DENORMAL
#error "float.h:_SW_DENORMAL macro is missing from libc-shim"
#endif

#ifndef _SW_INEXACT
#error "float.h:_SW_INEXACT macro is missing from libc-shim"
#endif

#ifndef _SW_INVALID
#error "float.h:_SW_INVALID macro is missing from libc-shim"
#endif

#ifndef _SW_OVERFLOW
#error "float.h:_SW_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef _SW_SQRTNEG
#error "float.h:_SW_SQRTNEG macro is missing from libc-shim"
#endif

#ifndef _SW_STACKOVERFLOW
#error "float.h:_SW_STACKOVERFLOW macro is missing from libc-shim"
#endif

#ifndef _SW_STACKUNDERFLOW
#error "float.h:_SW_STACKUNDERFLOW macro is missing from libc-shim"
#endif

#ifndef _SW_UNDERFLOW
#error "float.h:_SW_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef _SW_UNEMULATED
#error "float.h:_SW_UNEMULATED macro is missing from libc-shim"
#endif

#ifndef _SW_ZERODIVIDE
#error "float.h:_SW_ZERODIVIDE macro is missing from libc-shim"
#endif

#ifndef _clear87
#error "float.h:_clear87 macro is missing from libc-shim"
#endif

#ifndef _fpecode
#error "float.h:_fpecode macro is missing from libc-shim"
#endif

#ifndef _status87
#error "float.h:_status87 macro is missing from libc-shim"
#endif

#ifndef clear87
#error "float.h:clear87 macro is missing from libc-shim"
#endif

#ifndef control87
#error "float.h:control87 macro is missing from libc-shim"
#endif

#ifndef status87
#error "float.h:status87 macro is missing from libc-shim"
#endif

int main(void) { return 0; }
