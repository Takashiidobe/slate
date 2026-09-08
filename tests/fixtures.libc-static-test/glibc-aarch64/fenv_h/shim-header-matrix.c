#include <fenv.h>

extern int slate_oracle_feclearexcept(int);
extern int slate_oracle_fedisableexcept(int);
extern int slate_oracle_feenableexcept(int);
extern int slate_oracle_fegetenv(fenv_t *);
extern int slate_oracle_fegetexcept(void);
extern int slate_oracle_fegetexceptflag(unsigned int *, int);
extern int slate_oracle_fegetmode(unsigned int *);
extern int slate_oracle_fegetround(void);
extern int slate_oracle_feholdexcept(fenv_t *);
extern int slate_oracle_feraiseexcept(int);
extern int slate_oracle_fesetenv(const fenv_t *);
extern int slate_oracle_fesetexcept(int);
extern int slate_oracle_fesetexceptflag(const unsigned int *, int);
extern int slate_oracle_fesetmode(const unsigned int *);
extern int slate_oracle_fesetround(int);
extern int slate_oracle_fetestexcept(int);
extern int slate_oracle_fetestexceptflag(const unsigned int *, int);
extern int slate_oracle_feupdateenv(const fenv_t *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feclearexcept), __typeof__(feclearexcept)),
    "fenv.h:feclearexcept declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fedisableexcept), __typeof__(fedisableexcept)),
    "fenv.h:fedisableexcept declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feenableexcept), __typeof__(feenableexcept)),
    "fenv.h:feenableexcept declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fegetenv), __typeof__(fegetenv)),
    "fenv.h:fegetenv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fegetexcept), __typeof__(fegetexcept)),
    "fenv.h:fegetexcept declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fegetexceptflag), __typeof__(fegetexceptflag)),
    "fenv.h:fegetexceptflag declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fegetmode), __typeof__(fegetmode)),
    "fenv.h:fegetmode declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fegetround), __typeof__(fegetround)),
    "fenv.h:fegetround declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feholdexcept), __typeof__(feholdexcept)),
    "fenv.h:feholdexcept declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feraiseexcept), __typeof__(feraiseexcept)),
    "fenv.h:feraiseexcept declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fesetenv), __typeof__(fesetenv)),
    "fenv.h:fesetenv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fesetexcept), __typeof__(fesetexcept)),
    "fenv.h:fesetexcept declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fesetexceptflag), __typeof__(fesetexceptflag)),
    "fenv.h:fesetexceptflag declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fesetmode), __typeof__(fesetmode)),
    "fenv.h:fesetmode declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fesetround), __typeof__(fesetround)),
    "fenv.h:fesetround declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fetestexcept), __typeof__(fetestexcept)),
    "fenv.h:fetestexcept declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fetestexceptflag), __typeof__(fetestexceptflag)),
    "fenv.h:fetestexceptflag declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feupdateenv), __typeof__(feupdateenv)),
    "fenv.h:feupdateenv declaration differs from oracle");

static __typeof__(feclearexcept) *const slate_reference_feclearexcept = &feclearexcept;
static __typeof__(fedisableexcept) *const slate_reference_fedisableexcept = &fedisableexcept;
static __typeof__(feenableexcept) *const slate_reference_feenableexcept = &feenableexcept;
static __typeof__(fegetenv) *const slate_reference_fegetenv = &fegetenv;
static __typeof__(fegetexcept) *const slate_reference_fegetexcept = &fegetexcept;
static __typeof__(fegetexceptflag) *const slate_reference_fegetexceptflag = &fegetexceptflag;
static __typeof__(fegetmode) *const slate_reference_fegetmode = &fegetmode;
static __typeof__(fegetround) *const slate_reference_fegetround = &fegetround;
static __typeof__(feholdexcept) *const slate_reference_feholdexcept = &feholdexcept;
static __typeof__(feraiseexcept) *const slate_reference_feraiseexcept = &feraiseexcept;
static __typeof__(fesetenv) *const slate_reference_fesetenv = &fesetenv;
static __typeof__(fesetexcept) *const slate_reference_fesetexcept = &fesetexcept;
static __typeof__(fesetexceptflag) *const slate_reference_fesetexceptflag = &fesetexceptflag;
static __typeof__(fesetmode) *const slate_reference_fesetmode = &fesetmode;
static __typeof__(fesetround) *const slate_reference_fesetround = &fesetround;
static __typeof__(fetestexcept) *const slate_reference_fetestexcept = &fetestexcept;
static __typeof__(fetestexceptflag) *const slate_reference_fetestexceptflag = &fetestexceptflag;
static __typeof__(feupdateenv) *const slate_reference_feupdateenv = &feupdateenv;

typedef unsigned int slate_oracle_typedef_femode_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_femode_t, femode_t), "typedef femode_t differs from oracle");

typedef struct fenv_t slate_oracle_typedef_fenv_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fenv_t, fenv_t), "typedef fenv_t differs from oracle");

typedef unsigned int slate_oracle_typedef_fexcept_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fexcept_t, fexcept_t), "typedef fexcept_t differs from oracle");

#ifndef FE_ALL_EXCEPT
#error "fenv.h:FE_ALL_EXCEPT macro is missing from libc-shim"
#endif

#ifndef FE_DFL_ENV
#error "fenv.h:FE_DFL_ENV macro is missing from libc-shim"
#endif

#ifndef FE_DFL_MODE
#error "fenv.h:FE_DFL_MODE macro is missing from libc-shim"
#endif

#ifndef FE_DIVBYZERO
#error "fenv.h:FE_DIVBYZERO macro is missing from libc-shim"
#endif

#ifndef FE_DOWNWARD
#error "fenv.h:FE_DOWNWARD macro is missing from libc-shim"
#endif

#ifndef FE_EXCEPT_SHIFT
#error "fenv.h:FE_EXCEPT_SHIFT macro is missing from libc-shim"
#endif

#ifndef FE_INEXACT
#error "fenv.h:FE_INEXACT macro is missing from libc-shim"
#endif

#ifndef FE_INVALID
#error "fenv.h:FE_INVALID macro is missing from libc-shim"
#endif

#ifndef FE_NOMASK_ENV
#error "fenv.h:FE_NOMASK_ENV macro is missing from libc-shim"
#endif

#ifndef FE_OVERFLOW
#error "fenv.h:FE_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef FE_TONEAREST
#error "fenv.h:FE_TONEAREST macro is missing from libc-shim"
#endif

#ifndef FE_TOWARDZERO
#error "fenv.h:FE_TOWARDZERO macro is missing from libc-shim"
#endif

#ifndef FE_UNDERFLOW
#error "fenv.h:FE_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef FE_UPWARD
#error "fenv.h:FE_UPWARD macro is missing from libc-shim"
#endif

int main(void) { return 0; }
