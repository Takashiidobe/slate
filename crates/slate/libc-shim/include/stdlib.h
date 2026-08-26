#ifndef _SLATE_STDLIB_H
#define _SLATE_STDLIB_H

#include <features.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __STDC_VERSION_STDLIB_H__ 202311L
#endif

#define __NEED_size_t
#define __NEED_wchar_t
#define __NEED_NULL
#if defined(__SLATE_LIBC_MSVC)
#define __NEED_uintptr_t
#endif
#include <bits/types.h>

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/stddef.h>
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define RAND_MAX     2147483647

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
_Noreturn void _Exit(int status);
_Noreturn void quick_exit(int status);
_Noreturn void abort(void);
#if !defined(__SLATE_LIBC_MSVC)
_Noreturn void abort_with_reason(const char *restrict reason);
#endif
int            atexit(void (*func)(void));
int            at_quick_exit(void (*func)(void));

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
#if !defined(__SLATE_LIBC_MSVC)
void *aligned_alloc(size_t alignment, size_t size);
void  free_sized(void *ptr, size_t size);
void  free_aligned_sized(void *ptr, size_t alignment, size_t size);

static inline size_t memalignment(const void *p) {
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
long long llabs(long long j);
div_t     div(int numer, int denom);
ldiv_t    ldiv(long numer, long denom);
lldiv_t   lldiv(long long numer, long long denom);

int         atoi(const char *nptr);
long        atol(const char *nptr);
long long   atoll(const char *nptr);
double      atof(const char *nptr);
double      strtod(const char *restrict nptr, char **restrict endptr);
float       strtof(const char *restrict nptr, char **restrict endptr);
long double strtold(const char *restrict nptr, char **restrict endptr);
#if !defined(__SLATE_LIBC_MSVC)
int         strfromd(char *restrict s, size_t n, const char *restrict format,
                     double fp);
int strfromf(char *restrict s, size_t n, const char *restrict format, float fp);
int strfroml(char *restrict s, size_t n, const char *restrict format,
             long double fp);
#endif
long strtol(const char *restrict nptr, char **restrict endptr, int base);
unsigned long strtoul(const char *restrict nptr, char **restrict endptr,
                      int base);
long long strtoll(const char *restrict nptr, char **restrict endptr, int base);
unsigned long long strtoull(const char *restrict nptr, char **restrict endptr,
                            int base);

int  rand(void);
void srand(unsigned int seed);

char *getenv(const char *name);
int   system(const char *command);

void  qsort(void *base, size_t nmemb, size_t size,
            int (*compar)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));

int    mblen(const char *s, size_t n);
int    mbtowc(wchar_t *restrict pwc, const char *restrict s, size_t n);
int    wctomb(char *s, wchar_t wc);
size_t mbstowcs(wchar_t *restrict dst, const char *restrict src, size_t len);
size_t wcstombs(char *restrict dst, const wchar_t *restrict src, size_t len);

#if !defined(__SLATE_LIBC_MSVC)
size_t __ctype_get_mb_cur_max(void);
#define MB_CUR_MAX (__ctype_get_mb_cur_max())
#endif

#if defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) ||                      \
    defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
int   posix_memalign(void **memptr, size_t alignment, size_t size);
int   setenv(const char *name, const char *value, int overwrite);
int   unsetenv(const char *name);
int   mkstemp(char *template);
int   mkostemp(char *template, int flags);
char *mkdtemp(char *template);
int   getsubopt(char **optionp, char *const *tokens, char **valuep);
int   rand_r(unsigned int *seed);
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
char  *realpath(const char *restrict path, char *restrict resolved_path);
long   random(void);
void   srandom(unsigned int seed);
char  *initstate(unsigned int seed, char *state, size_t size);
char  *setstate(char *state);
int    putenv(char *string);
int    posix_openpt(int flags);
int    grantpt(int fd);
int    unlockpt(int fd);
char  *ptsname(int fd);
char  *l64a(long value);
long   a64l(const char *s);
void   setkey(const char *key);
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

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#include <alloca.h>
char *mktemp(char *template);
int   mkstemps(char *template, int suffixlen);
int   mkostemps(char *template, int suffixlen, int flags);
void *valloc(size_t size);
void *memalign(size_t alignment, size_t size);
int   getloadavg(double loadavg[], int nelem);
int   clearenv(void);
void *reallocarray(void *ptr, size_t nmemb, size_t size);
void  qsort_r(void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *, void *), void *arg);
#endif

#ifdef _GNU_SOURCE
int   ptsname_r(int fd, char *buf, size_t buflen);
char *ecvt(double number, int ndigits, int *decpt, int *sign);
char *fcvt(double number, int ndigits, int *decpt, int *sign);
char *gcvt(double number, int ndigit, char *buf);
char *secure_getenv(const char *name);
char *canonicalize_file_name(const char *name);
struct __locale_struct;
float       strtof_l(const char *restrict nptr, char **restrict endptr,
                     struct __locale_struct *locale);
double      strtod_l(const char *restrict nptr, char **restrict endptr,
                     struct __locale_struct *locale);
long double strtold_l(const char *restrict nptr, char **restrict endptr,
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

#if !defined(__SLATE_LIBC_MSVC)
void         arc4random_stir(void);
void         arc4random_addrandom(unsigned char *data, int length);
unsigned int arc4random(void);
void         arc4random_buf(void *buffer, size_t size);
unsigned int arc4random_uniform(unsigned int upper_bound);
#endif

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/stdlib.h>
#endif

#endif
