#include <fenv.h>

extern int slate_oracle_feclearexcept(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feclearexcept), __typeof__(feclearexcept)),
    "fenv.h:feclearexcept declaration differs from oracle");

static __typeof__(feclearexcept) *const slate_reference_feclearexcept = &feclearexcept;

extern int slate_oracle_fegetenv(fenv_t *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fegetenv), __typeof__(fegetenv)),
    "fenv.h:fegetenv declaration differs from oracle");

static __typeof__(fegetenv) *const slate_reference_fegetenv = &fegetenv;

extern int slate_oracle_fegetexceptflag(unsigned long *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fegetexceptflag), __typeof__(fegetexceptflag)),
    "fenv.h:fegetexceptflag declaration differs from oracle");

static __typeof__(fegetexceptflag) *const slate_reference_fegetexceptflag = &fegetexceptflag;

extern int slate_oracle_fegetround(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fegetround), __typeof__(fegetround)),
    "fenv.h:fegetround declaration differs from oracle");

static __typeof__(fegetround) *const slate_reference_fegetround = &fegetround;

extern int slate_oracle_feholdexcept(fenv_t *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feholdexcept), __typeof__(feholdexcept)),
    "fenv.h:feholdexcept declaration differs from oracle");

static __typeof__(feholdexcept) *const slate_reference_feholdexcept = &feholdexcept;

extern int slate_oracle_feraiseexcept(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feraiseexcept), __typeof__(feraiseexcept)),
    "fenv.h:feraiseexcept declaration differs from oracle");

static __typeof__(feraiseexcept) *const slate_reference_feraiseexcept = &feraiseexcept;

extern int slate_oracle_fesetenv(const fenv_t *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fesetenv), __typeof__(fesetenv)),
    "fenv.h:fesetenv declaration differs from oracle");

static __typeof__(fesetenv) *const slate_reference_fesetenv = &fesetenv;

extern int slate_oracle_fesetexceptflag(const unsigned long *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fesetexceptflag), __typeof__(fesetexceptflag)),
    "fenv.h:fesetexceptflag declaration differs from oracle");

static __typeof__(fesetexceptflag) *const slate_reference_fesetexceptflag = &fesetexceptflag;

extern int slate_oracle_fesetround(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fesetround), __typeof__(fesetround)),
    "fenv.h:fesetround declaration differs from oracle");

static __typeof__(fesetround) *const slate_reference_fesetround = &fesetround;

extern int slate_oracle_fetestexcept(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fetestexcept), __typeof__(fetestexcept)),
    "fenv.h:fetestexcept declaration differs from oracle");

static __typeof__(fetestexcept) *const slate_reference_fetestexcept = &fetestexcept;

extern int slate_oracle_feupdateenv(const fenv_t *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_feupdateenv), __typeof__(feupdateenv)),
    "fenv.h:feupdateenv declaration differs from oracle");

static __typeof__(feupdateenv) *const slate_reference_feupdateenv = &feupdateenv;

extern const struct fenv_t slate_oracle__Fenv0;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Fenv0), __typeof__(_Fenv0)), "_Fenv0 object type differs from oracle");

static __typeof__(_Fenv0) *const slate_reference__Fenv0 = &_Fenv0;

typedef struct fenv_t slate_oracle_typedef_fenv_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fenv_t, fenv_t), "typedef fenv_t differs from oracle");

typedef unsigned long slate_oracle_typedef_fexcept_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fexcept_t, fexcept_t), "typedef fexcept_t differs from oracle");

_Static_assert(sizeof(struct fenv_t) == 8, "struct fenv_t size differs from oracle");

_Static_assert(_Alignof(struct fenv_t) == 4, "struct fenv_t alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct fenv_t, _Fe_ctl) == 0, "struct fenv_t._Fe_ctl offset differs from oracle");

typedef unsigned long slate_oracle_struct_fenv_t__Fe_ctl;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fenv_t *)0)->_Fe_ctl), slate_oracle_struct_fenv_t__Fe_ctl), "struct fenv_t._Fe_ctl field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fenv_t, _Fe_stat) == 4, "struct fenv_t._Fe_stat offset differs from oracle");

typedef unsigned long slate_oracle_struct_fenv_t__Fe_stat;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fenv_t *)0)->_Fe_stat), slate_oracle_struct_fenv_t__Fe_stat), "struct fenv_t._Fe_stat field type differs from oracle");

#ifndef FE_ALL_EXCEPT
#error "fenv.h:FE_ALL_EXCEPT macro is missing from libc-shim"
#endif

#ifndef FE_DFL_ENV
#error "fenv.h:FE_DFL_ENV macro is missing from libc-shim"
#endif

#ifndef FE_DIVBYZERO
#error "fenv.h:FE_DIVBYZERO macro is missing from libc-shim"
#endif

#ifndef FE_DOWNWARD
#error "fenv.h:FE_DOWNWARD macro is missing from libc-shim"
#endif

#ifndef FE_INEXACT
#error "fenv.h:FE_INEXACT macro is missing from libc-shim"
#endif

#ifndef FE_INVALID
#error "fenv.h:FE_INVALID macro is missing from libc-shim"
#endif

#ifndef FE_OVERFLOW
#error "fenv.h:FE_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef FE_ROUND_MASK
#error "fenv.h:FE_ROUND_MASK macro is missing from libc-shim"
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

#ifndef _FENV
#error "fenv.h:_FENV macro is missing from libc-shim"
#endif

int main(void) { return 0; }
