#ifndef _SLATE_FENV_H
#define _SLATE_FENV_H

#define __STDC_VERSION_FENV_H__ 202311L

#define FE_INVALID 0x01
#define FE_DIVBYZERO 0x04
#define FE_OVERFLOW 0x08
#define FE_UNDERFLOW 0x10
#define FE_INEXACT 0x20
#define FE_ALL_EXCEPT                                                          \
  (FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

#define FE_TONEAREST 0
#define FE_DOWNWARD 0x400
#define FE_UPWARD 0x800
#define FE_TOWARDZERO 0xc00

typedef unsigned short fexcept_t;

/* Layout matches the real glibc x86-64 fenv_t (fstenv block plus MXCSR)
   since fegetenv/fesetenv/feholdexcept/feupdateenv are real glibc symbols
   that read and write it directly. */
typedef struct {
  unsigned short __control_word;
  unsigned short __glibc_reserved1;
  unsigned short __status_word;
  unsigned short __glibc_reserved2;
  unsigned short __tags;
  unsigned short __glibc_reserved3;
  unsigned int __eip;
  unsigned short __cs_selector;
  unsigned int __opcode : 11;
  unsigned int __glibc_reserved4 : 5;
  unsigned int __data_offset;
  unsigned short __data_selector;
  unsigned short __glibc_reserved5;
  unsigned int __mxcsr;
} fenv_t;

#define FE_DFL_ENV ((const fenv_t *)-1)

/* Layout matches the real glibc x86-64 femode_t (control word + MXCSR). */
typedef struct {
  unsigned short __control_word;
  unsigned short __glibc_reserved;
  unsigned int __mxcsr;
} femode_t;

#define FE_DFL_MODE ((const femode_t *)-1L)

int feclearexcept(int excepts);
int fegetexceptflag(fexcept_t *flagp, int excepts);
int feraiseexcept(int excepts);
int fesetexcept(int excepts);
int fesetexceptflag(const fexcept_t *flagp, int excepts);
int fetestexcept(int excepts);
int fetestexceptflag(const fexcept_t *flagp, int excepts);

int fegetround(void);
int fesetround(int rounding_direction);

int fegetenv(fenv_t *envp);
int feholdexcept(fenv_t *envp);
int fesetenv(const fenv_t *envp);
int feupdateenv(const fenv_t *envp);

int fegetmode(femode_t *modep);
int fesetmode(const femode_t *modep);

#endif
