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
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
int fesetexcept(int excepts);
#endif
int fesetexceptflag(const fexcept_t *flagp, int excepts);
int fetestexcept(int excepts);
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
int fetestexceptflag(const fexcept_t *flagp, int excepts);
#endif

int fegetround(void);
int fesetround(int rounding_direction);

int fegetenv(fenv_t *envp);
int feholdexcept(fenv_t *envp);
int fesetenv(const fenv_t *envp);
int feupdateenv(const fenv_t *envp);

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
int fegetmode(femode_t *modep);
int fesetmode(const femode_t *modep);
#endif

#ifdef _GNU_SOURCE
int feenableexcept(int excepts);
int fedisableexcept(int excepts);
int fegetexcept(void);
#endif

#endif

#endif
