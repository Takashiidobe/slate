#ifndef _SLATE_FENV_H
#define _SLATE_FENV_H

#include <features.h>

#if defined(__SLATE_LIBC_MSVC)

#include <bits/msvc/fenv.h>

#else

#define __STDC_VERSION_FENV_H__ 202311L

#include <bits/fenv.h>

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

#endif
