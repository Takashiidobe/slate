#if !defined(_SLATE_LIBC)
#error "Never include <bits/aarch64/fenv.h> directly; include a public header instead."
#endif

#define FE_INVALID    1
#define FE_DIVBYZERO  2
#define FE_OVERFLOW   4
#define FE_UNDERFLOW  8
#define FE_INEXACT    16
#define FE_ALL_EXCEPT 31
#define FE_TONEAREST  0
#define FE_DOWNWARD   0x800000
#define FE_UPWARD     0x400000
#define FE_TOWARDZERO 0xc00000

typedef unsigned int fexcept_t;

typedef struct fenv_t {
  unsigned int __fpcr;
  unsigned int __fpsr;
} fenv_t;

#define FE_DFL_ENV ((const fenv_t *)-1)

#if !defined(__SLATE_LIBC_GLIBC)
typedef struct femode_t {
  unsigned int __fpcr;
} femode_t;
#endif

#define FE_DFL_MODE ((const femode_t *)-1L)

#if defined(__SLATE_LIBC_GLIBC)
typedef unsigned int femode_t;
#define FE_EXCEPT_SHIFT 0
#define FE_NOMASK_ENV ((const fenv_t *)-1)
#endif
