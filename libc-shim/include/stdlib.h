#ifndef _SLATE_STDLIB_H
#define _SLATE_STDLIB_H

#define __need_size_t
#define __need_wchar_t
#define __need_NULL
#include <stddef.h>
#undef __need_size_t
#undef __need_wchar_t
#undef __need_NULL

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
int            atexit(void (*func)(void));
int            at_quick_exit(void (*func)(void));

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
void *aligned_alloc(size_t alignment, size_t size);
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
double      strtod(const char *restrict nptr, char **restrict endptr);
float       strtof(const char *restrict nptr, char **restrict endptr);
long double strtold(const char *restrict nptr, char **restrict endptr);
int         strfromd(char *restrict s, size_t n, const char *restrict format,
                     double fp);
int strfromf(char *restrict s, size_t n, const char *restrict format, float fp);
int strfroml(char *restrict s, size_t n, const char *restrict format,
             long double fp);
long strtol(const char *restrict nptr, char **restrict endptr, int base);
unsigned long strtoul(const char *restrict nptr, char **restrict endptr,
                      int base);
long long strtoll(const char *restrict nptr, char **restrict endptr, int base);
unsigned long long strtoull(const char *restrict nptr, char **restrict endptr,
                            int base);

int  rand(void);
void srand(unsigned int seed);

char *getenv(const char *name);

void  qsort(void *base, size_t nmemb, size_t size,
            int (*compar)(const void *, const void *));
void *bsearch(const void *key, const void *base, size_t nmemb, size_t size,
              int (*compar)(const void *, const void *));

size_t mbstowcs(wchar_t *restrict dst, const char *restrict src, size_t len);
size_t wcstombs(char *restrict dst, const wchar_t *restrict src, size_t len);

#if defined(__SLATE_LIBC_MSVC)
#include <bits/msvc/locale/stdlib.h>
#include <bits/msvc/wchar/stdlib.h>
#endif

#endif
