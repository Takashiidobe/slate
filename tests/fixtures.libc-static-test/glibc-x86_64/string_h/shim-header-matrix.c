#include <string.h>

extern int slate_oracle___memcmpeq(const void *, const void *, unsigned long);
extern void * slate_oracle___mempcpy(void *restrict, const void *restrict, unsigned long);
extern char * slate_oracle___stpcpy(char *restrict, const char *restrict);
extern char * slate_oracle___stpncpy(char *restrict, const char *restrict, unsigned long);
extern char * slate_oracle___strtok_r(char *restrict, const char *restrict, char **restrict);
extern char * slate_oracle_basename(const char *);
extern int slate_oracle_bcmp(const void *, const void *, __size_t);
extern void slate_oracle_bcopy(const void *, void *, __size_t);
extern void slate_oracle_bzero(void *, __size_t);
extern void slate_oracle_explicit_bzero(void *, unsigned long);
extern int slate_oracle_ffs(int);
extern int slate_oracle_ffsl(long);
extern int slate_oracle_ffsll(long long);
extern char * slate_oracle_index(const char *, int);
extern void * slate_oracle_memccpy(void *, const void *, int, __size_t);
extern void * slate_oracle_memchr(const void *, int, __size_t);
extern int slate_oracle_memcmp(const void *, const void *, __size_t);
extern void * slate_oracle_memcpy(void *, const void *, __size_t);
extern void * slate_oracle_memfrob(void *, unsigned long);
extern void * slate_oracle_memmem(const void *, unsigned long, const void *, unsigned long);
extern void * slate_oracle_memmove(void *, const void *, __size_t);
extern void * slate_oracle_mempcpy(void *, const void *, __size_t);
extern void * slate_oracle_memrchr(const void *, int, unsigned long);
extern void * slate_oracle_memset(void *, int, __size_t);
extern void * slate_oracle_memset_explicit(void *, int, unsigned long);
extern void * slate_oracle_rawmemchr(const void *, int);
extern char * slate_oracle_rindex(const char *, int);
extern const char * slate_oracle_sigabbrev_np(int);
extern const char * slate_oracle_sigdescr_np(int);
extern char * slate_oracle_stpcpy(char *, const char *);
extern char * slate_oracle_stpncpy(char *, const char *, __size_t);
extern int slate_oracle_strcasecmp(const char *, const char *);
extern int slate_oracle_strcasecmp_l(const char *, const char *, struct __locale_struct *);
extern char * slate_oracle_strcasestr(const char *, const char *);
extern char * slate_oracle_strcat(char *, const char *);
extern char * slate_oracle_strchr(const char *, int);
extern char * slate_oracle_strchrnul(const char *, int);
extern int slate_oracle_strcmp(const char *, const char *);
extern int slate_oracle_strcoll(const char *, const char *);
extern int slate_oracle_strcoll_l(const char *, const char *, struct __locale_struct *);
extern char * slate_oracle_strcpy(char *, const char *);
extern __size_t slate_oracle_strcspn(const char *, const char *);
extern char * slate_oracle_strdup(const char *);
extern char * slate_oracle_strerror(int);
extern char * slate_oracle_strerror_l(int, struct __locale_struct *);
extern char * slate_oracle_strerror_r(int, char *, unsigned long);
extern const char * slate_oracle_strerrordesc_np(int);
extern const char * slate_oracle_strerrorname_np(int);
extern char * slate_oracle_strfry(char *);
extern __size_t slate_oracle_strlcat(char *, const char *, __size_t);
extern __size_t slate_oracle_strlcpy(char *, const char *, __size_t);
extern __size_t slate_oracle_strlen(const char *);
extern int slate_oracle_strncasecmp(const char *, const char *, __size_t);
extern int slate_oracle_strncasecmp_l(const char *, const char *, unsigned long, struct __locale_struct *);
extern char * slate_oracle_strncat(char *, const char *, __size_t);
extern int slate_oracle_strncmp(const char *, const char *, __size_t);
extern char * slate_oracle_strncpy(char *, const char *, __size_t);
extern char * slate_oracle_strndup(const char *, __size_t);
extern unsigned long slate_oracle_strnlen(const char *, unsigned long);
extern char * slate_oracle_strpbrk(const char *, const char *);
extern char * slate_oracle_strrchr(const char *, int);
extern char * slate_oracle_strsep(char **restrict, const char *restrict);
extern char * slate_oracle_strsignal(int);
extern __size_t slate_oracle_strspn(const char *, const char *);
extern char * slate_oracle_strstr(const char *, const char *);
extern char * slate_oracle_strtok(char *, const char *);
extern char * slate_oracle_strtok_r(char *restrict, const char *restrict, char **restrict);
extern int slate_oracle_strverscmp(const char *, const char *);
extern __size_t slate_oracle_strxfrm(char *, const char *, __size_t);
extern unsigned long slate_oracle_strxfrm_l(char *, const char *, unsigned long, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___memcmpeq), __typeof__(__memcmpeq)),
    "string.h:__memcmpeq declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___mempcpy), __typeof__(__mempcpy)),
    "string.h:__mempcpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___stpcpy), __typeof__(__stpcpy)),
    "string.h:__stpcpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___stpncpy), __typeof__(__stpncpy)),
    "string.h:__stpncpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle___strtok_r), __typeof__(__strtok_r)),
    "string.h:__strtok_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_basename), __typeof__(basename)),
    "string.h:basename declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_bcmp), __typeof__(bcmp)),
    "string.h:bcmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_bcopy), __typeof__(bcopy)),
    "string.h:bcopy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_bzero), __typeof__(bzero)),
    "string.h:bzero declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_explicit_bzero), __typeof__(explicit_bzero)),
    "string.h:explicit_bzero declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ffs), __typeof__(ffs)),
    "string.h:ffs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ffsl), __typeof__(ffsl)),
    "string.h:ffsl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ffsll), __typeof__(ffsll)),
    "string.h:ffsll declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_index), __typeof__(index)),
    "string.h:index declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memccpy), __typeof__(memccpy)),
    "string.h:memccpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memchr), __typeof__(memchr)),
    "string.h:memchr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memcmp), __typeof__(memcmp)),
    "string.h:memcmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memcpy), __typeof__(memcpy)),
    "string.h:memcpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memfrob), __typeof__(memfrob)),
    "string.h:memfrob declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memmem), __typeof__(memmem)),
    "string.h:memmem declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memmove), __typeof__(memmove)),
    "string.h:memmove declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_mempcpy), __typeof__(mempcpy)),
    "string.h:mempcpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memrchr), __typeof__(memrchr)),
    "string.h:memrchr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memset), __typeof__(memset)),
    "string.h:memset declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_memset_explicit), __typeof__(memset_explicit)),
    "string.h:memset_explicit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rawmemchr), __typeof__(rawmemchr)),
    "string.h:rawmemchr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rindex), __typeof__(rindex)),
    "string.h:rindex declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sigabbrev_np), __typeof__(sigabbrev_np)),
    "string.h:sigabbrev_np declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sigdescr_np), __typeof__(sigdescr_np)),
    "string.h:sigdescr_np declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_stpcpy), __typeof__(stpcpy)),
    "string.h:stpcpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_stpncpy), __typeof__(stpncpy)),
    "string.h:stpncpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcasecmp), __typeof__(strcasecmp)),
    "string.h:strcasecmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcasecmp_l), __typeof__(strcasecmp_l)),
    "string.h:strcasecmp_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcasestr), __typeof__(strcasestr)),
    "string.h:strcasestr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcat), __typeof__(strcat)),
    "string.h:strcat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strchr), __typeof__(strchr)),
    "string.h:strchr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strchrnul), __typeof__(strchrnul)),
    "string.h:strchrnul declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcmp), __typeof__(strcmp)),
    "string.h:strcmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcoll), __typeof__(strcoll)),
    "string.h:strcoll declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcoll_l), __typeof__(strcoll_l)),
    "string.h:strcoll_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcpy), __typeof__(strcpy)),
    "string.h:strcpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcspn), __typeof__(strcspn)),
    "string.h:strcspn declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strdup), __typeof__(strdup)),
    "string.h:strdup declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strerror), __typeof__(strerror)),
    "string.h:strerror declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strerror_l), __typeof__(strerror_l)),
    "string.h:strerror_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strerror_r), __typeof__(strerror_r)),
    "string.h:strerror_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strerrordesc_np), __typeof__(strerrordesc_np)),
    "string.h:strerrordesc_np declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strerrorname_np), __typeof__(strerrorname_np)),
    "string.h:strerrorname_np declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strfry), __typeof__(strfry)),
    "string.h:strfry declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strlcat), __typeof__(strlcat)),
    "string.h:strlcat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strlcpy), __typeof__(strlcpy)),
    "string.h:strlcpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strlen), __typeof__(strlen)),
    "string.h:strlen declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strncasecmp), __typeof__(strncasecmp)),
    "string.h:strncasecmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strncasecmp_l), __typeof__(strncasecmp_l)),
    "string.h:strncasecmp_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strncat), __typeof__(strncat)),
    "string.h:strncat declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strncmp), __typeof__(strncmp)),
    "string.h:strncmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strncpy), __typeof__(strncpy)),
    "string.h:strncpy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strndup), __typeof__(strndup)),
    "string.h:strndup declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strnlen), __typeof__(strnlen)),
    "string.h:strnlen declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strpbrk), __typeof__(strpbrk)),
    "string.h:strpbrk declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strrchr), __typeof__(strrchr)),
    "string.h:strrchr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strsep), __typeof__(strsep)),
    "string.h:strsep declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strsignal), __typeof__(strsignal)),
    "string.h:strsignal declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strspn), __typeof__(strspn)),
    "string.h:strspn declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strstr), __typeof__(strstr)),
    "string.h:strstr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtok), __typeof__(strtok)),
    "string.h:strtok declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strtok_r), __typeof__(strtok_r)),
    "string.h:strtok_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strverscmp), __typeof__(strverscmp)),
    "string.h:strverscmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strxfrm), __typeof__(strxfrm)),
    "string.h:strxfrm declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strxfrm_l), __typeof__(strxfrm_l)),
    "string.h:strxfrm_l declaration differs from oracle");

static __typeof__(__memcmpeq) *const slate_reference___memcmpeq = &__memcmpeq;
static __typeof__(__mempcpy) *const slate_reference___mempcpy = &__mempcpy;
static __typeof__(__stpcpy) *const slate_reference___stpcpy = &__stpcpy;
static __typeof__(__stpncpy) *const slate_reference___stpncpy = &__stpncpy;
static __typeof__(__strtok_r) *const slate_reference___strtok_r = &__strtok_r;
static __typeof__(basename) *const slate_reference_basename = &basename;
static __typeof__(bcmp) *const slate_reference_bcmp = &bcmp;
static __typeof__(bcopy) *const slate_reference_bcopy = &bcopy;
static __typeof__(bzero) *const slate_reference_bzero = &bzero;
static __typeof__(explicit_bzero) *const slate_reference_explicit_bzero = &explicit_bzero;
static __typeof__(ffs) *const slate_reference_ffs = &ffs;
static __typeof__(ffsl) *const slate_reference_ffsl = &ffsl;
static __typeof__(ffsll) *const slate_reference_ffsll = &ffsll;
static __typeof__(index) *const slate_reference_index = &index;
static __typeof__(memccpy) *const slate_reference_memccpy = &memccpy;
static __typeof__(memchr) *const slate_reference_memchr = &memchr;
static __typeof__(memcmp) *const slate_reference_memcmp = &memcmp;
static __typeof__(memcpy) *const slate_reference_memcpy = &memcpy;
static __typeof__(memfrob) *const slate_reference_memfrob = &memfrob;
static __typeof__(memmem) *const slate_reference_memmem = &memmem;
static __typeof__(memmove) *const slate_reference_memmove = &memmove;
static __typeof__(mempcpy) *const slate_reference_mempcpy = &mempcpy;
static __typeof__(memrchr) *const slate_reference_memrchr = &memrchr;
static __typeof__(memset) *const slate_reference_memset = &memset;
static __typeof__(memset_explicit) *const slate_reference_memset_explicit = &memset_explicit;
static __typeof__(rawmemchr) *const slate_reference_rawmemchr = &rawmemchr;
static __typeof__(rindex) *const slate_reference_rindex = &rindex;
static __typeof__(sigabbrev_np) *const slate_reference_sigabbrev_np = &sigabbrev_np;
static __typeof__(sigdescr_np) *const slate_reference_sigdescr_np = &sigdescr_np;
static __typeof__(stpcpy) *const slate_reference_stpcpy = &stpcpy;
static __typeof__(stpncpy) *const slate_reference_stpncpy = &stpncpy;
static __typeof__(strcasecmp) *const slate_reference_strcasecmp = &strcasecmp;
static __typeof__(strcasecmp_l) *const slate_reference_strcasecmp_l = &strcasecmp_l;
static __typeof__(strcasestr) *const slate_reference_strcasestr = &strcasestr;
static __typeof__(strcat) *const slate_reference_strcat = &strcat;
static __typeof__(strchr) *const slate_reference_strchr = &strchr;
static __typeof__(strchrnul) *const slate_reference_strchrnul = &strchrnul;
static __typeof__(strcmp) *const slate_reference_strcmp = &strcmp;
static __typeof__(strcoll) *const slate_reference_strcoll = &strcoll;
static __typeof__(strcoll_l) *const slate_reference_strcoll_l = &strcoll_l;
static __typeof__(strcpy) *const slate_reference_strcpy = &strcpy;
static __typeof__(strcspn) *const slate_reference_strcspn = &strcspn;
static __typeof__(strdup) *const slate_reference_strdup = &strdup;
static __typeof__(strerror) *const slate_reference_strerror = &strerror;
static __typeof__(strerror_l) *const slate_reference_strerror_l = &strerror_l;
static __typeof__(strerror_r) *const slate_reference_strerror_r = &strerror_r;
static __typeof__(strerrordesc_np) *const slate_reference_strerrordesc_np = &strerrordesc_np;
static __typeof__(strerrorname_np) *const slate_reference_strerrorname_np = &strerrorname_np;
static __typeof__(strfry) *const slate_reference_strfry = &strfry;
static __typeof__(strlcat) *const slate_reference_strlcat = &strlcat;
static __typeof__(strlcpy) *const slate_reference_strlcpy = &strlcpy;
static __typeof__(strlen) *const slate_reference_strlen = &strlen;
static __typeof__(strncasecmp) *const slate_reference_strncasecmp = &strncasecmp;
static __typeof__(strncasecmp_l) *const slate_reference_strncasecmp_l = &strncasecmp_l;
static __typeof__(strncat) *const slate_reference_strncat = &strncat;
static __typeof__(strncmp) *const slate_reference_strncmp = &strncmp;
static __typeof__(strncpy) *const slate_reference_strncpy = &strncpy;
static __typeof__(strndup) *const slate_reference_strndup = &strndup;
static __typeof__(strnlen) *const slate_reference_strnlen = &strnlen;
static __typeof__(strpbrk) *const slate_reference_strpbrk = &strpbrk;
static __typeof__(strrchr) *const slate_reference_strrchr = &strrchr;
static __typeof__(strsep) *const slate_reference_strsep = &strsep;
static __typeof__(strsignal) *const slate_reference_strsignal = &strsignal;
static __typeof__(strspn) *const slate_reference_strspn = &strspn;
static __typeof__(strstr) *const slate_reference_strstr = &strstr;
static __typeof__(strtok) *const slate_reference_strtok = &strtok;
static __typeof__(strtok_r) *const slate_reference_strtok_r = &strtok_r;
static __typeof__(strverscmp) *const slate_reference_strverscmp = &strverscmp;
static __typeof__(strxfrm) *const slate_reference_strxfrm = &strxfrm;
static __typeof__(strxfrm_l) *const slate_reference_strxfrm_l = &strxfrm_l;

typedef struct __locale_struct * slate_oracle_typedef_locale_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_locale_t, locale_t), "typedef locale_t differs from oracle");

typedef unsigned long slate_oracle_typedef_size_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_size_t, size_t), "typedef size_t differs from oracle");

typedef const unsigned short * slate_oracle_struct___locale_struct___ctype_b;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __locale_struct *)0)->__ctype_b), slate_oracle_struct___locale_struct___ctype_b), "struct __locale_struct.__ctype_b field type differs from oracle");

typedef const int * slate_oracle_struct___locale_struct___ctype_tolower;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __locale_struct *)0)->__ctype_tolower), slate_oracle_struct___locale_struct___ctype_tolower), "struct __locale_struct.__ctype_tolower field type differs from oracle");

typedef const int * slate_oracle_struct___locale_struct___ctype_toupper;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct __locale_struct *)0)->__ctype_toupper), slate_oracle_struct___locale_struct___ctype_toupper), "struct __locale_struct.__ctype_toupper field type differs from oracle");

#ifndef NULL
#error "string.h:NULL macro is missing from libc-shim"
#endif

#ifndef _ATFILE_SOURCE
#error "string.h:_ATFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES_LOCALE_T_H
#error "string.h:_BITS_TYPES_LOCALE_T_H macro is missing from libc-shim"
#endif

#ifndef _BITS_TYPES___LOCALE_T_H
#error "string.h:_BITS_TYPES___LOCALE_T_H macro is missing from libc-shim"
#endif

#ifndef _DEFAULT_SOURCE
#error "string.h:_DEFAULT_SOURCE macro is missing from libc-shim"
#endif

#ifndef _DYNAMIC_STACK_SIZE_SOURCE
#error "string.h:_DYNAMIC_STACK_SIZE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _FEATURES_H
#error "string.h:_FEATURES_H macro is missing from libc-shim"
#endif

#ifndef _ISOC11_SOURCE
#error "string.h:_ISOC11_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC23_SOURCE
#error "string.h:_ISOC23_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC2Y_SOURCE
#error "string.h:_ISOC2Y_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC95_SOURCE
#error "string.h:_ISOC95_SOURCE macro is missing from libc-shim"
#endif

#ifndef _ISOC99_SOURCE
#error "string.h:_ISOC99_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE64_SOURCE
#error "string.h:_LARGEFILE64_SOURCE macro is missing from libc-shim"
#endif

#ifndef _LARGEFILE_SOURCE
#error "string.h:_LARGEFILE_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_C_SOURCE
#error "string.h:_POSIX_C_SOURCE macro is missing from libc-shim"
#endif

#ifndef _POSIX_SOURCE
#error "string.h:_POSIX_SOURCE macro is missing from libc-shim"
#endif

#ifndef _SIZE_T
#error "string.h:_SIZE_T macro is missing from libc-shim"
#endif

#ifndef _STDC_PREDEF_H
#error "string.h:_STDC_PREDEF_H macro is missing from libc-shim"
#endif

#ifndef _STRINGS_H
#error "string.h:_STRINGS_H macro is missing from libc-shim"
#endif

#ifndef _STRING_H
#error "string.h:_STRING_H macro is missing from libc-shim"
#endif

#ifndef _SYS_CDEFS_H
#error "string.h:_SYS_CDEFS_H macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE
#error "string.h:_XOPEN_SOURCE macro is missing from libc-shim"
#endif

#ifndef _XOPEN_SOURCE_EXTENDED
#error "string.h:_XOPEN_SOURCE_EXTENDED macro is missing from libc-shim"
#endif

#ifndef memchr
#error "string.h:memchr macro is missing from libc-shim"
#endif

#ifndef strchr
#error "string.h:strchr macro is missing from libc-shim"
#endif

#ifndef strdupa
#error "string.h:strdupa macro is missing from libc-shim"
#endif

#ifndef strndupa
#error "string.h:strndupa macro is missing from libc-shim"
#endif

#ifndef strpbrk
#error "string.h:strpbrk macro is missing from libc-shim"
#endif

#ifndef strrchr
#error "string.h:strrchr macro is missing from libc-shim"
#endif

#ifndef strstr
#error "string.h:strstr macro is missing from libc-shim"
#endif

int main(void) { return 0; }
