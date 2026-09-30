#include <stdlib.h>

extern void slate_oracle__Exit(int) __attribute__((noreturn));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__Exit), __typeof__(_Exit)),
    "stdlib.h:_Exit declaration differs from oracle");

static __typeof__(_Exit) *const slate_reference__Exit = &_Exit;

extern int slate_oracle____mb_cur_max_func(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle____mb_cur_max_func), __typeof__(___mb_cur_max_func)),
    "stdlib.h:___mb_cur_max_func declaration differs from oracle");

static __typeof__(___mb_cur_max_func) *const slate_reference____mb_cur_max_func = &___mb_cur_max_func;

extern int slate_oracle____mb_cur_max_l_func(_locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle____mb_cur_max_l_func), __typeof__(___mb_cur_max_l_func)),
    "stdlib.h:___mb_cur_max_l_func declaration differs from oracle");

static __typeof__(___mb_cur_max_l_func) *const slate_reference____mb_cur_max_l_func = &___mb_cur_max_l_func;

extern unsigned long * slate_oracle___doserrno(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___doserrno), __typeof__(__doserrno)),
    "stdlib.h:__doserrno declaration differs from oracle");

static __typeof__(__doserrno) *const slate_reference___doserrno = &__doserrno;

extern int * slate_oracle___p___argc(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___p___argc), __typeof__(__p___argc)),
    "stdlib.h:__p___argc declaration differs from oracle");

static __typeof__(__p___argc) *const slate_reference___p___argc = &__p___argc;

extern char *** slate_oracle___p___argv(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___p___argv), __typeof__(__p___argv)),
    "stdlib.h:__p___argv declaration differs from oracle");

static __typeof__(__p___argv) *const slate_reference___p___argv = &__p___argv;

extern unsigned short *** slate_oracle___p___wargv(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___p___wargv), __typeof__(__p___wargv)),
    "stdlib.h:__p___wargv declaration differs from oracle");

static __typeof__(__p___wargv) *const slate_reference___p___wargv = &__p___wargv;

extern char *** slate_oracle___p__environ(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___p__environ), __typeof__(__p__environ)),
    "stdlib.h:__p__environ declaration differs from oracle");

static __typeof__(__p__environ) *const slate_reference___p__environ = &__p__environ;

extern int * slate_oracle___p__fmode(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___p__fmode), __typeof__(__p__fmode)),
    "stdlib.h:__p__fmode declaration differs from oracle");

static __typeof__(__p__fmode) *const slate_reference___p__fmode = &__p__fmode;

extern char ** slate_oracle___p__pgmptr(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___p__pgmptr), __typeof__(__p__pgmptr)),
    "stdlib.h:__p__pgmptr declaration differs from oracle");

static __typeof__(__p__pgmptr) *const slate_reference___p__pgmptr = &__p__pgmptr;

extern unsigned short *** slate_oracle___p__wenviron(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___p__wenviron), __typeof__(__p__wenviron)),
    "stdlib.h:__p__wenviron declaration differs from oracle");

static __typeof__(__p__wenviron) *const slate_reference___p__wenviron = &__p__wenviron;

extern unsigned short ** slate_oracle___p__wpgmptr(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___p__wpgmptr), __typeof__(__p__wpgmptr)),
    "stdlib.h:__p__wpgmptr declaration differs from oracle");

static __typeof__(__p__wpgmptr) *const slate_reference___p__wpgmptr = &__p__wpgmptr;

extern char ** slate_oracle___sys_errlist(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___sys_errlist), __typeof__(__sys_errlist)),
    "stdlib.h:__sys_errlist declaration differs from oracle");

static __typeof__(__sys_errlist) *const slate_reference___sys_errlist = &__sys_errlist;

extern int * slate_oracle___sys_nerr(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___sys_nerr), __typeof__(__sys_nerr)),
    "stdlib.h:__sys_nerr declaration differs from oracle");

static __typeof__(__sys_nerr) *const slate_reference___sys_nerr = &__sys_nerr;

extern long long slate_oracle__abs64(long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__abs64), __typeof__(_abs64)),
    "stdlib.h:_abs64 declaration differs from oracle");

static __typeof__(_abs64) *const slate_reference__abs64 = &_abs64;

extern int slate_oracle__atodbl(_CRT_DOUBLE *, char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atodbl), __typeof__(_atodbl)),
    "stdlib.h:_atodbl declaration differs from oracle");

static __typeof__(_atodbl) *const slate_reference__atodbl = &_atodbl;

extern int slate_oracle__atodbl_l(_CRT_DOUBLE *, char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atodbl_l), __typeof__(_atodbl_l)),
    "stdlib.h:_atodbl_l declaration differs from oracle");

static __typeof__(_atodbl_l) *const slate_reference__atodbl_l = &_atodbl_l;

extern double slate_oracle__atof_l(const char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atof_l), __typeof__(_atof_l)),
    "stdlib.h:_atof_l declaration differs from oracle");

static __typeof__(_atof_l) *const slate_reference__atof_l = &_atof_l;

extern int slate_oracle__atoflt(_CRT_FLOAT *, const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atoflt), __typeof__(_atoflt)),
    "stdlib.h:_atoflt declaration differs from oracle");

static __typeof__(_atoflt) *const slate_reference__atoflt = &_atoflt;

extern int slate_oracle__atoflt_l(_CRT_FLOAT *, const char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atoflt_l), __typeof__(_atoflt_l)),
    "stdlib.h:_atoflt_l declaration differs from oracle");

static __typeof__(_atoflt_l) *const slate_reference__atoflt_l = &_atoflt_l;

extern long long slate_oracle__atoi64(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atoi64), __typeof__(_atoi64)),
    "stdlib.h:_atoi64 declaration differs from oracle");

static __typeof__(_atoi64) *const slate_reference__atoi64 = &_atoi64;

extern long long slate_oracle__atoi64_l(const char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atoi64_l), __typeof__(_atoi64_l)),
    "stdlib.h:_atoi64_l declaration differs from oracle");

static __typeof__(_atoi64_l) *const slate_reference__atoi64_l = &_atoi64_l;

extern int slate_oracle__atoi_l(const char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atoi_l), __typeof__(_atoi_l)),
    "stdlib.h:_atoi_l declaration differs from oracle");

static __typeof__(_atoi_l) *const slate_reference__atoi_l = &_atoi_l;

extern long slate_oracle__atol_l(const char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atol_l), __typeof__(_atol_l)),
    "stdlib.h:_atol_l declaration differs from oracle");

static __typeof__(_atol_l) *const slate_reference__atol_l = &_atol_l;

extern int slate_oracle__atoldbl(_LDOUBLE *, char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atoldbl), __typeof__(_atoldbl)),
    "stdlib.h:_atoldbl declaration differs from oracle");

static __typeof__(_atoldbl) *const slate_reference__atoldbl = &_atoldbl;

extern int slate_oracle__atoldbl_l(_LDOUBLE *, char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atoldbl_l), __typeof__(_atoldbl_l)),
    "stdlib.h:_atoldbl_l declaration differs from oracle");

static __typeof__(_atoldbl_l) *const slate_reference__atoldbl_l = &_atoldbl_l;

extern long long slate_oracle__atoll_l(const char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atoll_l), __typeof__(_atoll_l)),
    "stdlib.h:_atoll_l declaration differs from oracle");

static __typeof__(_atoll_l) *const slate_reference__atoll_l = &_atoll_l;

extern void slate_oracle__beep(unsigned int, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__beep), __typeof__(_beep)),
    "stdlib.h:_beep declaration differs from oracle");

static __typeof__(_beep) *const slate_reference__beep = &_beep;

extern unsigned long long slate_oracle__byteswap_uint64(unsigned long long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__byteswap_uint64), __typeof__(_byteswap_uint64)),
    "stdlib.h:_byteswap_uint64 declaration differs from oracle");

static __typeof__(_byteswap_uint64) *const slate_reference__byteswap_uint64 = &_byteswap_uint64;

extern unsigned long slate_oracle__byteswap_ulong(unsigned long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__byteswap_ulong), __typeof__(_byteswap_ulong)),
    "stdlib.h:_byteswap_ulong declaration differs from oracle");

static __typeof__(_byteswap_ulong) *const slate_reference__byteswap_ulong = &_byteswap_ulong;

extern unsigned short slate_oracle__byteswap_ushort(unsigned short);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__byteswap_ushort), __typeof__(_byteswap_ushort)),
    "stdlib.h:_byteswap_ushort declaration differs from oracle");

static __typeof__(_byteswap_ushort) *const slate_reference__byteswap_ushort = &_byteswap_ushort;

extern int slate_oracle__dupenv_s(char **, unsigned long long *, const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dupenv_s), __typeof__(_dupenv_s)),
    "stdlib.h:_dupenv_s declaration differs from oracle");

static __typeof__(_dupenv_s) *const slate_reference__dupenv_s = &_dupenv_s;

extern char * slate_oracle__ecvt(double, int, int *, int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ecvt), __typeof__(_ecvt)),
    "stdlib.h:_ecvt declaration differs from oracle");

static __typeof__(_ecvt) *const slate_reference__ecvt = &_ecvt;

extern int slate_oracle__ecvt_s(char *, unsigned long long, double, int, int *, int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ecvt_s), __typeof__(_ecvt_s)),
    "stdlib.h:_ecvt_s declaration differs from oracle");

static __typeof__(_ecvt_s) *const slate_reference__ecvt_s = &_ecvt_s;

extern int * slate_oracle__errno(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__errno), __typeof__(_errno)),
    "stdlib.h:_errno declaration differs from oracle");

static __typeof__(_errno) *const slate_reference__errno = &_errno;

extern void slate_oracle__exit(int) __attribute__((noreturn));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__exit), __typeof__(_exit)),
    "stdlib.h:_exit declaration differs from oracle");

static __typeof__(_exit) *const slate_reference__exit = &_exit;

extern char * slate_oracle__fcvt(double, int, int *, int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fcvt), __typeof__(_fcvt)),
    "stdlib.h:_fcvt declaration differs from oracle");

static __typeof__(_fcvt) *const slate_reference__fcvt = &_fcvt;

extern int slate_oracle__fcvt_s(char *, unsigned long long, double, int, int *, int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fcvt_s), __typeof__(_fcvt_s)),
    "stdlib.h:_fcvt_s declaration differs from oracle");

static __typeof__(_fcvt_s) *const slate_reference__fcvt_s = &_fcvt_s;

extern char * slate_oracle__fullpath(char *, const char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fullpath), __typeof__(_fullpath)),
    "stdlib.h:_fullpath declaration differs from oracle");

static __typeof__(_fullpath) *const slate_reference__fullpath = &_fullpath;

extern char * slate_oracle__gcvt(double, int, char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__gcvt), __typeof__(_gcvt)),
    "stdlib.h:_gcvt declaration differs from oracle");

static __typeof__(_gcvt) *const slate_reference__gcvt = &_gcvt;

extern int slate_oracle__gcvt_s(char *, unsigned long long, double, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__gcvt_s), __typeof__(_gcvt_s)),
    "stdlib.h:_gcvt_s declaration differs from oracle");

static __typeof__(_gcvt_s) *const slate_reference__gcvt_s = &_gcvt_s;

extern int slate_oracle__get_doserrno(unsigned long *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_doserrno), __typeof__(_get_doserrno)),
    "stdlib.h:_get_doserrno declaration differs from oracle");

static __typeof__(_get_doserrno) *const slate_reference__get_doserrno = &_get_doserrno;

extern int slate_oracle__get_errno(int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_errno), __typeof__(_get_errno)),
    "stdlib.h:_get_errno declaration differs from oracle");

static __typeof__(_get_errno) *const slate_reference__get_errno = &_get_errno;

extern int slate_oracle__get_fmode(int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_fmode), __typeof__(_get_fmode)),
    "stdlib.h:_get_fmode declaration differs from oracle");

static __typeof__(_get_fmode) *const slate_reference__get_fmode = &_get_fmode;

extern void slate_oracle__get_invalid_parameter_handler(*)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) __attribute__((cdecl)) (void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_invalid_parameter_handler), __typeof__(_get_invalid_parameter_handler)),
    "stdlib.h:_get_invalid_parameter_handler declaration differs from oracle");

static __typeof__(_get_invalid_parameter_handler) *const slate_reference__get_invalid_parameter_handler = &_get_invalid_parameter_handler;

extern int slate_oracle__get_pgmptr(char **) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_pgmptr), __typeof__(_get_pgmptr)),
    "stdlib.h:_get_pgmptr declaration differs from oracle");

static __typeof__(_get_pgmptr) *const slate_reference__get_pgmptr = &_get_pgmptr;

extern void slate_oracle__get_purecall_handler(*)(void) __attribute__((cdecl)) (void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_purecall_handler), __typeof__(_get_purecall_handler)),
    "stdlib.h:_get_purecall_handler declaration differs from oracle");

static __typeof__(_get_purecall_handler) *const slate_reference__get_purecall_handler = &_get_purecall_handler;

extern void slate_oracle__get_thread_local_invalid_parameter_handler(*)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) __attribute__((cdecl)) (void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_thread_local_invalid_parameter_handler), __typeof__(_get_thread_local_invalid_parameter_handler)),
    "stdlib.h:_get_thread_local_invalid_parameter_handler declaration differs from oracle");

static __typeof__(_get_thread_local_invalid_parameter_handler) *const slate_reference__get_thread_local_invalid_parameter_handler = &_get_thread_local_invalid_parameter_handler;

extern int slate_oracle__get_wpgmptr(unsigned short **) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_wpgmptr), __typeof__(_get_wpgmptr)),
    "stdlib.h:_get_wpgmptr declaration differs from oracle");

static __typeof__(_get_wpgmptr) *const slate_reference__get_wpgmptr = &_get_wpgmptr;

extern char * slate_oracle__i64toa(long long, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__i64toa), __typeof__(_i64toa)),
    "stdlib.h:_i64toa declaration differs from oracle");

static __typeof__(_i64toa) *const slate_reference__i64toa = &_i64toa;

extern int slate_oracle__i64toa_s(long long, char *, unsigned long long, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__i64toa_s), __typeof__(_i64toa_s)),
    "stdlib.h:_i64toa_s declaration differs from oracle");

static __typeof__(_i64toa_s) *const slate_reference__i64toa_s = &_i64toa_s;

extern char * slate_oracle__itoa(int, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__itoa), __typeof__(_itoa)),
    "stdlib.h:_itoa declaration differs from oracle");

static __typeof__(_itoa) *const slate_reference__itoa = &_itoa;

extern int slate_oracle__itoa_s(int, char *, unsigned long long, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__itoa_s), __typeof__(_itoa_s)),
    "stdlib.h:_itoa_s declaration differs from oracle");

static __typeof__(_itoa_s) *const slate_reference__itoa_s = &_itoa_s;

extern unsigned long slate_oracle__lrotl(unsigned long, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__lrotl), __typeof__(_lrotl)),
    "stdlib.h:_lrotl declaration differs from oracle");

static __typeof__(_lrotl) *const slate_reference__lrotl = &_lrotl;

extern unsigned long slate_oracle__lrotr(unsigned long, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__lrotr), __typeof__(_lrotr)),
    "stdlib.h:_lrotr declaration differs from oracle");

static __typeof__(_lrotr) *const slate_reference__lrotr = &_lrotr;

extern char * slate_oracle__ltoa(long, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ltoa), __typeof__(_ltoa)),
    "stdlib.h:_ltoa declaration differs from oracle");

static __typeof__(_ltoa) *const slate_reference__ltoa = &_ltoa;

extern int slate_oracle__ltoa_s(long, char *, unsigned long long, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ltoa_s), __typeof__(_ltoa_s)),
    "stdlib.h:_ltoa_s declaration differs from oracle");

static __typeof__(_ltoa_s) *const slate_reference__ltoa_s = &_ltoa_s;

extern void slate_oracle__makepath(char *, const char *, const char *, const char *, const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__makepath), __typeof__(_makepath)),
    "stdlib.h:_makepath declaration differs from oracle");

static __typeof__(_makepath) *const slate_reference__makepath = &_makepath;

extern int slate_oracle__makepath_s(char *, unsigned long long, const char *, const char *, const char *, const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__makepath_s), __typeof__(_makepath_s)),
    "stdlib.h:_makepath_s declaration differs from oracle");

static __typeof__(_makepath_s) *const slate_reference__makepath_s = &_makepath_s;

extern int slate_oracle__mblen_l(const char *, unsigned long long, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mblen_l), __typeof__(_mblen_l)),
    "stdlib.h:_mblen_l declaration differs from oracle");

static __typeof__(_mblen_l) *const slate_reference__mblen_l = &_mblen_l;

extern unsigned long long slate_oracle__mbstowcs_l(unsigned short *, const char *, unsigned long long, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mbstowcs_l), __typeof__(_mbstowcs_l)),
    "stdlib.h:_mbstowcs_l declaration differs from oracle");

static __typeof__(_mbstowcs_l) *const slate_reference__mbstowcs_l = &_mbstowcs_l;

extern int slate_oracle__mbstowcs_s_l(unsigned long long *, unsigned short *, unsigned long long, const char *, unsigned long long, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mbstowcs_s_l), __typeof__(_mbstowcs_s_l)),
    "stdlib.h:_mbstowcs_s_l declaration differs from oracle");

static __typeof__(_mbstowcs_s_l) *const slate_reference__mbstowcs_s_l = &_mbstowcs_s_l;

extern unsigned long long slate_oracle__mbstrlen(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mbstrlen), __typeof__(_mbstrlen)),
    "stdlib.h:_mbstrlen declaration differs from oracle");

static __typeof__(_mbstrlen) *const slate_reference__mbstrlen = &_mbstrlen;

extern unsigned long long slate_oracle__mbstrlen_l(const char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mbstrlen_l), __typeof__(_mbstrlen_l)),
    "stdlib.h:_mbstrlen_l declaration differs from oracle");

static __typeof__(_mbstrlen_l) *const slate_reference__mbstrlen_l = &_mbstrlen_l;

extern unsigned long long slate_oracle__mbstrnlen(const char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mbstrnlen), __typeof__(_mbstrnlen)),
    "stdlib.h:_mbstrnlen declaration differs from oracle");

static __typeof__(_mbstrnlen) *const slate_reference__mbstrnlen = &_mbstrnlen;

extern unsigned long long slate_oracle__mbstrnlen_l(const char *, unsigned long long, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mbstrnlen_l), __typeof__(_mbstrnlen_l)),
    "stdlib.h:_mbstrnlen_l declaration differs from oracle");

static __typeof__(_mbstrnlen_l) *const slate_reference__mbstrnlen_l = &_mbstrnlen_l;

extern int slate_oracle__mbtowc_l(unsigned short *, const char *, unsigned long long, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__mbtowc_l), __typeof__(_mbtowc_l)),
    "stdlib.h:_mbtowc_l declaration differs from oracle");

static __typeof__(_mbtowc_l) *const slate_reference__mbtowc_l = &_mbtowc_l;

extern int slate_oracle__onexit(*)(void) __attribute__((cdecl)) (int (*)(void) __attribute__((cdecl))) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__onexit), __typeof__(_onexit)),
    "stdlib.h:_onexit declaration differs from oracle");

static __typeof__(_onexit) *const slate_reference__onexit = &_onexit;

extern int slate_oracle__putenv(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__putenv), __typeof__(_putenv)),
    "stdlib.h:_putenv declaration differs from oracle");

static __typeof__(_putenv) *const slate_reference__putenv = &_putenv;

extern int slate_oracle__putenv_s(const char *, const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__putenv_s), __typeof__(_putenv_s)),
    "stdlib.h:_putenv_s declaration differs from oracle");

static __typeof__(_putenv_s) *const slate_reference__putenv_s = &_putenv_s;

extern unsigned int slate_oracle__rotl(unsigned int, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__rotl), __typeof__(_rotl)),
    "stdlib.h:_rotl declaration differs from oracle");

static __typeof__(_rotl) *const slate_reference__rotl = &_rotl;

extern unsigned long long slate_oracle__rotl64(unsigned long long, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__rotl64), __typeof__(_rotl64)),
    "stdlib.h:_rotl64 declaration differs from oracle");

static __typeof__(_rotl64) *const slate_reference__rotl64 = &_rotl64;

extern unsigned int slate_oracle__rotr(unsigned int, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__rotr), __typeof__(_rotr)),
    "stdlib.h:_rotr declaration differs from oracle");

static __typeof__(_rotr) *const slate_reference__rotr = &_rotr;

extern unsigned long long slate_oracle__rotr64(unsigned long long, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__rotr64), __typeof__(_rotr64)),
    "stdlib.h:_rotr64 declaration differs from oracle");

static __typeof__(_rotr64) *const slate_reference__rotr64 = &_rotr64;

extern void slate_oracle__searchenv(const char *, const char *, char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__searchenv), __typeof__(_searchenv)),
    "stdlib.h:_searchenv declaration differs from oracle");

static __typeof__(_searchenv) *const slate_reference__searchenv = &_searchenv;

extern int slate_oracle__searchenv_s(const char *, const char *, char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__searchenv_s), __typeof__(_searchenv_s)),
    "stdlib.h:_searchenv_s declaration differs from oracle");

static __typeof__(_searchenv_s) *const slate_reference__searchenv_s = &_searchenv_s;

extern unsigned int slate_oracle__set_abort_behavior(unsigned int, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_abort_behavior), __typeof__(_set_abort_behavior)),
    "stdlib.h:_set_abort_behavior declaration differs from oracle");

static __typeof__(_set_abort_behavior) *const slate_reference__set_abort_behavior = &_set_abort_behavior;

extern int slate_oracle__set_doserrno(unsigned long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_doserrno), __typeof__(_set_doserrno)),
    "stdlib.h:_set_doserrno declaration differs from oracle");

static __typeof__(_set_doserrno) *const slate_reference__set_doserrno = &_set_doserrno;

extern int slate_oracle__set_errno(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_errno), __typeof__(_set_errno)),
    "stdlib.h:_set_errno declaration differs from oracle");

static __typeof__(_set_errno) *const slate_reference__set_errno = &_set_errno;

extern int slate_oracle__set_error_mode(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_error_mode), __typeof__(_set_error_mode)),
    "stdlib.h:_set_error_mode declaration differs from oracle");

static __typeof__(_set_error_mode) *const slate_reference__set_error_mode = &_set_error_mode;

extern int slate_oracle__set_fmode(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_fmode), __typeof__(_set_fmode)),
    "stdlib.h:_set_fmode declaration differs from oracle");

static __typeof__(_set_fmode) *const slate_reference__set_fmode = &_set_fmode;

extern void slate_oracle__set_invalid_parameter_handler(*)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) __attribute__((cdecl)) (void (*)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) __attribute__((cdecl))) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_invalid_parameter_handler), __typeof__(_set_invalid_parameter_handler)),
    "stdlib.h:_set_invalid_parameter_handler declaration differs from oracle");

static __typeof__(_set_invalid_parameter_handler) *const slate_reference__set_invalid_parameter_handler = &_set_invalid_parameter_handler;

extern void slate_oracle__set_purecall_handler(*)(void) __attribute__((cdecl)) (void (*)(void) __attribute__((cdecl))) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_purecall_handler), __typeof__(_set_purecall_handler)),
    "stdlib.h:_set_purecall_handler declaration differs from oracle");

static __typeof__(_set_purecall_handler) *const slate_reference__set_purecall_handler = &_set_purecall_handler;

extern void slate_oracle__set_thread_local_invalid_parameter_handler(*)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) __attribute__((cdecl)) (void (*)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) __attribute__((cdecl))) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_thread_local_invalid_parameter_handler), __typeof__(_set_thread_local_invalid_parameter_handler)),
    "stdlib.h:_set_thread_local_invalid_parameter_handler declaration differs from oracle");

static __typeof__(_set_thread_local_invalid_parameter_handler) *const slate_reference__set_thread_local_invalid_parameter_handler = &_set_thread_local_invalid_parameter_handler;

extern void slate_oracle__seterrormode(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__seterrormode), __typeof__(_seterrormode)),
    "stdlib.h:_seterrormode declaration differs from oracle");

static __typeof__(_seterrormode) *const slate_reference__seterrormode = &_seterrormode;

extern void slate_oracle__sleep(unsigned long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__sleep), __typeof__(_sleep)),
    "stdlib.h:_sleep declaration differs from oracle");

static __typeof__(_sleep) *const slate_reference__sleep = &_sleep;

extern void slate_oracle__splitpath(const char *, char *, char *, char *, char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__splitpath), __typeof__(_splitpath)),
    "stdlib.h:_splitpath declaration differs from oracle");

static __typeof__(_splitpath) *const slate_reference__splitpath = &_splitpath;

extern int slate_oracle__splitpath_s(const char *, char *, unsigned long long, char *, unsigned long long, char *, unsigned long long, char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__splitpath_s), __typeof__(_splitpath_s)),
    "stdlib.h:_splitpath_s declaration differs from oracle");

static __typeof__(_splitpath_s) *const slate_reference__splitpath_s = &_splitpath_s;

extern double slate_oracle__strtod_l(const char *, char **, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtod_l), __typeof__(_strtod_l)),
    "stdlib.h:_strtod_l declaration differs from oracle");

static __typeof__(_strtod_l) *const slate_reference__strtod_l = &_strtod_l;

extern float slate_oracle__strtof_l(const char *, char **, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtof_l), __typeof__(_strtof_l)),
    "stdlib.h:_strtof_l declaration differs from oracle");

static __typeof__(_strtof_l) *const slate_reference__strtof_l = &_strtof_l;

extern long long slate_oracle__strtoi64(const char *, char **, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtoi64), __typeof__(_strtoi64)),
    "stdlib.h:_strtoi64 declaration differs from oracle");

static __typeof__(_strtoi64) *const slate_reference__strtoi64 = &_strtoi64;

extern long long slate_oracle__strtoi64_l(const char *, char **, int, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtoi64_l), __typeof__(_strtoi64_l)),
    "stdlib.h:_strtoi64_l declaration differs from oracle");

static __typeof__(_strtoi64_l) *const slate_reference__strtoi64_l = &_strtoi64_l;

extern long slate_oracle__strtol_l(const char *, char **, int, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtol_l), __typeof__(_strtol_l)),
    "stdlib.h:_strtol_l declaration differs from oracle");

static __typeof__(_strtol_l) *const slate_reference__strtol_l = &_strtol_l;

extern long double slate_oracle__strtold_l(const char *, char **, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtold_l), __typeof__(_strtold_l)),
    "stdlib.h:_strtold_l declaration differs from oracle");

static __typeof__(_strtold_l) *const slate_reference__strtold_l = &_strtold_l;

extern long long slate_oracle__strtoll_l(const char *, char **, int, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtoll_l), __typeof__(_strtoll_l)),
    "stdlib.h:_strtoll_l declaration differs from oracle");

static __typeof__(_strtoll_l) *const slate_reference__strtoll_l = &_strtoll_l;

extern unsigned long long slate_oracle__strtoui64(const char *, char **, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtoui64), __typeof__(_strtoui64)),
    "stdlib.h:_strtoui64 declaration differs from oracle");

static __typeof__(_strtoui64) *const slate_reference__strtoui64 = &_strtoui64;

extern unsigned long long slate_oracle__strtoui64_l(const char *, char **, int, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtoui64_l), __typeof__(_strtoui64_l)),
    "stdlib.h:_strtoui64_l declaration differs from oracle");

static __typeof__(_strtoui64_l) *const slate_reference__strtoui64_l = &_strtoui64_l;

extern unsigned long slate_oracle__strtoul_l(const char *, char **, int, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtoul_l), __typeof__(_strtoul_l)),
    "stdlib.h:_strtoul_l declaration differs from oracle");

static __typeof__(_strtoul_l) *const slate_reference__strtoul_l = &_strtoul_l;

extern unsigned long long slate_oracle__strtoull_l(const char *, char **, int, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__strtoull_l), __typeof__(_strtoull_l)),
    "stdlib.h:_strtoull_l declaration differs from oracle");

static __typeof__(_strtoull_l) *const slate_reference__strtoull_l = &_strtoull_l;

extern void slate_oracle__swab(char *, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__swab), __typeof__(_swab)),
    "stdlib.h:_swab declaration differs from oracle");

static __typeof__(_swab) *const slate_reference__swab = &_swab;

extern char * slate_oracle__ui64toa(unsigned long long, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ui64toa), __typeof__(_ui64toa)),
    "stdlib.h:_ui64toa declaration differs from oracle");

static __typeof__(_ui64toa) *const slate_reference__ui64toa = &_ui64toa;

extern int slate_oracle__ui64toa_s(unsigned long long, char *, unsigned long long, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ui64toa_s), __typeof__(_ui64toa_s)),
    "stdlib.h:_ui64toa_s declaration differs from oracle");

static __typeof__(_ui64toa_s) *const slate_reference__ui64toa_s = &_ui64toa_s;

extern char * slate_oracle__ultoa(unsigned long, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ultoa), __typeof__(_ultoa)),
    "stdlib.h:_ultoa declaration differs from oracle");

static __typeof__(_ultoa) *const slate_reference__ultoa = &_ultoa;

extern int slate_oracle__ultoa_s(unsigned long, char *, unsigned long long, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ultoa_s), __typeof__(_ultoa_s)),
    "stdlib.h:_ultoa_s declaration differs from oracle");

static __typeof__(_ultoa_s) *const slate_reference__ultoa_s = &_ultoa_s;

extern unsigned long long slate_oracle__wcstombs_l(char *, const unsigned short *, unsigned long long, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wcstombs_l), __typeof__(_wcstombs_l)),
    "stdlib.h:_wcstombs_l declaration differs from oracle");

static __typeof__(_wcstombs_l) *const slate_reference__wcstombs_l = &_wcstombs_l;

extern int slate_oracle__wcstombs_s_l(unsigned long long *, char *, unsigned long long, const unsigned short *, unsigned long long, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wcstombs_s_l), __typeof__(_wcstombs_s_l)),
    "stdlib.h:_wcstombs_s_l declaration differs from oracle");

static __typeof__(_wcstombs_s_l) *const slate_reference__wcstombs_s_l = &_wcstombs_s_l;

extern int slate_oracle__wctomb_l(char *, unsigned short, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wctomb_l), __typeof__(_wctomb_l)),
    "stdlib.h:_wctomb_l declaration differs from oracle");

static __typeof__(_wctomb_l) *const slate_reference__wctomb_l = &_wctomb_l;

extern int slate_oracle__wctomb_s_l(int *, char *, unsigned long long, unsigned short, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wctomb_s_l), __typeof__(_wctomb_s_l)),
    "stdlib.h:_wctomb_s_l declaration differs from oracle");

static __typeof__(_wctomb_s_l) *const slate_reference__wctomb_s_l = &_wctomb_s_l;

extern void slate_oracle_abort(void) __attribute__((noreturn));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_abort), __typeof__(abort)),
    "stdlib.h:abort declaration differs from oracle");

static __typeof__(abort) *const slate_reference_abort = &abort;

extern int slate_oracle_abs(int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_abs), __typeof__(abs)),
    "stdlib.h:abs declaration differs from oracle");

static __typeof__(abs) *const slate_reference_abs = &abs;

extern int slate_oracle_at_quick_exit(void (*)(void) __attribute__((cdecl))) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_at_quick_exit), __typeof__(at_quick_exit)),
    "stdlib.h:at_quick_exit declaration differs from oracle");

static __typeof__(at_quick_exit) *const slate_reference_at_quick_exit = &at_quick_exit;

extern int slate_oracle_atexit(void (*)(void) __attribute__((cdecl))) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atexit), __typeof__(atexit)),
    "stdlib.h:atexit declaration differs from oracle");

static __typeof__(atexit) *const slate_reference_atexit = &atexit;

extern double slate_oracle_atof(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atof), __typeof__(atof)),
    "stdlib.h:atof declaration differs from oracle");

static __typeof__(atof) *const slate_reference_atof = &atof;

extern int slate_oracle_atoi(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atoi), __typeof__(atoi)),
    "stdlib.h:atoi declaration differs from oracle");

static __typeof__(atoi) *const slate_reference_atoi = &atoi;

extern long slate_oracle_atol(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atol), __typeof__(atol)),
    "stdlib.h:atol declaration differs from oracle");

static __typeof__(atol) *const slate_reference_atol = &atol;

extern long long slate_oracle_atoll(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atoll), __typeof__(atoll)),
    "stdlib.h:atoll declaration differs from oracle");

static __typeof__(atoll) *const slate_reference_atoll = &atoll;

extern struct _div_t slate_oracle_div(int, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_div), __typeof__(div)),
    "stdlib.h:div declaration differs from oracle");

static __typeof__(div) *const slate_reference_div = &div;

extern char * slate_oracle_ecvt(double, int, int *, int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ecvt), __typeof__(ecvt)),
    "stdlib.h:ecvt declaration differs from oracle");

static __typeof__(ecvt) *const slate_reference_ecvt = &ecvt;

extern void slate_oracle_exit(int) __attribute__((noreturn));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exit), __typeof__(exit)),
    "stdlib.h:exit declaration differs from oracle");

static __typeof__(exit) *const slate_reference_exit = &exit;

extern char * slate_oracle_fcvt(double, int, int *, int *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fcvt), __typeof__(fcvt)),
    "stdlib.h:fcvt declaration differs from oracle");

static __typeof__(fcvt) *const slate_reference_fcvt = &fcvt;

extern char * slate_oracle_gcvt(double, int, char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gcvt), __typeof__(gcvt)),
    "stdlib.h:gcvt declaration differs from oracle");

static __typeof__(gcvt) *const slate_reference_gcvt = &gcvt;

extern char * slate_oracle_getenv(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getenv), __typeof__(getenv)),
    "stdlib.h:getenv declaration differs from oracle");

static __typeof__(getenv) *const slate_reference_getenv = &getenv;

extern int slate_oracle_getenv_s(unsigned long long *, char *, unsigned long long, const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getenv_s), __typeof__(getenv_s)),
    "stdlib.h:getenv_s declaration differs from oracle");

static __typeof__(getenv_s) *const slate_reference_getenv_s = &getenv_s;

extern char * slate_oracle_itoa(int, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_itoa), __typeof__(itoa)),
    "stdlib.h:itoa declaration differs from oracle");

static __typeof__(itoa) *const slate_reference_itoa = &itoa;

extern long slate_oracle_labs(long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_labs), __typeof__(labs)),
    "stdlib.h:labs declaration differs from oracle");

static __typeof__(labs) *const slate_reference_labs = &labs;

extern struct _ldiv_t slate_oracle_ldiv(long, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ldiv), __typeof__(ldiv)),
    "stdlib.h:ldiv declaration differs from oracle");

static __typeof__(ldiv) *const slate_reference_ldiv = &ldiv;

extern long long slate_oracle_llabs(long long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llabs), __typeof__(llabs)),
    "stdlib.h:llabs declaration differs from oracle");

static __typeof__(llabs) *const slate_reference_llabs = &llabs;

extern struct _lldiv_t slate_oracle_lldiv(long long, long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lldiv), __typeof__(lldiv)),
    "stdlib.h:lldiv declaration differs from oracle");

static __typeof__(lldiv) *const slate_reference_lldiv = &lldiv;

extern char * slate_oracle_ltoa(long, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ltoa), __typeof__(ltoa)),
    "stdlib.h:ltoa declaration differs from oracle");

static __typeof__(ltoa) *const slate_reference_ltoa = &ltoa;

extern int slate_oracle_mblen(const char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mblen), __typeof__(mblen)),
    "stdlib.h:mblen declaration differs from oracle");

static __typeof__(mblen) *const slate_reference_mblen = &mblen;

extern unsigned long long slate_oracle_mbstowcs(unsigned short *, const char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbstowcs), __typeof__(mbstowcs)),
    "stdlib.h:mbstowcs declaration differs from oracle");

static __typeof__(mbstowcs) *const slate_reference_mbstowcs = &mbstowcs;

extern int slate_oracle_mbstowcs_s(unsigned long long *, unsigned short *, unsigned long long, const char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbstowcs_s), __typeof__(mbstowcs_s)),
    "stdlib.h:mbstowcs_s declaration differs from oracle");

static __typeof__(mbstowcs_s) *const slate_reference_mbstowcs_s = &mbstowcs_s;

extern int slate_oracle_mbtowc(unsigned short *, const char *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mbtowc), __typeof__(mbtowc)),
    "stdlib.h:mbtowc declaration differs from oracle");

static __typeof__(mbtowc) *const slate_reference_mbtowc = &mbtowc;

extern int slate_oracle_onexit(*)(void) __attribute__((cdecl)) (int (*)(void) __attribute__((cdecl))) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_onexit), __typeof__(onexit)),
    "stdlib.h:onexit declaration differs from oracle");

static __typeof__(onexit) *const slate_reference_onexit = &onexit;

extern void slate_oracle_perror(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_perror), __typeof__(perror)),
    "stdlib.h:perror declaration differs from oracle");

static __typeof__(perror) *const slate_reference_perror = &perror;

extern int slate_oracle_putenv(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_putenv), __typeof__(putenv)),
    "stdlib.h:putenv declaration differs from oracle");

static __typeof__(putenv) *const slate_reference_putenv = &putenv;

extern void slate_oracle_quick_exit(int) __attribute__((noreturn)) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_quick_exit), __typeof__(quick_exit)),
    "stdlib.h:quick_exit declaration differs from oracle");

static __typeof__(quick_exit) *const slate_reference_quick_exit = &quick_exit;

extern int slate_oracle_rand(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rand), __typeof__(rand)),
    "stdlib.h:rand declaration differs from oracle");

static __typeof__(rand) *const slate_reference_rand = &rand;

extern void slate_oracle_srand(unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_srand), __typeof__(srand)),
    "stdlib.h:srand declaration differs from oracle");

static __typeof__(srand) *const slate_reference_srand = &srand;

extern double slate_oracle_strtod(const char *, char **);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtod), __typeof__(strtod)),
    "stdlib.h:strtod declaration differs from oracle");

static __typeof__(strtod) *const slate_reference_strtod = &strtod;

extern float slate_oracle_strtof(const char *, char **);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtof), __typeof__(strtof)),
    "stdlib.h:strtof declaration differs from oracle");

static __typeof__(strtof) *const slate_reference_strtof = &strtof;

extern long slate_oracle_strtol(const char *, char **, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtol), __typeof__(strtol)),
    "stdlib.h:strtol declaration differs from oracle");

static __typeof__(strtol) *const slate_reference_strtol = &strtol;

extern long double slate_oracle_strtold(const char *, char **);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtold), __typeof__(strtold)),
    "stdlib.h:strtold declaration differs from oracle");

static __typeof__(strtold) *const slate_reference_strtold = &strtold;

extern long long slate_oracle_strtoll(const char *, char **, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoll), __typeof__(strtoll)),
    "stdlib.h:strtoll declaration differs from oracle");

static __typeof__(strtoll) *const slate_reference_strtoll = &strtoll;

extern unsigned long slate_oracle_strtoul(const char *, char **, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoul), __typeof__(strtoul)),
    "stdlib.h:strtoul declaration differs from oracle");

static __typeof__(strtoul) *const slate_reference_strtoul = &strtoul;

extern unsigned long long slate_oracle_strtoull(const char *, char **, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtoull), __typeof__(strtoull)),
    "stdlib.h:strtoull declaration differs from oracle");

static __typeof__(strtoull) *const slate_reference_strtoull = &strtoull;

extern void slate_oracle_swab(char *, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_swab), __typeof__(swab)),
    "stdlib.h:swab declaration differs from oracle");

static __typeof__(swab) *const slate_reference_swab = &swab;

extern int slate_oracle_system(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_system), __typeof__(system)),
    "stdlib.h:system declaration differs from oracle");

static __typeof__(system) *const slate_reference_system = &system;

extern char * slate_oracle_ultoa(unsigned long, char *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ultoa), __typeof__(ultoa)),
    "stdlib.h:ultoa declaration differs from oracle");

static __typeof__(ultoa) *const slate_reference_ultoa = &ultoa;

extern unsigned long long slate_oracle_wcstombs(char *, const unsigned short *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcstombs), __typeof__(wcstombs)),
    "stdlib.h:wcstombs declaration differs from oracle");

static __typeof__(wcstombs) *const slate_reference_wcstombs = &wcstombs;

extern int slate_oracle_wcstombs_s(unsigned long long *, char *, unsigned long long, const unsigned short *, unsigned long long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wcstombs_s), __typeof__(wcstombs_s)),
    "stdlib.h:wcstombs_s declaration differs from oracle");

static __typeof__(wcstombs_s) *const slate_reference_wcstombs_s = &wcstombs_s;

extern int slate_oracle_wctomb(char *, unsigned short) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wctomb), __typeof__(wctomb)),
    "stdlib.h:wctomb declaration differs from oracle");

static __typeof__(wctomb) *const slate_reference_wctomb = &wctomb;

extern int slate_oracle_wctomb_s(int *, char *, unsigned long long, unsigned short) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_wctomb_s), __typeof__(wctomb_s)),
    "stdlib.h:wctomb_s declaration differs from oracle");

static __typeof__(wctomb_s) *const slate_reference_wctomb_s = &wctomb_s;

typedef struct _div_t slate_oracle_typedef_div_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_div_t, div_t), "typedef div_t differs from oracle");

typedef struct _ldiv_t slate_oracle_typedef_ldiv_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ldiv_t, ldiv_t), "typedef ldiv_t differs from oracle");

typedef struct _lldiv_t slate_oracle_typedef_lldiv_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_lldiv_t, lldiv_t), "typedef lldiv_t differs from oracle");

#ifndef EXIT_FAILURE
#error "stdlib.h:EXIT_FAILURE macro is missing from libc-shim"
#endif

#ifndef EXIT_SUCCESS
#error "stdlib.h:EXIT_SUCCESS macro is missing from libc-shim"
#endif

#ifndef MB_CUR_MAX
#error "stdlib.h:MB_CUR_MAX macro is missing from libc-shim"
#endif

#ifndef RAND_MAX
#error "stdlib.h:RAND_MAX macro is missing from libc-shim"
#endif

#ifndef _CALL_REPORTFAULT
#error "stdlib.h:_CALL_REPORTFAULT macro is missing from libc-shim"
#endif

#ifndef _CRT_DOUBLE_DEC
#error "stdlib.h:_CRT_DOUBLE_DEC macro is missing from libc-shim"
#endif

#ifndef _CRT_V12_LEGACY_FUNCTIONALITY
#error "stdlib.h:_CRT_V12_LEGACY_FUNCTIONALITY macro is missing from libc-shim"
#endif

#ifndef _CVTBUFSIZE
#error "stdlib.h:_CVTBUFSIZE macro is missing from libc-shim"
#endif

#ifndef _INC_STDLIB
#error "stdlib.h:_INC_STDLIB macro is missing from libc-shim"
#endif

#ifndef _MAX_DIR
#error "stdlib.h:_MAX_DIR macro is missing from libc-shim"
#endif

#ifndef _MAX_DRIVE
#error "stdlib.h:_MAX_DRIVE macro is missing from libc-shim"
#endif

#ifndef _MAX_ENV
#error "stdlib.h:_MAX_ENV macro is missing from libc-shim"
#endif

#ifndef _MAX_EXT
#error "stdlib.h:_MAX_EXT macro is missing from libc-shim"
#endif

#ifndef _MAX_FNAME
#error "stdlib.h:_MAX_FNAME macro is missing from libc-shim"
#endif

#ifndef _MAX_PATH
#error "stdlib.h:_MAX_PATH macro is missing from libc-shim"
#endif

#ifndef _OUT_TO_DEFAULT
#error "stdlib.h:_OUT_TO_DEFAULT macro is missing from libc-shim"
#endif

#ifndef _OUT_TO_MSGBOX
#error "stdlib.h:_OUT_TO_MSGBOX macro is missing from libc-shim"
#endif

#ifndef _OUT_TO_STDERR
#error "stdlib.h:_OUT_TO_STDERR macro is missing from libc-shim"
#endif

#ifndef _PTR_LD
#error "stdlib.h:_PTR_LD macro is missing from libc-shim"
#endif

#ifndef _REPORT_ERRMODE
#error "stdlib.h:_REPORT_ERRMODE macro is missing from libc-shim"
#endif

#ifndef _WRITE_ABORT_MSG
#error "stdlib.h:_WRITE_ABORT_MSG macro is missing from libc-shim"
#endif

#ifndef __argc
#error "stdlib.h:__argc macro is missing from libc-shim"
#endif

#ifndef __argv
#error "stdlib.h:__argv macro is missing from libc-shim"
#endif

#ifndef __max
#error "stdlib.h:__max macro is missing from libc-shim"
#endif

#ifndef __mb_cur_max
#error "stdlib.h:__mb_cur_max macro is missing from libc-shim"
#endif

#ifndef __min
#error "stdlib.h:__min macro is missing from libc-shim"
#endif

#ifndef __wargv
#error "stdlib.h:__wargv macro is missing from libc-shim"
#endif

#ifndef _countof
#error "stdlib.h:_countof macro is missing from libc-shim"
#endif

#ifndef _doserrno
#error "stdlib.h:_doserrno macro is missing from libc-shim"
#endif

#ifndef _environ
#error "stdlib.h:_environ macro is missing from libc-shim"
#endif

#ifndef _fmode
#error "stdlib.h:_fmode macro is missing from libc-shim"
#endif

#ifndef _pgmptr
#error "stdlib.h:_pgmptr macro is missing from libc-shim"
#endif

#ifndef _sys_errlist
#error "stdlib.h:_sys_errlist macro is missing from libc-shim"
#endif

#ifndef _sys_nerr
#error "stdlib.h:_sys_nerr macro is missing from libc-shim"
#endif

#ifndef _wenviron
#error "stdlib.h:_wenviron macro is missing from libc-shim"
#endif

#ifndef _wpgmptr
#error "stdlib.h:_wpgmptr macro is missing from libc-shim"
#endif

#ifndef environ
#error "stdlib.h:environ macro is missing from libc-shim"
#endif

#ifndef errno
#error "stdlib.h:errno macro is missing from libc-shim"
#endif

#ifndef max
#error "stdlib.h:max macro is missing from libc-shim"
#endif

#ifndef min
#error "stdlib.h:min macro is missing from libc-shim"
#endif

#ifndef onexit_t
#error "stdlib.h:onexit_t macro is missing from libc-shim"
#endif

#ifndef sys_errlist
#error "stdlib.h:sys_errlist macro is missing from libc-shim"
#endif

#ifndef sys_nerr
#error "stdlib.h:sys_nerr macro is missing from libc-shim"
#endif

int main(void) { return 0; }
