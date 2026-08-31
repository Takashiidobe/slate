#if !defined(_SLATE_LIBC)
#error "Never include <bits/x86_64/fenv.h> directly; include a public header instead."
#endif

#define FE_INVALID   0x01
#define FE_DIVBYZERO 0x04
#define FE_OVERFLOW  0x08
#define FE_UNDERFLOW 0x10
#define FE_INEXACT   0x20
#define FE_ALL_EXCEPT                                                          \
  (FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

#define FE_TONEAREST  0
#define FE_DOWNWARD   0x400
#define FE_UPWARD     0x800
#define FE_TOWARDZERO 0xc00

typedef unsigned short fexcept_t;

typedef struct {
  unsigned short __control_word;
  unsigned short __glibc_reserved1;
  unsigned short __status_word;
  unsigned short __glibc_reserved2;
  unsigned short __tags;
  unsigned short __glibc_reserved3;
  unsigned int   __eip;
  unsigned short __cs_selector;
  unsigned int   __opcode          : 11;
  unsigned int   __glibc_reserved4 : 5;
  unsigned int   __data_offset;
  unsigned short __data_selector;
  unsigned short __glibc_reserved5;
#ifdef __x86_64__
  unsigned int __mxcsr;
#endif
} fenv_t;

#define FE_DFL_ENV ((const fenv_t *)-1)

typedef struct {
  unsigned short __control_word;
  unsigned short __glibc_reserved;
  unsigned int   __mxcsr;
} femode_t;

#define FE_DFL_MODE ((const femode_t *)-1L)
