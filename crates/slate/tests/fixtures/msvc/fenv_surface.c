#include <fenv.h>
#include <stddef.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

#ifdef __STDC_VERSION_FENV_H__
#error "MSVC fenv.h does not expose the C23 version macro"
#endif

_Static_assert(sizeof(fexcept_t) == 4, "fexcept_t size");
_Static_assert(_Alignof(fexcept_t) == 4, "fexcept_t alignment");
_Static_assert((fexcept_t)-1 > 0, "fexcept_t signedness");
_Static_assert(sizeof(fenv_t) == 8, "fenv_t size");
_Static_assert(_Alignof(fenv_t) == 4, "fenv_t alignment");
_Static_assert(offsetof(fenv_t, _Fe_ctl) == 0, "fenv_t control offset");
_Static_assert(offsetof(fenv_t, _Fe_stat) == 4, "fenv_t status offset");

_Static_assert(FE_TONEAREST == 0x0000, "FE_TONEAREST");
_Static_assert(FE_UPWARD == 0x0100, "FE_UPWARD");
_Static_assert(FE_DOWNWARD == 0x0200, "FE_DOWNWARD");
_Static_assert(FE_TOWARDZERO == 0x0300, "FE_TOWARDZERO");
_Static_assert(FE_ROUND_MASK == 0x0300, "FE_ROUND_MASK");
_Static_assert(FE_INEXACT == 0x01, "FE_INEXACT");
_Static_assert(FE_UNDERFLOW == 0x02, "FE_UNDERFLOW");
_Static_assert(FE_OVERFLOW == 0x04, "FE_OVERFLOW");
_Static_assert(FE_DIVBYZERO == 0x08, "FE_DIVBYZERO");
_Static_assert(FE_INVALID == 0x10, "FE_INVALID");
_Static_assert(FE_ALL_EXCEPT == 0x1f, "FE_ALL_EXCEPT");

TYPE_IS(FE_DFL_ENV, const fenv_t *);
TYPE_IS(&fegetround, int (*)(void));
TYPE_IS(&fesetround, int (*)(int));
TYPE_IS(&fegetenv, int (*)(fenv_t *));
TYPE_IS(&fesetenv, int (*)(const fenv_t *));
TYPE_IS(&feclearexcept, int (*)(int));
TYPE_IS(&feholdexcept, int (*)(fenv_t *));
TYPE_IS(&fetestexcept, int (*)(int));
TYPE_IS(&fegetexceptflag, int (*)(fexcept_t *, int));
TYPE_IS(&fesetexceptflag, int (*)(const fexcept_t *, int));
TYPE_IS(&feraiseexcept, int (*)(int));
TYPE_IS(&feupdateenv, int (*)(const fenv_t *));

extern int fesetexcept;
extern int fetestexceptflag;
extern int fegetmode;
extern int fesetmode;

int main(void) { return 0; }
