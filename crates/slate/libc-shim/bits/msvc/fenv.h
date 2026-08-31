#ifndef _SLATE_BITS_MSVC_FENV_H
#define _SLATE_BITS_MSVC_FENV_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/fenv.h> directly; include a public header instead."
#endif

#define FE_TONEAREST  0x0000
#define FE_UPWARD     0x0100
#define FE_DOWNWARD   0x0200
#define FE_TOWARDZERO 0x0300
#define FE_ROUND_MASK 0x0300

#define FE_INEXACT   0x01
#define FE_UNDERFLOW 0x02
#define FE_OVERFLOW  0x04
#define FE_DIVBYZERO 0x08
#define FE_INVALID   0x10
#define FE_ALL_EXCEPT                                                          \
  (FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)

typedef unsigned long fexcept_t;

typedef struct fenv_t {
  unsigned long _Fe_ctl;
  unsigned long _Fe_stat;
} fenv_t;

extern const fenv_t _Fenv0;

#define FE_DFL_ENV (&_Fenv0)

int fegetround(void);
int fesetround(int);
int fegetenv(fenv_t *);
int fesetenv(const fenv_t *);
int feclearexcept(int);
int feholdexcept(fenv_t *);
int fetestexcept(int);
int fegetexceptflag(fexcept_t *, int);
int fesetexceptflag(const fexcept_t *, int);
int feraiseexcept(int);
int feupdateenv(const fenv_t *);

#endif
