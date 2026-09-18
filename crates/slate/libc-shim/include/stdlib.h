#ifndef _SLATE_STDLIB_H
#define _SLATE_STDLIB_H

#include <features.h>

#if defined(__SLATE_LIBC_GLIBC)
#include <sys/types.h>
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __STDC_VERSION_STDLIB_H__ 202311L
#endif

#define __NEED_size_t
#define __NEED_wchar_t
#define __NEED_NULL
#if defined(__SLATE_LIBC_GLIBC) && defined(_GNU_SOURCE)
#define __NEED_locale_t
#endif
#if defined(__SLATE_LIBC_MSVC)
#define __NEED_uintptr_t
#endif
#if defined(__SLATE_LIBC_GLIBC) && __SLATE_GLIBC_MINOR__ >= 43
#define __NEED_once_flag
#endif
#include <bits/types.h>

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/stddef.h>
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define RAND_MAX     2147483647

#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_MUSL)
#define WEXITSTATUS(s) (((s) & 0xff00) >> 8)
#define WTERMSIG(s)    ((s) & 0x7f)
#define WSTOPSIG(s)    WEXITSTATUS(s)
#define WIFEXITED(s)   (!WTERMSIG(s))
#define WIFSTOPPED(s)  ((short)((((s) & 0xffff) * 0x10001U) >> 8) > 0x7f00)
#define WIFSIGNALED(s) ((((s) & 0xffff) - 1U) < 0xffu)
#define WIFCONTINUED(s) ((s) == 0xffff)
#endif

#if defined(__SLATE_LIBC_MUSL)
#define WNOHANG        1
#define WUNTRACED      2
#define WCOREDUMP(s)   ((s) & 0x80)
#endif

typedef struct {
  int quot;
  int rem;
} div_t;

typedef struct {
  long quot;
  long rem;
} ldiv_t;

typedef struct {
  long long quot;
  long long rem;
} lldiv_t;

_Noreturn void exit(int status);
#if defined(__SLATE_LIBC_MUSL) ||                                             \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
_Noreturn void _Exit(int status);
#endif
#if defined(__SLATE_LIBC_MUSL) ||                                             \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
_Noreturn void quick_exit(int status);
#endif
_Noreturn void abort(void);
int            atexit(void (*func)(void));
#if defined(__SLATE_LIBC_MUSL) ||                                             \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
int            at_quick_exit(void (*func)(void));
#endif

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
#if defined(__SLATE_LIBC_FREEBSD) ||                                          \
    (defined(__SLATE_LIBC_MUSL)) ||                                          \
    (defined(__SLATE_LIBC_GLIBC) && defined(__STDC_VERSION__) &&              \
     __STDC_VERSION__ >= 201112L)
void *aligned_alloc(size_t alignment, size_t size);
#endif
#if defined(__SLATE_LIBC_FREEBSD) ||                                          \
    (defined(__SLATE_LIBC_GLIBC) && defined(__STDC_VERSION__) &&              \
     __STDC_VERSION__ >= 202311L)
void  free_sized(void *ptr, size_t size);
void  free_aligned_sized(void *ptr, size_t alignment, size_t size);

static __inline size_t memalignment(const void *p) {
  __UINTPTR_TYPE__ v = (__UINTPTR_TYPE__)p;
  size_t            align = 0;
  while (v != 0 && (v & 1) == 0) {
    v >>= 1;
    align++;
  }
  return v == 0 ? 0 : ((size_t)1 << align);
}
#endif
void  free(void *ptr);

int       abs(int j);
long      labs(long j);
#if defined(__SLATE_LIBC_MUSL) ||                                             \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
long long llabs(long long j);
#endif
div_t     div(int numer, int denom);
ldiv_t    ldiv(long numer, long denom);
#if defined(__SLATE_LIBC_MUSL) ||                                             \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
lldiv_t   lldiv(long long numer, long long denom);
#endif

int         atoi(const char *nptr);
long        atol(const char *nptr);
#if defined(__SLATE_LIBC_MUSL) ||                                             \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
long long   atoll(const char *nptr);
#endif
double      atof(const char *nptr);
double      strtod(const char *__restrict nptr, char **__restrict endptr);
#if defined(__SLATE_LIBC_MUSL) ||                                             \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
float       strtof(const char *__restrict nptr, char **__restrict endptr);
long double strtold(const char *__restrict nptr, char **__restrict endptr);
#endif
#if defined(__SLATE_LIBC_FREEBSD) ||                                          \
    (defined(__SLATE_LIBC_GLIBC) && defined(__STDC_VERSION__) &&              \
     __STDC_VERSION__ >= 202311L)
int         strfromd(char *__restrict s, size_t n, const char *__restrict format,
                     double fp);
int strfromf(char *__restrict s, size_t n, const char *__restrict format, float fp);
int strfroml(char *__restrict s, size_t n, const char *__restrict format,
             long double fp);
#endif
long strtol(const char *__restrict nptr, char **__restrict endptr, int base);
unsigned long strtoul(const char *__restrict nptr, char **__restrict endptr,
                      int base);
#if defined(__SLATE_LIBC_MUSL) ||                                             \
    (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
long long strtoll(const char *__restrict nptr, char **__restrict endptr, int base);
unsigned long long strtoull(const char *__restrict nptr, char **__restrict endptr,
                            int base);
#endif

#if defined(__SLATE_LIBC_GLIBC) && defined(_GNU_SOURCE)
long strtol_l(const char *__restrict, char **__restrict, int, locale_t);
unsigned long strtoul_l(const char *__restrict, char **__restrict, int, locale_t);
long long strtoll_l(const char *__restrict, char **__restrict, int, locale_t);
unsigned long long strtoull_l(const char *__restrict, char **__restrict, int,
                              locale_t);
#endif

int  rand(void);
void srand(unsigned int seed);

char *getenv(const char *name);
int   system(const char *command);

void  qsort(void *base, size_t nmemb, size_t size,
            int (*compar)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));

#if defined(__SLATE_LIBC_GLIBC) && defined(__STDC_VERSION__) &&               \
    __STDC_VERSION__ >= 202311L
#define bsearch(key, base, nmemb, size, compar)                               \
  (bsearch)((key), (base), (nmemb), (size), (compar))
#endif

#if defined(__SLATE_LIBC_GLIBC) && __SLATE_GLIBC_MINOR__ >= 43 &&              \
    defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
void call_once(once_flag *, void (*)(void));
#endif

int    mblen(const char *s, size_t n);
int    mbtowc(wchar_t *__restrict pwc, const char *__restrict s, size_t n);
int    wctomb(char *s, wchar_t wc);
size_t mbstowcs(wchar_t *__restrict dst, const char *__restrict src, size_t len);
size_t wcstombs(char *__restrict dst, const wchar_t *__restrict src, size_t len);

#if !defined(__SLATE_LIBC_MSVC)
size_t __ctype_get_mb_cur_max(void);
#define MB_CUR_MAX (__ctype_get_mb_cur_max())
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE) ||                            \
    (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE + 0 >= 500) ||                   \
    (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE + 0 >= 200112L)
int   posix_memalign(void **memptr, size_t alignment, size_t size);
int   setenv(const char *name, const char *value, int overwrite);
int   unsetenv(const char *name);
int   mkstemp(char *template);
#if !defined(__SLATE_LIBC_GLIBC) || defined(_GNU_SOURCE) ||                  \
    (defined(_XOPEN_SOURCE) && defined(__STRICT_ANSI__) &&                   \
     defined(_GNU_SOURCE))
int   mkostemp(char *template, int flags);
#endif
char *mkdtemp(char *template);
int   getsubopt(char **optionp, char *const *tokens, char **valuep);
int   rand_r(unsigned int *seed);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
char  *realpath(const char *__restrict path, char *__restrict resolved_path);
long   random(void);
void   srandom(unsigned int seed);
char  *initstate(unsigned int seed, char *state, size_t size);
char  *setstate(char *state);
int    putenv(char *string);
#if !defined(__SLATE_LIBC_GLIBC) || defined(_GNU_SOURCE) ||                  \
    (defined(_XOPEN_SOURCE) && defined(__STRICT_ANSI__))
int    posix_openpt(int flags);
int    grantpt(int fd);
int    unlockpt(int fd);
char  *ptsname(int fd);
#endif
char  *l64a(long value);
long   a64l(const char *s);
#if !defined(__SLATE_LIBC_GLIBC) || defined(_GNU_SOURCE) ||                  \
    (defined(_XOPEN_SOURCE) && defined(__STRICT_ANSI__))
void   setkey(const char *key);
#endif
double drand48(void);
double erand48(unsigned short xsubi[3]);
long   lrand48(void);
long   nrand48(unsigned short xsubi[3]);
long   mrand48(void);
long   jrand48(unsigned short xsubi[3]);
void   srand48(long seedval);
unsigned short *seed48(unsigned short seed16v[3]);
void            lcong48(unsigned short param[7]);
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE) ||                           \
    (defined(__SLATE_LIBC_FREEBSD) && __BSD_VISIBLE)
#include <alloca.h>
char *mktemp(char *template);
int   mkstemps(char *template, int suffixlen);
#if !defined(__SLATE_LIBC_GLIBC) || defined(_GNU_SOURCE) ||                  \
    (defined(_XOPEN_SOURCE) && defined(__STRICT_ANSI__))
int   mkostemps(char *template, int suffixlen, int flags);
#endif
void *valloc(size_t size);
#if !defined(__SLATE_LIBC_GLIBC) || defined(_GNU_SOURCE) ||                  \
    (defined(_XOPEN_SOURCE) && defined(__STRICT_ANSI__))
void *memalign(size_t alignment, size_t size);
#endif
int   getloadavg(double loadavg[], int nelem);
int   clearenv(void);
#if defined(__SLATE_LIBC_GLIBC) || defined(__SLATE_LIBC_FREEBSD)
int   on_exit(void (*function)(int, void *), void *arg);
#endif
void *reallocarray(void *ptr, size_t nmemb, size_t size);
#if !defined(__SLATE_LIBC_GLIBC) || defined(_GNU_SOURCE) ||                  \
    (defined(_XOPEN_SOURCE) && defined(__STRICT_ANSI__))
void  qsort_r(void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *, void *), void *arg);
#endif
#if defined(__SLATE_LIBC_FREEBSD)
void __qsort_r_compat(void *base, size_t nmemb, size_t size, void *arg,
                      int (*compar)(void *, const void *, const void *));
__sym_compat(qsort_r, __qsort_r_compat, FBSD_1.0);
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define qsort_r(base, nmemb, size, arg4, arg5)                                \
  _Generic((arg5),                                                            \
      int (*)(void *, const void *, const void *): __qsort_r_compat,          \
      default: qsort_r)(base, nmemb, size, arg4, arg5)
#endif
#endif
#endif

#if defined(__SLATE_LIBC_GLIBC) && defined(_GNU_SOURCE)
int   ptsname_r(int fd, char *buf, size_t buflen);
char *ecvt(double number, int ndigits, int *decpt, int *sign);
char *fcvt(double number, int ndigits, int *decpt, int *sign);
char *gcvt(double number, int ndigit, char *buf);
char *secure_getenv(const char *name);
char *canonicalize_file_name(const char *name);
struct __locale_struct;
float       strtof_l(const char *__restrict nptr, char **__restrict endptr,
                     struct __locale_struct *locale);
double      strtod_l(const char *__restrict nptr, char **__restrict endptr,
                     struct __locale_struct *locale);
long double strtold_l(const char *__restrict nptr, char **__restrict endptr,
                      struct __locale_struct *locale);
#endif

#if defined(_LARGEFILE64_SOURCE)
#define mkstemp64  mkstemp
#define mkostemp64 mkostemp
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define mkstemps64  mkstemps
#define mkostemps64 mkostemps
#endif
#endif

#if defined(__SLATE_LIBC_FREEBSD) && __BSD_VISIBLE
void         arc4random_stir(void);
void         arc4random_addrandom(unsigned char *data, int length);
unsigned int arc4random(void);
void         arc4random_buf(void *buffer, size_t size);
unsigned int arc4random_uniform(unsigned int upper_bound);
#elif defined(__SLATE_LIBC_GLIBC) && defined(_GNU_SOURCE)
unsigned int arc4random(void);
void         arc4random_buf(void *buffer, size_t size);
unsigned int arc4random_uniform(unsigned int upper_bound);
#endif

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/stdlib.h>
#endif

#endif
