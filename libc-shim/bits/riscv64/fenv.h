#if !defined(_SLATE_LIBC)
#error "Never include <bits/riscv64/fenv.h> directly; include a public header instead."
#endif

#ifdef __riscv_float_abi_soft
enum {
  FE_ALL_EXCEPT = 0,
  FE_TONEAREST  = 0,
};
#else
enum {
  FE_INEXACT    = 1,
  FE_UNDERFLOW  = 2,
  FE_OVERFLOW   = 4,
  FE_DIVBYZERO  = 8,
  FE_INVALID    = 16,
  FE_ALL_EXCEPT = 31,
};

enum {
  FE_TONEAREST  = 0,
  FE_TOWARDZERO = 1,
  FE_DOWNWARD   = 2,
  FE_UPWARD     = 3,
};
#endif

typedef unsigned int fexcept_t;
typedef unsigned int fenv_t;

#define FE_DFL_ENV ((const fenv_t *)-1)

typedef unsigned int femode_t;

#define FE_DFL_MODE ((const femode_t *)-1L)
