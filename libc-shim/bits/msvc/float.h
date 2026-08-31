#ifndef _SLATE_BITS_MSVC_FLOAT_H
#define _SLATE_BITS_MSVC_FLOAT_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/float.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

#undef DBL_NORM_MAX
#undef DBL_SNAN
#undef FLT_NORM_MAX
#undef FLT_SNAN
#undef LDBL_DECIMAL_DIG
#undef LDBL_NORM_MAX
#undef LDBL_SNAN
#undef FLT_ROUNDS

#define _DBL_RADIX  2
#define FLT_GUARD   0
#define FLT_NORMALIZE 0
#define _LDBL_RADIX _DBL_RADIX

#define _SW_INEXACT    0x00000001
#define _SW_UNDERFLOW  0x00000002
#define _SW_OVERFLOW   0x00000004
#define _SW_ZERODIVIDE 0x00000008
#define _SW_INVALID    0x00000010
#define _SW_DENORMAL   0x00080000

#define _EM_AMBIGUOUS 0x80000000
#define _MCW_EM       0x0008001f
#define _EM_INEXACT   0x00000001
#define _EM_UNDERFLOW 0x00000002
#define _EM_OVERFLOW  0x00000004
#define _EM_ZERODIVIDE 0x00000008
#define _EM_INVALID    0x00000010
#define _EM_DENORMAL   0x00080000

#define _MCW_RC  0x00000300
#define _RC_NEAR 0x00000000
#define _RC_DOWN 0x00000100
#define _RC_UP   0x00000200
#define _RC_CHOP 0x00000300

#define _MCW_PC 0x00030000
#define _PC_64  0x00000000
#define _PC_53  0x00010000
#define _PC_24  0x00020000
#define _MCW_IC 0x00040000
#define _IC_AFFINE     0x00040000
#define _IC_PROJECTIVE 0x00000000

#define _MCW_DN                         0x03000000
#define _DN_SAVE                        0x00000000
#define _DN_FLUSH                       0x01000000
#define _DN_FLUSH_OPERANDS_SAVE_RESULTS 0x02000000
#define _DN_SAVE_OPERANDS_FLUSH_RESULTS 0x03000000

#define _SW_UNEMULATED      0x0040
#define _SW_SQRTNEG         0x0080
#define _SW_STACKOVERFLOW   0x0200
#define _SW_STACKUNDERFLOW  0x0400

#define _FPE_INVALID         0x81
#define _FPE_DENORMAL        0x82
#define _FPE_ZERODIVIDE      0x83
#define _FPE_OVERFLOW        0x84
#define _FPE_UNDERFLOW       0x85
#define _FPE_INEXACT         0x86
#define _FPE_UNEMULATED      0x87
#define _FPE_SQRTNEG         0x88
#define _FPE_STACKOVERFLOW   0x8a
#define _FPE_STACKUNDERFLOW  0x8b
#define _FPE_EXPLICITGEN     0x8c
#define _FPE_MULTIPLE_TRAPS  0x8d
#define _FPE_MULTIPLE_FAULTS 0x8e

#define _FPCLASS_SNAN 0x0001
#define _FPCLASS_QNAN 0x0002
#define _FPCLASS_NINF 0x0004
#define _FPCLASS_NN   0x0008
#define _FPCLASS_ND   0x0010
#define _FPCLASS_NZ   0x0020
#define _FPCLASS_PZ   0x0040
#define _FPCLASS_PD   0x0080
#define _FPCLASS_PN   0x0100
#define _FPCLASS_PINF 0x0200

#define _CW_DEFAULT                                                            \
  (_RC_NEAR + _EM_INVALID + _EM_ZERODIVIDE + _EM_OVERFLOW + _EM_UNDERFLOW +  \
   _EM_INEXACT + _EM_DENORMAL)

unsigned int _clearfp(void);
unsigned int _controlfp(unsigned int, unsigned int);
void         _set_controlfp(unsigned int, unsigned int);
unsigned int _statusfp(void);
void         _fpreset(void);
unsigned int _control87(unsigned int, unsigned int);
int         *__fpecode(void);
int          __fpe_flt_rounds(void);

#define FLT_ROUNDS   (__fpe_flt_rounds())
#define _DBL_ROUNDS  FLT_ROUNDS
#define _LDBL_ROUNDS _DBL_ROUNDS

double _scalb(double, long);
double _logb(double);
double _nextafter(double, double);
int    _finite(double);
int    _isnan(double);
int    _fpclass(double);
float  _scalbf(float, long);

#include <bits/msvc/secure/float.h>

#endif
