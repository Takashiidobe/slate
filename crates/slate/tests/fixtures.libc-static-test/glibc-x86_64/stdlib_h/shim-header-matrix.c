#include <stdlib.h>

extern void slate_oracle__Exit(int) __attribute__((noreturn));
extern unsigned long slate_oracle___ctype_get_mb_cur_max(void);
extern long slate_oracle_a64l(const char *);
extern void slate_oracle_abort(void) __attribute__((noreturn));
extern int slate_oracle_abs(int);
extern void * slate_oracle_aligned_alloc(__size_t, __size_t);
extern void * slate_oracle_alloca(__size_t);
extern unsigned int slate_oracle_arc4random(void);
extern void slate_oracle_arc4random_buf(void *, unsigned long);
extern unsigned int slate_oracle_arc4random_uniform(unsigned int);
extern int slate_oracle_at_quick_exit(void (*)(void));
extern int slate_oracle_atexit(void (*)(void));
extern double slate_oracle_atof(const char *);
extern int slate_oracle_atoi(const char *);
extern long slate_oracle_atol(const char *);
extern long long slate_oracle_atoll(const char *);
extern void * slate_oracle_bsearch(const void *, const void *, unsigned long, unsigned long, int (*)(const void *, const void *));
extern void slate_oracle_call_once(struct __once_flag *, void (*)(void));
extern void * slate_oracle_calloc(__size_t, __size_t);
extern char * slate_oracle_canonicalize_file_name(const char *);
extern int slate_oracle_clearenv(void);
extern struct div_t slate_oracle_div(int, int);
extern double slate_oracle_drand48(void);
extern int slate_oracle_drand48_r(struct drand48_data *restrict, double *restrict);
extern char * slate_oracle_ecvt(double, int, int *restrict, int *restrict);
extern int slate_oracle_ecvt_r(double, int, int *restrict, int *restrict, char *restrict, unsigned long);
extern double slate_oracle_erand48(unsigned short *);
extern int slate_oracle_erand48_r(unsigned short *, struct drand48_data *restrict, double *restrict);
extern void slate_oracle_exit(int) __attribute__((noreturn));
extern char * slate_oracle_fcvt(double, int, int *restrict, int *restrict);
extern int slate_oracle_fcvt_r(double, int, int *restrict, int *restrict, char *restrict, unsigned long);
extern void slate_oracle_free(void *);
extern void slate_oracle_free_aligned_sized(void *, unsigned long, unsigned long);
extern void slate_oracle_free_sized(void *, unsigned long);
extern char * slate_oracle_gcvt(double, int, char *);
extern char * slate_oracle_getenv(const char *);
extern int slate_oracle_getloadavg(double *, int);
extern int slate_oracle_getpt(void);
extern int slate_oracle_getsubopt(char **restrict, char *const *restrict, char **restrict);
extern int slate_oracle_grantpt(int);
extern char * slate_oracle_initstate(unsigned int, char *, unsigned long);
extern int slate_oracle_initstate_r(unsigned int, char *restrict, unsigned long, struct random_data *restrict);
extern long slate_oracle_jrand48(unsigned short *);
extern int slate_oracle_jrand48_r(unsigned short *, struct drand48_data *restrict, long *restrict);
extern char * slate_oracle_l64a(long);
extern long slate_oracle_labs(long);
extern void slate_oracle_lcong48(unsigned short *);
extern int slate_oracle_lcong48_r(unsigned short *, struct drand48_data *);
extern struct ldiv_t slate_oracle_ldiv(long, long);
extern long long slate_oracle_llabs(long long);
extern struct lldiv_t slate_oracle_lldiv(long long, long long);
extern long slate_oracle_lrand48(void);
extern int slate_oracle_lrand48_r(struct drand48_data *restrict, long *restrict);
extern void * slate_oracle_malloc(__size_t);
extern int slate_oracle_mblen(const char *, unsigned long);
extern unsigned long slate_oracle_mbstowcs(int *restrict, const char *restrict, unsigned long);
extern int slate_oracle_mbtowc(int *restrict, const char *restrict, unsigned long);
extern unsigned long slate_oracle_memalignment(const void *);
extern char * slate_oracle_mkdtemp(char *);
extern int slate_oracle_mkostemp(char *, int);
extern int slate_oracle_mkostemp64(char *, int);
extern int slate_oracle_mkostemps(char *, int, int);
extern int slate_oracle_mkostemps64(char *, int, int);
extern int slate_oracle_mkstemp(char *);
extern int slate_oracle_mkstemp64(char *);
extern int slate_oracle_mkstemps(char *, int);
extern int slate_oracle_mkstemps64(char *, int);
extern char * slate_oracle_mktemp(char *);
extern long slate_oracle_mrand48(void);
extern int slate_oracle_mrand48_r(struct drand48_data *restrict, long *restrict);
extern long slate_oracle_nrand48(unsigned short *);
extern int slate_oracle_nrand48_r(unsigned short *, struct drand48_data *restrict, long *restrict);
extern int slate_oracle_on_exit(void (*)(int, void *), void *);
extern int slate_oracle_posix_memalign(void **, unsigned long, unsigned long);
extern int slate_oracle_posix_openpt(int);
extern int slate_oracle_pselect(int, struct fd_set *restrict, struct fd_set *restrict, struct fd_set *restrict, const struct timespec *restrict, const struct __sigset_t *restrict);
extern char * slate_oracle_ptsname(int);
extern int slate_oracle_ptsname_r(int, char *, unsigned long);
extern int slate_oracle_putenv(char *);
extern char * slate_oracle_qecvt(long double, int, int *restrict, int *restrict);
extern int slate_oracle_qecvt_r(long double, int, int *restrict, int *restrict, char *restrict, unsigned long);
extern char * slate_oracle_qfcvt(long double, int, int *restrict, int *restrict);
extern int slate_oracle_qfcvt_r(long double, int, int *restrict, int *restrict, char *restrict, unsigned long);
extern char * slate_oracle_qgcvt(long double, int, char *);
extern void slate_oracle_qsort(void *, unsigned long, unsigned long, int (*)(const void *, const void *));
extern void slate_oracle_qsort_r(void *, unsigned long, unsigned long, int (*)(const void *, const void *, void *), void *);
extern void slate_oracle_quick_exit(int) __attribute__((noreturn));
extern int slate_oracle_rand(void);
extern int slate_oracle_rand_r(unsigned int *);
extern long slate_oracle_random(void);
extern int slate_oracle_random_r(struct random_data *restrict, int *restrict);
extern void * slate_oracle_realloc(void *, __size_t);
extern void * slate_oracle_reallocarray(void *, unsigned long, unsigned long);
extern char * slate_oracle_realpath(const char *restrict, char *restrict);
extern int slate_oracle_rpmatch(const char *);
extern char * slate_oracle_secure_getenv(const char *);
extern unsigned short * slate_oracle_seed48(unsigned short *);
extern int slate_oracle_seed48_r(unsigned short *, struct drand48_data *);
extern int slate_oracle_select(int, struct fd_set *restrict, struct fd_set *restrict, struct fd_set *restrict, struct timeval *restrict);
extern int slate_oracle_setenv(const char *, const char *, int);
extern char * slate_oracle_setstate(char *);
extern int slate_oracle_setstate_r(char *restrict, struct random_data *restrict);
extern void slate_oracle_srand(unsigned int);
extern void slate_oracle_srand48(long);
extern int slate_oracle_srand48_r(long, struct drand48_data *);
extern void slate_oracle_srandom(unsigned int);
extern int slate_oracle_srandom_r(unsigned int, struct random_data *);
extern int slate_oracle_strfromd(char *, unsigned long, const char *, double);
extern int slate_oracle_strfromf(char *, unsigned long, const char *, float);
extern int slate_oracle_strfromf128(char *, unsigned long, const char *, __float128);
extern int slate_oracle_strfromf32(char *, unsigned long, const char *, float);
extern int slate_oracle_strfromf32x(char *, unsigned long, const char *, double);
extern int slate_oracle_strfromf64(char *, unsigned long, const char *, double);
extern int slate_oracle_strfromf64x(char *, unsigned long, const char *, long double);
extern int slate_oracle_strfroml(char *, unsigned long, const char *, long double);
extern double slate_oracle_strtod(const char *, char **);
extern double slate_oracle_strtod_l(const char *restrict, char **restrict, struct __locale_struct *);
extern float slate_oracle_strtof(const char *, char **);
extern __float128 slate_oracle_strtof128(const char *restrict, char **restrict);
extern __float128 slate_oracle_strtof128_l(const char *restrict, char **restrict, struct __locale_struct *);
extern float slate_oracle_strtof32(const char *restrict, char **restrict);
extern float slate_oracle_strtof32_l(const char *restrict, char **restrict, struct __locale_struct *);
extern double slate_oracle_strtof32x(const char *restrict, char **restrict);
extern double slate_oracle_strtof32x_l(const char *restrict, char **restrict, struct __locale_struct *);
extern double slate_oracle_strtof64(const char *restrict, char **restrict);
extern double slate_oracle_strtof64_l(const char *restrict, char **restrict, struct __locale_struct *);
extern long double slate_oracle_strtof64x(const char *restrict, char **restrict);
extern long double slate_oracle_strtof64x_l(const char *restrict, char **restrict, struct __locale_struct *);
extern float slate_oracle_strtof_l(const char *restrict, char **restrict, struct __locale_struct *);
extern long slate_oracle_strtol(const char *, char **, int);
extern long slate_oracle_strtol_l(const char *restrict, char **restrict, int, struct __locale_struct *);
extern long double slate_oracle_strtold(const char *, char **);
extern long double slate_oracle_strtold_l(const char *restrict, char **restrict, struct __locale_struct *);
extern long long slate_oracle_strtoll(const char *, char **, int);
extern long long slate_oracle_strtoll_l(const char *restrict, char **restrict, int, struct __locale_struct *);
extern long long slate_oracle_strtoq(const char *restrict, char **restrict, int);
extern unsigned long slate_oracle_strtoul(const char *, char **, int);
extern unsigned long slate_oracle_strtoul_l(const char *restrict, char **restrict, int, struct __locale_struct *);
extern unsigned long long slate_oracle_strtoull(const char *, char **, int);
extern unsigned long long slate_oracle_strtoull_l(const char *restrict, char **restrict, int, struct __locale_struct *);
extern unsigned long long slate_oracle_strtouq(const char *restrict, char **restrict, int);
extern int slate_oracle_system(const char *);
extern unsigned int slate_oracle_uabs(int);
extern unsigned long slate_oracle_ulabs(long);
extern unsigned long long slate_oracle_ullabs(long long);
extern int slate_oracle_unlockpt(int);
extern int slate_oracle_unsetenv(const char *);
extern void * slate_oracle_valloc(unsigned long);
extern unsigned long slate_oracle_wcstombs(char *restrict, const int *restrict, unsigned long);
extern int slate_oracle_wctomb(char *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__Exit), __typeof__(_Exit)),
    "stdlib.h:_Exit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___ctype_get_mb_cur_max), __typeof__(__ctype_get_mb_cur_max)),
    "stdlib.h:__ctype_get_mb_cur_max declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_a64l), __typeof__(a64l)),
    "stdlib.h:a64l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_abort), __typeof__(abort)),
    "stdlib.h:abort declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_abs), __typeof__(abs)),
    "stdlib.h:abs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_aligned_alloc), __typeof__(aligned_alloc)),
    "stdlib.h:aligned_alloc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_alloca), __typeof__(alloca)),
    "stdlib.h:alloca declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_arc4random), __typeof__(arc4random)),
    "stdlib.h:arc4random declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_arc4random_buf), __typeof__(arc4random_buf)),
    "stdlib.h:arc4random_buf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_arc4random_uniform), __typeof__(arc4random_uniform)),
    "stdlib.h:arc4random_uniform declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_at_quick_exit), __typeof__(at_quick_exit)),
    "stdlib.h:at_quick_exit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atexit), __typeof__(atexit)),
    "stdlib.h:atexit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atof), __typeof__(atof)),
    "stdlib.h:atof declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atoi), __typeof__(atoi)),
    "stdlib.h:atoi declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atol), __typeof__(atol)),
    "stdlib.h:atol declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atoll), __typeof__(atoll)),
    "stdlib.h:atoll declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_bsearch), __typeof__(bsearch)),
    "stdlib.h:bsearch declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_call_once), __typeof__(call_once)),
    "stdlib.h:call_once declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_calloc), __typeof__(calloc)),
    "stdlib.h:calloc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_canonicalize_file_name), __typeof__(canonicalize_file_name)),
    "stdlib.h:canonicalize_file_name declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_clearenv), __typeof__(clearenv)),
    "stdlib.h:clearenv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_div), __typeof__(div)),
    "stdlib.h:div declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_drand48), __typeof__(drand48)),
    "stdlib.h:drand48 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_drand48_r), __typeof__(drand48_r)),
    "stdlib.h:drand48_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ecvt), __typeof__(ecvt)),
    "stdlib.h:ecvt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ecvt_r), __typeof__(ecvt_r)),
    "stdlib.h:ecvt_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erand48), __typeof__(erand48)),
    "stdlib.h:erand48 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erand48_r), __typeof__(erand48_r)),
    "stdlib.h:erand48_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exit), __typeof__(exit)),
    "stdlib.h:exit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fcvt), __typeof__(fcvt)),
    "stdlib.h:fcvt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fcvt_r), __typeof__(fcvt_r)),
    "stdlib.h:fcvt_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_free), __typeof__(free)),
    "stdlib.h:free declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_free_aligned_sized), __typeof__(free_aligned_sized)),
    "stdlib.h:free_aligned_sized declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_free_sized), __typeof__(free_sized)),
    "stdlib.h:free_sized declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gcvt), __typeof__(gcvt)),
    "stdlib.h:gcvt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getenv), __typeof__(getenv)),
    "stdlib.h:getenv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getloadavg), __typeof__(getloadavg)),
    "stdlib.h:getloadavg declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getpt), __typeof__(getpt)),
    "stdlib.h:getpt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getsubopt), __typeof__(getsubopt)),
    "stdlib.h:getsubopt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_grantpt), __typeof__(grantpt)),
    "stdlib.h:grantpt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_initstate), __typeof__(initstate)),
    "stdlib.h:initstate declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_initstate_r), __typeof__(initstate_r)),
    "stdlib.h:initstate_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_jrand48), __typeof__(jrand48)),
    "stdlib.h:jrand48 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_jrand48_r), __typeof__(jrand48_r)),
    "stdlib.h:jrand48_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_l64a), __typeof__(l64a)),
    "stdlib.h:l64a declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_labs), __typeof__(labs)),
    "stdlib.h:labs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lcong48), __typeof__(lcong48)),
    "stdlib.h:lcong48 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lcong48_r), __typeof__(lcong48_r)),
    "stdlib.h:lcong48_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ldiv), __typeof__(ldiv)),
    "stdlib.h:ldiv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llabs), __typeof__(llabs)),
    "stdlib.h:llabs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lldiv), __typeof__(lldiv)),
    "stdlib.h:lldiv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lrand48), __typeof__(lrand48)),
    "stdlib.h:lrand48 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lrand48_r), __typeof__(lrand48_r)),
    "stdlib.h:lrand48_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_malloc), __typeof__(malloc)),
    "stdlib.h:malloc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mblen), __typeof__(mblen)),
    "stdlib.h:mblen declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbstowcs), __typeof__(mbstowcs)),
    "stdlib.h:mbstowcs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbtowc), __typeof__(mbtowc)),
    "stdlib.h:mbtowc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memalignment), __typeof__(memalignment)),
    "stdlib.h:memalignment declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mkdtemp), __typeof__(mkdtemp)),
    "stdlib.h:mkdtemp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mkostemp), __typeof__(mkostemp)),
    "stdlib.h:mkostemp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mkostemp64), __typeof__(mkostemp64)),
    "stdlib.h:mkostemp64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mkostemps), __typeof__(mkostemps)),
    "stdlib.h:mkostemps declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mkostemps64), __typeof__(mkostemps64)),
    "stdlib.h:mkostemps64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mkstemp), __typeof__(mkstemp)),
    "stdlib.h:mkstemp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mkstemp64), __typeof__(mkstemp64)),
    "stdlib.h:mkstemp64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mkstemps), __typeof__(mkstemps)),
    "stdlib.h:mkstemps declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mkstemps64), __typeof__(mkstemps64)),
    "stdlib.h:mkstemps64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mktemp), __typeof__(mktemp)),
    "stdlib.h:mktemp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mrand48), __typeof__(mrand48)),
    "stdlib.h:mrand48 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mrand48_r), __typeof__(mrand48_r)),
    "stdlib.h:mrand48_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nrand48), __typeof__(nrand48)),
    "stdlib.h:nrand48 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nrand48_r), __typeof__(nrand48_r)),
    "stdlib.h:nrand48_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_on_exit), __typeof__(on_exit)),
    "stdlib.h:on_exit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_posix_memalign), __typeof__(posix_memalign)),
    "stdlib.h:posix_memalign declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_posix_openpt), __typeof__(posix_openpt)),
    "stdlib.h:posix_openpt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pselect), __typeof__(pselect)),
    "stdlib.h:pselect declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ptsname), __typeof__(ptsname)),
    "stdlib.h:ptsname declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ptsname_r), __typeof__(ptsname_r)),
    "stdlib.h:ptsname_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_putenv), __typeof__(putenv)),
    "stdlib.h:putenv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_qecvt), __typeof__(qecvt)),
    "stdlib.h:qecvt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_qecvt_r), __typeof__(qecvt_r)),
    "stdlib.h:qecvt_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_qfcvt), __typeof__(qfcvt)),
    "stdlib.h:qfcvt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_qfcvt_r), __typeof__(qfcvt_r)),
    "stdlib.h:qfcvt_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_qgcvt), __typeof__(qgcvt)),
    "stdlib.h:qgcvt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_qsort), __typeof__(qsort)),
    "stdlib.h:qsort declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_qsort_r), __typeof__(qsort_r)),
    "stdlib.h:qsort_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_quick_exit), __typeof__(quick_exit)),
    "stdlib.h:quick_exit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rand), __typeof__(rand)),
    "stdlib.h:rand declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rand_r), __typeof__(rand_r)),
    "stdlib.h:rand_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_random), __typeof__(random)),
    "stdlib.h:random declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_random_r), __typeof__(random_r)),
    "stdlib.h:random_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_realloc), __typeof__(realloc)),
    "stdlib.h:realloc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_reallocarray), __typeof__(reallocarray)),
    "stdlib.h:reallocarray declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_realpath), __typeof__(realpath)),
    "stdlib.h:realpath declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rpmatch), __typeof__(rpmatch)),
    "stdlib.h:rpmatch declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_secure_getenv), __typeof__(secure_getenv)),
    "stdlib.h:secure_getenv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_seed48), __typeof__(seed48)),
    "stdlib.h:seed48 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_seed48_r), __typeof__(seed48_r)),
    "stdlib.h:seed48_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_select), __typeof__(select)),
    "stdlib.h:select declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setenv), __typeof__(setenv)),
    "stdlib.h:setenv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setstate), __typeof__(setstate)),
    "stdlib.h:setstate declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setstate_r), __typeof__(setstate_r)),
    "stdlib.h:setstate_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_srand), __typeof__(srand)),
    "stdlib.h:srand declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_srand48), __typeof__(srand48)),
    "stdlib.h:srand48 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_srand48_r), __typeof__(srand48_r)),
    "stdlib.h:srand48_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_srandom), __typeof__(srandom)),
    "stdlib.h:srandom declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_srandom_r), __typeof__(srandom_r)),
    "stdlib.h:srandom_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfromd), __typeof__(strfromd)),
    "stdlib.h:strfromd declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfromf), __typeof__(strfromf)),
    "stdlib.h:strfromf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfromf128), __typeof__(strfromf128)),
    "stdlib.h:strfromf128 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfromf32), __typeof__(strfromf32)),
    "stdlib.h:strfromf32 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfromf32x), __typeof__(strfromf32x)),
    "stdlib.h:strfromf32x declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfromf64), __typeof__(strfromf64)),
    "stdlib.h:strfromf64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfromf64x), __typeof__(strfromf64x)),
    "stdlib.h:strfromf64x declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfroml), __typeof__(strfroml)),
    "stdlib.h:strfroml declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtod), __typeof__(strtod)),
    "stdlib.h:strtod declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtod_l), __typeof__(strtod_l)),
    "stdlib.h:strtod_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof), __typeof__(strtof)),
    "stdlib.h:strtof declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof128), __typeof__(strtof128)),
    "stdlib.h:strtof128 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof128_l), __typeof__(strtof128_l)),
    "stdlib.h:strtof128_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof32), __typeof__(strtof32)),
    "stdlib.h:strtof32 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof32_l), __typeof__(strtof32_l)),
    "stdlib.h:strtof32_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof32x), __typeof__(strtof32x)),
    "stdlib.h:strtof32x declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof32x_l), __typeof__(strtof32x_l)),
    "stdlib.h:strtof32x_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof64), __typeof__(strtof64)),
    "stdlib.h:strtof64 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof64_l), __typeof__(strtof64_l)),
    "stdlib.h:strtof64_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof64x), __typeof__(strtof64x)),
    "stdlib.h:strtof64x declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof64x_l), __typeof__(strtof64x_l)),
    "stdlib.h:strtof64x_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof_l), __typeof__(strtof_l)),
    "stdlib.h:strtof_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtol), __typeof__(strtol)),
    "stdlib.h:strtol declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtol_l), __typeof__(strtol_l)),
    "stdlib.h:strtol_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtold), __typeof__(strtold)),
    "stdlib.h:strtold declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtold_l), __typeof__(strtold_l)),
    "stdlib.h:strtold_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoll), __typeof__(strtoll)),
    "stdlib.h:strtoll declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoll_l), __typeof__(strtoll_l)),
    "stdlib.h:strtoll_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoq), __typeof__(strtoq)),
    "stdlib.h:strtoq declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoul), __typeof__(strtoul)),
    "stdlib.h:strtoul declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoul_l), __typeof__(strtoul_l)),
    "stdlib.h:strtoul_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoull), __typeof__(strtoull)),
    "stdlib.h:strtoull declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoull_l), __typeof__(strtoull_l)),
    "stdlib.h:strtoull_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtouq), __typeof__(strtouq)),
    "stdlib.h:strtouq declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_system), __typeof__(system)),
    "stdlib.h:system declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_uabs), __typeof__(uabs)),
    "stdlib.h:uabs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ulabs), __typeof__(ulabs)),
    "stdlib.h:ulabs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ullabs), __typeof__(ullabs)),
    "stdlib.h:ullabs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_unlockpt), __typeof__(unlockpt)),
    "stdlib.h:unlockpt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_unsetenv), __typeof__(unsetenv)),
    "stdlib.h:unsetenv declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_valloc), __typeof__(valloc)),
    "stdlib.h:valloc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcstombs), __typeof__(wcstombs)),
    "stdlib.h:wcstombs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wctomb), __typeof__(wctomb)),
    "stdlib.h:wctomb declaration differs from oracle");

static __typeof__(_Exit) *const slate_reference__Exit = &_Exit;
static __typeof__(__ctype_get_mb_cur_max) *const slate_reference___ctype_get_mb_cur_max = &__ctype_get_mb_cur_max;
static __typeof__(a64l) *const slate_reference_a64l = &a64l;
static __typeof__(abort) *const slate_reference_abort = &abort;
static __typeof__(abs) *const slate_reference_abs = &abs;
static __typeof__(aligned_alloc) *const slate_reference_aligned_alloc = &aligned_alloc;
static __typeof__(alloca) *const slate_reference_alloca = &alloca;
static __typeof__(arc4random) *const slate_reference_arc4random = &arc4random;
static __typeof__(arc4random_buf) *const slate_reference_arc4random_buf = &arc4random_buf;
static __typeof__(arc4random_uniform) *const slate_reference_arc4random_uniform = &arc4random_uniform;
static __typeof__(at_quick_exit) *const slate_reference_at_quick_exit = &at_quick_exit;
static __typeof__(atexit) *const slate_reference_atexit = &atexit;
static __typeof__(atof) *const slate_reference_atof = &atof;
static __typeof__(atoi) *const slate_reference_atoi = &atoi;
static __typeof__(atol) *const slate_reference_atol = &atol;
static __typeof__(atoll) *const slate_reference_atoll = &atoll;
static __typeof__(bsearch) *const slate_reference_bsearch = &bsearch;
static __typeof__(call_once) *const slate_reference_call_once = &call_once;
static __typeof__(calloc) *const slate_reference_calloc = &calloc;
static __typeof__(canonicalize_file_name) *const slate_reference_canonicalize_file_name = &canonicalize_file_name;
static __typeof__(clearenv) *const slate_reference_clearenv = &clearenv;
static __typeof__(div) *const slate_reference_div = &div;
static __typeof__(drand48) *const slate_reference_drand48 = &drand48;
static __typeof__(drand48_r) *const slate_reference_drand48_r = &drand48_r;
static __typeof__(ecvt) *const slate_reference_ecvt = &ecvt;
static __typeof__(ecvt_r) *const slate_reference_ecvt_r = &ecvt_r;
static __typeof__(erand48) *const slate_reference_erand48 = &erand48;
static __typeof__(erand48_r) *const slate_reference_erand48_r = &erand48_r;
static __typeof__(exit) *const slate_reference_exit = &exit;
static __typeof__(fcvt) *const slate_reference_fcvt = &fcvt;
static __typeof__(fcvt_r) *const slate_reference_fcvt_r = &fcvt_r;
static __typeof__(free) *const slate_reference_free = &free;
static __typeof__(free_aligned_sized) *const slate_reference_free_aligned_sized = &free_aligned_sized;
static __typeof__(free_sized) *const slate_reference_free_sized = &free_sized;
static __typeof__(gcvt) *const slate_reference_gcvt = &gcvt;
static __typeof__(getenv) *const slate_reference_getenv = &getenv;
static __typeof__(getloadavg) *const slate_reference_getloadavg = &getloadavg;
static __typeof__(getpt) *const slate_reference_getpt = &getpt;
static __typeof__(getsubopt) *const slate_reference_getsubopt = &getsubopt;
static __typeof__(grantpt) *const slate_reference_grantpt = &grantpt;
static __typeof__(initstate) *const slate_reference_initstate = &initstate;
static __typeof__(initstate_r) *const slate_reference_initstate_r = &initstate_r;
static __typeof__(jrand48) *const slate_reference_jrand48 = &jrand48;
static __typeof__(jrand48_r) *const slate_reference_jrand48_r = &jrand48_r;
static __typeof__(l64a) *const slate_reference_l64a = &l64a;
static __typeof__(labs) *const slate_reference_labs = &labs;
static __typeof__(lcong48) *const slate_reference_lcong48 = &lcong48;
static __typeof__(lcong48_r) *const slate_reference_lcong48_r = &lcong48_r;
static __typeof__(ldiv) *const slate_reference_ldiv = &ldiv;
static __typeof__(llabs) *const slate_reference_llabs = &llabs;
static __typeof__(lldiv) *const slate_reference_lldiv = &lldiv;
static __typeof__(lrand48) *const slate_reference_lrand48 = &lrand48;
static __typeof__(lrand48_r) *const slate_reference_lrand48_r = &lrand48_r;
static __typeof__(malloc) *const slate_reference_malloc = &malloc;
static __typeof__(mblen) *const slate_reference_mblen = &mblen;
static __typeof__(mbstowcs) *const slate_reference_mbstowcs = &mbstowcs;
static __typeof__(mbtowc) *const slate_reference_mbtowc = &mbtowc;
static __typeof__(memalignment) *const slate_reference_memalignment = &memalignment;
static __typeof__(mkdtemp) *const slate_reference_mkdtemp = &mkdtemp;
static __typeof__(mkostemp) *const slate_reference_mkostemp = &mkostemp;
static __typeof__(mkostemp64) *const slate_reference_mkostemp64 = &mkostemp64;
static __typeof__(mkostemps) *const slate_reference_mkostemps = &mkostemps;
static __typeof__(mkostemps64) *const slate_reference_mkostemps64 = &mkostemps64;
static __typeof__(mkstemp) *const slate_reference_mkstemp = &mkstemp;
static __typeof__(mkstemp64) *const slate_reference_mkstemp64 = &mkstemp64;
static __typeof__(mkstemps) *const slate_reference_mkstemps = &mkstemps;
static __typeof__(mkstemps64) *const slate_reference_mkstemps64 = &mkstemps64;
static __typeof__(mktemp) *const slate_reference_mktemp = &mktemp;
static __typeof__(mrand48) *const slate_reference_mrand48 = &mrand48;
static __typeof__(mrand48_r) *const slate_reference_mrand48_r = &mrand48_r;
static __typeof__(nrand48) *const slate_reference_nrand48 = &nrand48;
static __typeof__(nrand48_r) *const slate_reference_nrand48_r = &nrand48_r;
static __typeof__(on_exit) *const slate_reference_on_exit = &on_exit;
static __typeof__(posix_memalign) *const slate_reference_posix_memalign = &posix_memalign;
static __typeof__(posix_openpt) *const slate_reference_posix_openpt = &posix_openpt;
static __typeof__(pselect) *const slate_reference_pselect = &pselect;
static __typeof__(ptsname) *const slate_reference_ptsname = &ptsname;
static __typeof__(ptsname_r) *const slate_reference_ptsname_r = &ptsname_r;
static __typeof__(putenv) *const slate_reference_putenv = &putenv;
static __typeof__(qecvt) *const slate_reference_qecvt = &qecvt;
static __typeof__(qecvt_r) *const slate_reference_qecvt_r = &qecvt_r;
static __typeof__(qfcvt) *const slate_reference_qfcvt = &qfcvt;
static __typeof__(qfcvt_r) *const slate_reference_qfcvt_r = &qfcvt_r;
static __typeof__(qgcvt) *const slate_reference_qgcvt = &qgcvt;
static __typeof__(qsort) *const slate_reference_qsort = &qsort;
static __typeof__(qsort_r) *const slate_reference_qsort_r = &qsort_r;
static __typeof__(quick_exit) *const slate_reference_quick_exit = &quick_exit;
static __typeof__(rand) *const slate_reference_rand = &rand;
static __typeof__(rand_r) *const slate_reference_rand_r = &rand_r;
static __typeof__(random) *const slate_reference_random = &random;
static __typeof__(random_r) *const slate_reference_random_r = &random_r;
static __typeof__(realloc) *const slate_reference_realloc = &realloc;
static __typeof__(reallocarray) *const slate_reference_reallocarray = &reallocarray;
static __typeof__(realpath) *const slate_reference_realpath = &realpath;
static __typeof__(rpmatch) *const slate_reference_rpmatch = &rpmatch;
static __typeof__(secure_getenv) *const slate_reference_secure_getenv = &secure_getenv;
static __typeof__(seed48) *const slate_reference_seed48 = &seed48;
static __typeof__(seed48_r) *const slate_reference_seed48_r = &seed48_r;
static __typeof__(select) *const slate_reference_select = &select;
static __typeof__(setenv) *const slate_reference_setenv = &setenv;
static __typeof__(setstate) *const slate_reference_setstate = &setstate;
static __typeof__(setstate_r) *const slate_reference_setstate_r = &setstate_r;
static __typeof__(srand) *const slate_reference_srand = &srand;
static __typeof__(srand48) *const slate_reference_srand48 = &srand48;
static __typeof__(srand48_r) *const slate_reference_srand48_r = &srand48_r;
static __typeof__(srandom) *const slate_reference_srandom = &srandom;
static __typeof__(srandom_r) *const slate_reference_srandom_r = &srandom_r;
static __typeof__(strfromd) *const slate_reference_strfromd = &strfromd;
static __typeof__(strfromf) *const slate_reference_strfromf = &strfromf;
static __typeof__(strfromf128) *const slate_reference_strfromf128 = &strfromf128;
static __typeof__(strfromf32) *const slate_reference_strfromf32 = &strfromf32;
static __typeof__(strfromf32x) *const slate_reference_strfromf32x = &strfromf32x;
static __typeof__(strfromf64) *const slate_reference_strfromf64 = &strfromf64;
static __typeof__(strfromf64x) *const slate_reference_strfromf64x = &strfromf64x;
static __typeof__(strfroml) *const slate_reference_strfroml = &strfroml;
static __typeof__(strtod) *const slate_reference_strtod = &strtod;
static __typeof__(strtod_l) *const slate_reference_strtod_l = &strtod_l;
static __typeof__(strtof) *const slate_reference_strtof = &strtof;
static __typeof__(strtof128) *const slate_reference_strtof128 = &strtof128;
static __typeof__(strtof128_l) *const slate_reference_strtof128_l = &strtof128_l;
static __typeof__(strtof32) *const slate_reference_strtof32 = &strtof32;
static __typeof__(strtof32_l) *const slate_reference_strtof32_l = &strtof32_l;
static __typeof__(strtof32x) *const slate_reference_strtof32x = &strtof32x;
static __typeof__(strtof32x_l) *const slate_reference_strtof32x_l = &strtof32x_l;
static __typeof__(strtof64) *const slate_reference_strtof64 = &strtof64;
static __typeof__(strtof64_l) *const slate_reference_strtof64_l = &strtof64_l;
static __typeof__(strtof64x) *const slate_reference_strtof64x = &strtof64x;
static __typeof__(strtof64x_l) *const slate_reference_strtof64x_l = &strtof64x_l;
static __typeof__(strtof_l) *const slate_reference_strtof_l = &strtof_l;
static __typeof__(strtol) *const slate_reference_strtol = &strtol;
static __typeof__(strtol_l) *const slate_reference_strtol_l = &strtol_l;
static __typeof__(strtold) *const slate_reference_strtold = &strtold;
static __typeof__(strtold_l) *const slate_reference_strtold_l = &strtold_l;
static __typeof__(strtoll) *const slate_reference_strtoll = &strtoll;
static __typeof__(strtoll_l) *const slate_reference_strtoll_l = &strtoll_l;
static __typeof__(strtoq) *const slate_reference_strtoq = &strtoq;
static __typeof__(strtoul) *const slate_reference_strtoul = &strtoul;
static __typeof__(strtoul_l) *const slate_reference_strtoul_l = &strtoul_l;
static __typeof__(strtoull) *const slate_reference_strtoull = &strtoull;
static __typeof__(strtoull_l) *const slate_reference_strtoull_l = &strtoull_l;
static __typeof__(strtouq) *const slate_reference_strtouq = &strtouq;
static __typeof__(system) *const slate_reference_system = &system;
static __typeof__(uabs) *const slate_reference_uabs = &uabs;
static __typeof__(ulabs) *const slate_reference_ulabs = &ulabs;
static __typeof__(ullabs) *const slate_reference_ullabs = &ullabs;
static __typeof__(unlockpt) *const slate_reference_unlockpt = &unlockpt;
static __typeof__(unsetenv) *const slate_reference_unsetenv = &unsetenv;
static __typeof__(valloc) *const slate_reference_valloc = &valloc;
static __typeof__(wcstombs) *const slate_reference_wcstombs = &wcstombs;
static __typeof__(wctomb) *const slate_reference_wctomb = &wctomb;

typedef __float128 slate_oracle_typedef__Float128;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float128, _Float128), "typedef _Float128 differs from oracle");

typedef float slate_oracle_typedef__Float32;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float32, _Float32), "typedef _Float32 differs from oracle");

typedef double slate_oracle_typedef__Float32x;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float32x, _Float32x), "typedef _Float32x differs from oracle");

typedef double slate_oracle_typedef__Float64;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float64, _Float64), "typedef _Float64 differs from oracle");

typedef long double slate_oracle_typedef__Float64x;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef__Float64x, _Float64x), "typedef _Float64x differs from oracle");

typedef long slate_oracle_typedef_blkcnt64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_blkcnt64_t, blkcnt64_t), "typedef blkcnt64_t differs from oracle");

typedef long slate_oracle_typedef_blkcnt_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_blkcnt_t, blkcnt_t), "typedef blkcnt_t differs from oracle");

typedef long slate_oracle_typedef_blksize_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_blksize_t, blksize_t), "typedef blksize_t differs from oracle");

typedef char * slate_oracle_typedef_caddr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_caddr_t, caddr_t), "typedef caddr_t differs from oracle");

typedef long slate_oracle_typedef_clock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clock_t, clock_t), "typedef clock_t differs from oracle");

typedef int slate_oracle_typedef_clockid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_clockid_t, clockid_t), "typedef clockid_t differs from oracle");

typedef int slate_oracle_typedef_daddr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_daddr_t, daddr_t), "typedef daddr_t differs from oracle");

typedef unsigned long slate_oracle_typedef_dev_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_dev_t, dev_t), "typedef dev_t differs from oracle");

typedef struct struct div_t slate_oracle_typedef_div_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_div_t, div_t), "typedef div_t differs from oracle");

typedef long slate_oracle_typedef_fd_mask;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fd_mask, fd_mask), "typedef fd_mask differs from oracle");

typedef struct struct fd_set slate_oracle_typedef_fd_set;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fd_set, fd_set), "typedef fd_set differs from oracle");

typedef unsigned long slate_oracle_typedef_fsblkcnt64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsblkcnt64_t, fsblkcnt64_t), "typedef fsblkcnt64_t differs from oracle");

typedef unsigned long slate_oracle_typedef_fsblkcnt_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsblkcnt_t, fsblkcnt_t), "typedef fsblkcnt_t differs from oracle");

typedef unsigned long slate_oracle_typedef_fsfilcnt64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsfilcnt64_t, fsfilcnt64_t), "typedef fsfilcnt64_t differs from oracle");

typedef unsigned long slate_oracle_typedef_fsfilcnt_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsfilcnt_t, fsfilcnt_t), "typedef fsfilcnt_t differs from oracle");

typedef struct struct __fsid_t slate_oracle_typedef_fsid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_fsid_t, fsid_t), "typedef fsid_t differs from oracle");

typedef unsigned int slate_oracle_typedef_gid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_gid_t, gid_t), "typedef gid_t differs from oracle");

typedef unsigned int slate_oracle_typedef_id_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_id_t, id_t), "typedef id_t differs from oracle");

typedef unsigned long slate_oracle_typedef_ino64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ino64_t, ino64_t), "typedef ino64_t differs from oracle");

typedef unsigned long slate_oracle_typedef_ino_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ino_t, ino_t), "typedef ino_t differs from oracle");

typedef short slate_oracle_typedef_int16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int16_t, int16_t), "typedef int16_t differs from oracle");

typedef int slate_oracle_typedef_int32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int32_t, int32_t), "typedef int32_t differs from oracle");

typedef long slate_oracle_typedef_int64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int64_t, int64_t), "typedef int64_t differs from oracle");

typedef signed char slate_oracle_typedef_int8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_int8_t, int8_t), "typedef int8_t differs from oracle");

typedef int slate_oracle_typedef_key_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_key_t, key_t), "typedef key_t differs from oracle");

typedef struct struct ldiv_t slate_oracle_typedef_ldiv_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ldiv_t, ldiv_t), "typedef ldiv_t differs from oracle");

typedef struct struct lldiv_t slate_oracle_typedef_lldiv_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_lldiv_t, lldiv_t), "typedef lldiv_t differs from oracle");

typedef struct __locale_struct * slate_oracle_typedef_locale_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_locale_t, locale_t), "typedef locale_t differs from oracle");

typedef long slate_oracle_typedef_loff_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_loff_t, loff_t), "typedef loff_t differs from oracle");

typedef unsigned int slate_oracle_typedef_mode_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_mode_t, mode_t), "typedef mode_t differs from oracle");

typedef unsigned long slate_oracle_typedef_nlink_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_nlink_t, nlink_t), "typedef nlink_t differs from oracle");

typedef long slate_oracle_typedef_off64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off64_t, off64_t), "typedef off64_t differs from oracle");

typedef long slate_oracle_typedef_off_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off_t, off_t), "typedef off_t differs from oracle");

typedef struct struct __once_flag slate_oracle_typedef_once_flag;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_once_flag, once_flag), "typedef once_flag differs from oracle");

typedef int slate_oracle_typedef_pid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pid_t, pid_t), "typedef pid_t differs from oracle");

typedef union union pthread_attr_t slate_oracle_typedef_pthread_attr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_attr_t, pthread_attr_t), "typedef pthread_attr_t differs from oracle");

typedef union union pthread_barrier_t slate_oracle_typedef_pthread_barrier_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_barrier_t, pthread_barrier_t), "typedef pthread_barrier_t differs from oracle");

typedef union union pthread_barrierattr_t slate_oracle_typedef_pthread_barrierattr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_barrierattr_t, pthread_barrierattr_t), "typedef pthread_barrierattr_t differs from oracle");

typedef union union pthread_cond_t slate_oracle_typedef_pthread_cond_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_cond_t, pthread_cond_t), "typedef pthread_cond_t differs from oracle");

typedef union union pthread_condattr_t slate_oracle_typedef_pthread_condattr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_condattr_t, pthread_condattr_t), "typedef pthread_condattr_t differs from oracle");

typedef unsigned int slate_oracle_typedef_pthread_key_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_key_t, pthread_key_t), "typedef pthread_key_t differs from oracle");

typedef union union pthread_mutex_t slate_oracle_typedef_pthread_mutex_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_mutex_t, pthread_mutex_t), "typedef pthread_mutex_t differs from oracle");

typedef union union pthread_mutexattr_t slate_oracle_typedef_pthread_mutexattr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_mutexattr_t, pthread_mutexattr_t), "typedef pthread_mutexattr_t differs from oracle");

typedef int slate_oracle_typedef_pthread_once_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_once_t, pthread_once_t), "typedef pthread_once_t differs from oracle");

typedef union union pthread_rwlock_t slate_oracle_typedef_pthread_rwlock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_rwlock_t, pthread_rwlock_t), "typedef pthread_rwlock_t differs from oracle");

typedef union union pthread_rwlockattr_t slate_oracle_typedef_pthread_rwlockattr_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_rwlockattr_t, pthread_rwlockattr_t), "typedef pthread_rwlockattr_t differs from oracle");

typedef volatile int slate_oracle_typedef_pthread_spinlock_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_spinlock_t, pthread_spinlock_t), "typedef pthread_spinlock_t differs from oracle");

typedef unsigned long slate_oracle_typedef_pthread_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_pthread_t, pthread_t), "typedef pthread_t differs from oracle");

typedef long slate_oracle_typedef_quad_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_quad_t, quad_t), "typedef quad_t differs from oracle");

typedef long slate_oracle_typedef_register_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_register_t, register_t), "typedef register_t differs from oracle");

typedef struct struct __sigset_t slate_oracle_typedef_sigset_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_sigset_t, sigset_t), "typedef sigset_t differs from oracle");

typedef unsigned long slate_oracle_typedef_size_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_size_t, size_t), "typedef size_t differs from oracle");

typedef long slate_oracle_typedef_ssize_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ssize_t, ssize_t), "typedef ssize_t differs from oracle");

typedef long slate_oracle_typedef_suseconds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_suseconds_t, suseconds_t), "typedef suseconds_t differs from oracle");

typedef long slate_oracle_typedef_time_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_time_t, time_t), "typedef time_t differs from oracle");

typedef void * slate_oracle_typedef_timer_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_timer_t, timer_t), "typedef timer_t differs from oracle");

typedef unsigned char slate_oracle_typedef_u_char;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_char, u_char), "typedef u_char differs from oracle");

typedef unsigned int slate_oracle_typedef_u_int;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int, u_int), "typedef u_int differs from oracle");

typedef unsigned short slate_oracle_typedef_u_int16_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int16_t, u_int16_t), "typedef u_int16_t differs from oracle");

typedef unsigned int slate_oracle_typedef_u_int32_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int32_t, u_int32_t), "typedef u_int32_t differs from oracle");

typedef unsigned long slate_oracle_typedef_u_int64_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int64_t, u_int64_t), "typedef u_int64_t differs from oracle");

typedef unsigned char slate_oracle_typedef_u_int8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int8_t, u_int8_t), "typedef u_int8_t differs from oracle");

typedef unsigned long slate_oracle_typedef_u_long;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_long, u_long), "typedef u_long differs from oracle");

typedef unsigned long slate_oracle_typedef_u_quad_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_quad_t, u_quad_t), "typedef u_quad_t differs from oracle");

typedef unsigned short slate_oracle_typedef_u_short;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_short, u_short), "typedef u_short differs from oracle");

typedef unsigned int slate_oracle_typedef_uid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uid_t, uid_t), "typedef uid_t differs from oracle");

typedef unsigned int slate_oracle_typedef_uint;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_uint, uint), "typedef uint differs from oracle");

typedef unsigned long slate_oracle_typedef_ulong;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ulong, ulong), "typedef ulong differs from oracle");

typedef unsigned int slate_oracle_typedef_useconds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_useconds_t, useconds_t), "typedef useconds_t differs from oracle");

typedef unsigned short slate_oracle_typedef_ushort;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ushort, ushort), "typedef ushort differs from oracle");

typedef int slate_oracle_typedef_wchar_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_wchar_t, wchar_t), "typedef wchar_t differs from oracle");

typedef const unsigned short * slate_oracle_struct___locale_struct___ctype_b;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __locale_struct *)0)->__ctype_b), slate_oracle_struct___locale_struct___ctype_b), "struct __locale_struct.__ctype_b field type differs from oracle");

typedef const int * slate_oracle_struct___locale_struct___ctype_tolower;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __locale_struct *)0)->__ctype_tolower), slate_oracle_struct___locale_struct___ctype_tolower), "struct __locale_struct.__ctype_tolower field type differs from oracle");

typedef const int * slate_oracle_struct___locale_struct___ctype_toupper;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __locale_struct *)0)->__ctype_toupper), slate_oracle_struct___locale_struct___ctype_toupper), "struct __locale_struct.__ctype_toupper field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_cond_s, __wseq) == 0, "struct __pthread_cond_s.__wseq offset differs from oracle");

typedef union union __atomic_wide_counter slate_oracle_struct___pthread_cond_s___wseq;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__wseq), slate_oracle_struct___pthread_cond_s___wseq), "struct __pthread_cond_s.__wseq field type differs from oracle");

typedef union union __atomic_wide_counter slate_oracle_struct___pthread_cond_s___g1_start;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__g1_start), slate_oracle_struct___pthread_cond_s___g1_start), "struct __pthread_cond_s.__g1_start field type differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_cond_s___g1_orig_size;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__g1_orig_size), slate_oracle_struct___pthread_cond_s___g1_orig_size), "struct __pthread_cond_s.__g1_orig_size field type differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_cond_s___wrefs;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__wrefs), slate_oracle_struct___pthread_cond_s___wrefs), "struct __pthread_cond_s.__wrefs field type differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_cond_s___unused_initialized_1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__unused_initialized_1), slate_oracle_struct___pthread_cond_s___unused_initialized_1), "struct __pthread_cond_s.__unused_initialized_1 field type differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_cond_s___unused_initialized_2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_cond_s *)0)->__unused_initialized_2), slate_oracle_struct___pthread_cond_s___unused_initialized_2), "struct __pthread_cond_s.__unused_initialized_2 field type differs from oracle");

typedef struct __pthread_internal_list * slate_oracle_struct___pthread_internal_list___prev;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_internal_list *)0)->__prev), slate_oracle_struct___pthread_internal_list___prev), "struct __pthread_internal_list.__prev field type differs from oracle");

typedef struct __pthread_internal_list * slate_oracle_struct___pthread_internal_list___next;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_internal_list *)0)->__next), slate_oracle_struct___pthread_internal_list___next), "struct __pthread_internal_list.__next field type differs from oracle");

typedef struct __pthread_internal_slist * slate_oracle_struct___pthread_internal_slist___next;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_internal_slist *)0)->__next), slate_oracle_struct___pthread_internal_slist___next), "struct __pthread_internal_slist.__next field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __lock) == 0, "struct __pthread_mutex_s.__lock offset differs from oracle");

typedef int slate_oracle_struct___pthread_mutex_s___lock;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__lock), slate_oracle_struct___pthread_mutex_s___lock), "struct __pthread_mutex_s.__lock field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __count) == 4, "struct __pthread_mutex_s.__count offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_mutex_s___count;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__count), slate_oracle_struct___pthread_mutex_s___count), "struct __pthread_mutex_s.__count field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __owner) == 8, "struct __pthread_mutex_s.__owner offset differs from oracle");

typedef int slate_oracle_struct___pthread_mutex_s___owner;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__owner), slate_oracle_struct___pthread_mutex_s___owner), "struct __pthread_mutex_s.__owner field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __nusers) == 12, "struct __pthread_mutex_s.__nusers offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_mutex_s___nusers;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__nusers), slate_oracle_struct___pthread_mutex_s___nusers), "struct __pthread_mutex_s.__nusers field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __kind) == 16, "struct __pthread_mutex_s.__kind offset differs from oracle");

typedef int slate_oracle_struct___pthread_mutex_s___kind;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__kind), slate_oracle_struct___pthread_mutex_s___kind), "struct __pthread_mutex_s.__kind field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __spins) == 20, "struct __pthread_mutex_s.__spins offset differs from oracle");

typedef short slate_oracle_struct___pthread_mutex_s___spins;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__spins), slate_oracle_struct___pthread_mutex_s___spins), "struct __pthread_mutex_s.__spins field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_mutex_s, __glibc_reserved) == 22, "struct __pthread_mutex_s.__glibc_reserved offset differs from oracle");

typedef short slate_oracle_struct___pthread_mutex_s___glibc_reserved;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__glibc_reserved), slate_oracle_struct___pthread_mutex_s___glibc_reserved), "struct __pthread_mutex_s.__glibc_reserved field type differs from oracle");

typedef struct __pthread_internal_list slate_oracle_struct___pthread_mutex_s___list;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_mutex_s *)0)->__list), slate_oracle_struct___pthread_mutex_s___list), "struct __pthread_mutex_s.__list field type differs from oracle");

_Static_assert(sizeof(struct __pthread_rwlock_arch_t) == 56, "struct __pthread_rwlock_arch_t size differs from oracle");

_Static_assert(_Alignof(struct __pthread_rwlock_arch_t) == 8, "struct __pthread_rwlock_arch_t alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __readers) == 0, "struct __pthread_rwlock_arch_t.__readers offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___readers;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__readers), slate_oracle_struct___pthread_rwlock_arch_t___readers), "struct __pthread_rwlock_arch_t.__readers field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __writers) == 4, "struct __pthread_rwlock_arch_t.__writers offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___writers;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__writers), slate_oracle_struct___pthread_rwlock_arch_t___writers), "struct __pthread_rwlock_arch_t.__writers field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __wrphase_futex) == 8, "struct __pthread_rwlock_arch_t.__wrphase_futex offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___wrphase_futex;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__wrphase_futex), slate_oracle_struct___pthread_rwlock_arch_t___wrphase_futex), "struct __pthread_rwlock_arch_t.__wrphase_futex field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __writers_futex) == 12, "struct __pthread_rwlock_arch_t.__writers_futex offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___writers_futex;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__writers_futex), slate_oracle_struct___pthread_rwlock_arch_t___writers_futex), "struct __pthread_rwlock_arch_t.__writers_futex field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __pad3) == 16, "struct __pthread_rwlock_arch_t.__pad3 offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___pad3;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__pad3), slate_oracle_struct___pthread_rwlock_arch_t___pad3), "struct __pthread_rwlock_arch_t.__pad3 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __pad4) == 20, "struct __pthread_rwlock_arch_t.__pad4 offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___pad4;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__pad4), slate_oracle_struct___pthread_rwlock_arch_t___pad4), "struct __pthread_rwlock_arch_t.__pad4 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __cur_writer) == 24, "struct __pthread_rwlock_arch_t.__cur_writer offset differs from oracle");

typedef int slate_oracle_struct___pthread_rwlock_arch_t___cur_writer;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__cur_writer), slate_oracle_struct___pthread_rwlock_arch_t___cur_writer), "struct __pthread_rwlock_arch_t.__cur_writer field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __shared) == 28, "struct __pthread_rwlock_arch_t.__shared offset differs from oracle");

typedef int slate_oracle_struct___pthread_rwlock_arch_t___shared;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__shared), slate_oracle_struct___pthread_rwlock_arch_t___shared), "struct __pthread_rwlock_arch_t.__shared field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __pad1) == 32, "struct __pthread_rwlock_arch_t.__pad1 offset differs from oracle");

typedef unsigned long slate_oracle_struct___pthread_rwlock_arch_t___pad1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__pad1), slate_oracle_struct___pthread_rwlock_arch_t___pad1), "struct __pthread_rwlock_arch_t.__pad1 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __pad2) == 40, "struct __pthread_rwlock_arch_t.__pad2 offset differs from oracle");

typedef unsigned long slate_oracle_struct___pthread_rwlock_arch_t___pad2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__pad2), slate_oracle_struct___pthread_rwlock_arch_t___pad2), "struct __pthread_rwlock_arch_t.__pad2 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct __pthread_rwlock_arch_t, __flags) == 48, "struct __pthread_rwlock_arch_t.__flags offset differs from oracle");

typedef unsigned int slate_oracle_struct___pthread_rwlock_arch_t___flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __pthread_rwlock_arch_t *)0)->__flags), slate_oracle_struct___pthread_rwlock_arch_t___flags), "struct __pthread_rwlock_arch_t.__flags field type differs from oracle");

_Static_assert(sizeof(struct drand48_data) == 24, "struct drand48_data size differs from oracle");

_Static_assert(_Alignof(struct drand48_data) == 8, "struct drand48_data alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct drand48_data, __x) == 0, "struct drand48_data.__x offset differs from oracle");

_Static_assert(__builtin_offsetof(struct drand48_data, __old_x) == 6, "struct drand48_data.__old_x offset differs from oracle");

_Static_assert(__builtin_offsetof(struct drand48_data, __c) == 12, "struct drand48_data.__c offset differs from oracle");

typedef unsigned short slate_oracle_struct_drand48_data___c;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct drand48_data *)0)->__c), slate_oracle_struct_drand48_data___c), "struct drand48_data.__c field type differs from oracle");

_Static_assert(__builtin_offsetof(struct drand48_data, __init) == 14, "struct drand48_data.__init offset differs from oracle");

typedef unsigned short slate_oracle_struct_drand48_data___init;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct drand48_data *)0)->__init), slate_oracle_struct_drand48_data___init), "struct drand48_data.__init field type differs from oracle");

_Static_assert(__builtin_offsetof(struct drand48_data, __a) == 16, "struct drand48_data.__a offset differs from oracle");

typedef unsigned long long slate_oracle_struct_drand48_data___a;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct drand48_data *)0)->__a), slate_oracle_struct_drand48_data___a), "struct drand48_data.__a field type differs from oracle");

_Static_assert(sizeof(union pthread_attr_t) == 56, "union pthread_attr_t size differs from oracle");

_Static_assert(_Alignof(union pthread_attr_t) == 8, "union pthread_attr_t alignment differs from oracle");

_Static_assert(__builtin_offsetof(union pthread_attr_t, __size) == 0, "union pthread_attr_t.__size offset differs from oracle");

_Static_assert(__builtin_offsetof(union pthread_attr_t, __align) == 0, "union pthread_attr_t.__align offset differs from oracle");

typedef long slate_oracle_union_pthread_attr_t___align;
_Static_assert(__builtin_types_compatible_p(__typeof__((( union pthread_attr_t *)0)->__align), slate_oracle_union_pthread_attr_t___align), "union pthread_attr_t.__align field type differs from oracle");

_Static_assert(sizeof(struct random_data) == 48, "struct random_data size differs from oracle");

_Static_assert(_Alignof(struct random_data) == 8, "struct random_data alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct random_data, fptr) == 0, "struct random_data.fptr offset differs from oracle");

typedef int * slate_oracle_struct_random_data_fptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct random_data *)0)->fptr), slate_oracle_struct_random_data_fptr), "struct random_data.fptr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct random_data, rptr) == 8, "struct random_data.rptr offset differs from oracle");

typedef int * slate_oracle_struct_random_data_rptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct random_data *)0)->rptr), slate_oracle_struct_random_data_rptr), "struct random_data.rptr field type differs from oracle");

_Static_assert(__builtin_offsetof(struct random_data, state) == 16, "struct random_data.state offset differs from oracle");

typedef int * slate_oracle_struct_random_data_state;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct random_data *)0)->state), slate_oracle_struct_random_data_state), "struct random_data.state field type differs from oracle");

_Static_assert(__builtin_offsetof(struct random_data, rand_type) == 24, "struct random_data.rand_type offset differs from oracle");

typedef int slate_oracle_struct_random_data_rand_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct random_data *)0)->rand_type), slate_oracle_struct_random_data_rand_type), "struct random_data.rand_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct random_data, rand_deg) == 28, "struct random_data.rand_deg offset differs from oracle");

typedef int slate_oracle_struct_random_data_rand_deg;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct random_data *)0)->rand_deg), slate_oracle_struct_random_data_rand_deg), "struct random_data.rand_deg field type differs from oracle");

_Static_assert(__builtin_offsetof(struct random_data, rand_sep) == 32, "struct random_data.rand_sep offset differs from oracle");

typedef int slate_oracle_struct_random_data_rand_sep;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct random_data *)0)->rand_sep), slate_oracle_struct_random_data_rand_sep), "struct random_data.rand_sep field type differs from oracle");

_Static_assert(__builtin_offsetof(struct random_data, end_ptr) == 40, "struct random_data.end_ptr offset differs from oracle");

typedef int * slate_oracle_struct_random_data_end_ptr;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct random_data *)0)->end_ptr), slate_oracle_struct_random_data_end_ptr), "struct random_data.end_ptr field type differs from oracle");

_Static_assert(sizeof(struct timespec) == 16, "struct timespec size differs from oracle");

_Static_assert(_Alignof(struct timespec) == 8, "struct timespec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_sec) == 0, "struct timespec.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_sec), slate_oracle_struct_timespec_tv_sec), "struct timespec.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timespec, tv_nsec) == 8, "struct timespec.tv_nsec offset differs from oracle");

typedef long slate_oracle_struct_timespec_tv_nsec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timespec *)0)->tv_nsec), slate_oracle_struct_timespec_tv_nsec), "struct timespec.tv_nsec field type differs from oracle");

_Static_assert(sizeof(struct timeval) == 16, "struct timeval size differs from oracle");

_Static_assert(_Alignof(struct timeval) == 8, "struct timeval alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_sec) == 0, "struct timeval.tv_sec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_sec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_sec), slate_oracle_struct_timeval_tv_sec), "struct timeval.tv_sec field type differs from oracle");

_Static_assert(__builtin_offsetof(struct timeval, tv_usec) == 8, "struct timeval.tv_usec offset differs from oracle");

typedef long slate_oracle_struct_timeval_tv_usec;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct timeval *)0)->tv_usec), slate_oracle_struct_timeval_tv_usec), "struct timeval.tv_usec field type differs from oracle");

#ifndef BIG_ENDIAN
#error "stdlib.h:BIG_ENDIAN macro is missing from libc-shim"
#endif

#ifndef BYTE_ORDER
#error "stdlib.h:BYTE_ORDER macro is missing from libc-shim"
#endif

#ifndef EXIT_FAILURE
#error "stdlib.h:EXIT_FAILURE macro is missing from libc-shim"
#endif

#ifndef EXIT_SUCCESS
#error "stdlib.h:EXIT_SUCCESS macro is missing from libc-shim"
#endif

#ifndef FD_CLR
#error "stdlib.h:FD_CLR macro is missing from libc-shim"
#endif

#ifndef FD_ISSET
#error "stdlib.h:FD_ISSET macro is missing from libc-shim"
#endif

#ifndef FD_SET
#error "stdlib.h:FD_SET macro is missing from libc-shim"
#endif

#ifndef FD_SETSIZE
#error "stdlib.h:FD_SETSIZE macro is missing from libc-shim"
#endif

#ifndef FD_ZERO
#error "stdlib.h:FD_ZERO macro is missing from libc-shim"
#endif

#ifndef LITTLE_ENDIAN
#error "stdlib.h:LITTLE_ENDIAN macro is missing from libc-shim"
#endif

#ifndef MB_CUR_MAX
#error "stdlib.h:MB_CUR_MAX macro is missing from libc-shim"
#endif

#ifndef NFDBITS
#error "stdlib.h:NFDBITS macro is missing from libc-shim"
#endif

#ifndef NULL
#error "stdlib.h:NULL macro is missing from libc-shim"
#endif

#ifndef ONCE_FLAG_INIT
#error "stdlib.h:ONCE_FLAG_INIT macro is missing from libc-shim"
#endif

#ifndef PDP_ENDIAN
#error "stdlib.h:PDP_ENDIAN macro is missing from libc-shim"
#endif

#ifndef RAND_MAX
#error "stdlib.h:RAND_MAX macro is missing from libc-shim"
#endif

#ifndef WCONTINUED
#error "stdlib.h:WCONTINUED macro is missing from libc-shim"
#endif

#ifndef WEXITED
#error "stdlib.h:WEXITED macro is missing from libc-shim"
#endif

#ifndef WEXITSTATUS
#error "stdlib.h:WEXITSTATUS macro is missing from libc-shim"
#endif

#ifndef WIFCONTINUED
#error "stdlib.h:WIFCONTINUED macro is missing from libc-shim"
#endif

#ifndef WIFEXITED
#error "stdlib.h:WIFEXITED macro is missing from libc-shim"
#endif

#ifndef WIFSIGNALED
#error "stdlib.h:WIFSIGNALED macro is missing from libc-shim"
#endif

#ifndef WIFSTOPPED
#error "stdlib.h:WIFSTOPPED macro is missing from libc-shim"
#endif

#ifndef WNOHANG
#error "stdlib.h:WNOHANG macro is missing from libc-shim"
#endif

#ifndef WNOWAIT
#error "stdlib.h:WNOWAIT macro is missing from libc-shim"
#endif

#ifndef WSTOPPED
#error "stdlib.h:WSTOPPED macro is missing from libc-shim"
#endif

#ifndef WSTOPSIG
#error "stdlib.h:WSTOPSIG macro is missing from libc-shim"
#endif

#ifndef WTERMSIG
#error "stdlib.h:WTERMSIG macro is missing from libc-shim"
#endif

#ifndef WUNTRACED
#error "stdlib.h:WUNTRACED macro is missing from libc-shim"
#endif

#ifndef _ALLOCA_H
#error "stdlib.h:_ALLOCA_H macro is missing from libc-shim"
#endif

#ifndef _ATFILE_SOURCE
#error "stdlib.h:_ATFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _BITS_ATOMIC_WIDE_COUNTER_H
#error "stdlib.h:_BITS_ATOMIC_WIDE_COUNTER_H macro is missing from libc-shim"
#endif

#ifndef _BITS_BYTESWAP_H
#error "stdlib.h:_BITS_BYTESWAP_H macro is missing from libc-shim"
#endif

#ifndef _BITS_ENDIANNESS_H
#error "stdlib.h:_BITS_ENDIANNESS_H macro is missing from libc-shim"
#endif

#ifndef _BITS_ENDIAN_H
#error "stdlib.h:_BITS_ENDIAN_H macro is missing from libc-shim"
#endif

#ifndef _BITS_FLOATN_COMMON_H
#error "stdlib.h:_BITS_FLOATN_COMMON_H macro is missing from libc-shim"
#endif

#ifndef _BITS_FLOATN_H
#error "stdlib.h:_BITS_FLOATN_H macro is missing from libc-shim"
#endif

#ifndef _BITS_PTHREADTYPES_ARCH_H
#error "stdlib.h:_BITS_PTHREADTYPES_ARCH_H macro is missing from libc-shim"
#endif

#ifndef _BITS_PTHREADTYPES_COMMON_H
#error "stdlib.h:_BITS_PTHREADTYPES_COMMON_H macro is missing from libc-shim"
#endif

#ifndef _BITS_STDINT_INTN_H
#error "stdlib.h:_BITS_STDINT_INTN_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TIME64_H
#error "stdlib.h:_BITS_TIME64_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPESIZES_H
#error "stdlib.h:_BITS_TYPESIZES_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES_H
#error "stdlib.h:_BITS_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES_LOCALE_T_H
#error "stdlib.h:_BITS_TYPES_LOCALE_T_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES___LOCALE_T_H
#error "stdlib.h:_BITS_TYPES___LOCALE_T_H macro is missing from libc-shim"
#endif

#ifndef _BITS_UINTN_IDENTITY_H
#error "stdlib.h:_BITS_UINTN_IDENTITY_H macro is missing from libc-shim"
#endif

#ifndef _DEFAULT_SOURCE
#error "stdlib.h:_DEFAULT_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DYNAMIC_STACK_SIZE_SOURCE
#error "stdlib.h:_DYNAMIC_STACK_SIZE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ENDIAN_H
#error "stdlib.h:_ENDIAN_H macro is missing from libc-shim"
#endif

#ifndef _FEATURES_H
#error "stdlib.h:_FEATURES_H macro is missing from libc-shim"
#endif

#ifndef _ISOC11_SOURCE
#error "stdlib.h:_ISOC11_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC23_SOURCE
#error "stdlib.h:_ISOC23_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC2Y_SOURCE
#error "stdlib.h:_ISOC2Y_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC95_SOURCE
#error "stdlib.h:_ISOC95_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC99_SOURCE
#error "stdlib.h:_ISOC99_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE64_SOURCE
#error "stdlib.h:_LARGEFILE64_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE_SOURCE
#error "stdlib.h:_LARGEFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_C_SOURCE
#error "stdlib.h:_POSIX_C_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_SOURCE
#error "stdlib.h:_POSIX_SOURCE macro is missing from libc-shim"
#endif

#ifndef _RWLOCK_INTERNAL_H
#error "stdlib.h:_RWLOCK_INTERNAL_H macro is missing from libc-shim"
#endif

#ifndef _SIGSET_NWORDS
#error "stdlib.h:_SIGSET_NWORDS macro is missing from libc-shim"
#endif

#ifndef _SIZE_T
#error "stdlib.h:_SIZE_T macro is missing from libc-shim"
#endif

#ifndef _STDC_PREDEF_H
#error "stdlib.h:_STDC_PREDEF_H macro is missing from libc-shim"
#endif

#ifndef _STDLIB_H
#error "stdlib.h:_STDLIB_H macro is missing from libc-shim"
#endif

#ifndef _STRUCT_TIMESPEC
#error "stdlib.h:_STRUCT_TIMESPEC macro is missing from libc-shim"
#endif

#ifndef _SYS_CDEFS_H
#error "stdlib.h:_SYS_CDEFS_H macro is missing from libc-shim"
#endif

#ifndef _SYS_SELECT_H
#error "stdlib.h:_SYS_SELECT_H macro is missing from libc-shim"
#endif

#ifndef _SYS_TYPES_H
#error "stdlib.h:_SYS_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _THREAD_MUTEX_INTERNAL_H
#error "stdlib.h:_THREAD_MUTEX_INTERNAL_H macro is missing from libc-shim"
#endif

#ifndef _THREAD_SHARED_TYPES_H
#error "stdlib.h:_THREAD_SHARED_TYPES_H macro is missing from libc-shim"
#endif

#ifndef _WCHAR_T
#error "stdlib.h:_WCHAR_T macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE
#error "stdlib.h:_XOPEN_SOURCE macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE_EXTENDED
#error "stdlib.h:_XOPEN_SOURCE_EXTENDED macro is missing from libc-shim"
#endif

#ifndef alloca
#error "stdlib.h:alloca macro is missing from libc-shim"
#endif

#ifndef be16toh
#error "stdlib.h:be16toh macro is missing from libc-shim"
#endif

#ifndef be32toh
#error "stdlib.h:be32toh macro is missing from libc-shim"
#endif

#ifndef be64toh
#error "stdlib.h:be64toh macro is missing from libc-shim"
#endif

#ifndef bsearch
#error "stdlib.h:bsearch macro is missing from libc-shim"
#endif

#ifndef htobe16
#error "stdlib.h:htobe16 macro is missing from libc-shim"
#endif

#ifndef htobe32
#error "stdlib.h:htobe32 macro is missing from libc-shim"
#endif

#ifndef htobe64
#error "stdlib.h:htobe64 macro is missing from libc-shim"
#endif

#ifndef htole16
#error "stdlib.h:htole16 macro is missing from libc-shim"
#endif

#ifndef htole32
#error "stdlib.h:htole32 macro is missing from libc-shim"
#endif

#ifndef htole64
#error "stdlib.h:htole64 macro is missing from libc-shim"
#endif

#ifndef le16toh
#error "stdlib.h:le16toh macro is missing from libc-shim"
#endif

#ifndef le32toh
#error "stdlib.h:le32toh macro is missing from libc-shim"
#endif

#ifndef le64toh
#error "stdlib.h:le64toh macro is missing from libc-shim"
#endif

int main(void) { return 0; }
