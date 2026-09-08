#include <strings.h>

extern int slate_oracle_bcmp(const void *, const void *, __size_t);
extern void slate_oracle_bcopy(const void *, void *, __size_t);
extern void slate_oracle_bzero(void *, __size_t);
extern int slate_oracle_ffs(int);
extern int slate_oracle_ffsl(long);
extern int slate_oracle_ffsll(long long);
extern char * slate_oracle_index(const char *, int);
extern char * slate_oracle_rindex(const char *, int);
extern int slate_oracle_strcasecmp(const char *, const char *);
extern int slate_oracle_strcasecmp_l(const char *, const char *, struct __locale_struct *);
extern int slate_oracle_strncasecmp(const char *, const char *, __size_t);
extern int slate_oracle_strncasecmp_l(const char *, const char *, unsigned int, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_bcmp), __typeof__(bcmp)),
    "strings.h:bcmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_bcopy), __typeof__(bcopy)),
    "strings.h:bcopy declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_bzero), __typeof__(bzero)),
    "strings.h:bzero declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ffs), __typeof__(ffs)),
    "strings.h:ffs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ffsl), __typeof__(ffsl)),
    "strings.h:ffsl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ffsll), __typeof__(ffsll)),
    "strings.h:ffsll declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_index), __typeof__(index)),
    "strings.h:index declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rindex), __typeof__(rindex)),
    "strings.h:rindex declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcasecmp), __typeof__(strcasecmp)),
    "strings.h:strcasecmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcasecmp_l), __typeof__(strcasecmp_l)),
    "strings.h:strcasecmp_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strncasecmp), __typeof__(strncasecmp)),
    "strings.h:strncasecmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strncasecmp_l), __typeof__(strncasecmp_l)),
    "strings.h:strncasecmp_l declaration differs from oracle");

static __typeof__(bcmp) *const slate_reference_bcmp = &bcmp;
static __typeof__(bcopy) *const slate_reference_bcopy = &bcopy;
static __typeof__(bzero) *const slate_reference_bzero = &bzero;
static __typeof__(ffs) *const slate_reference_ffs = &ffs;
static __typeof__(ffsl) *const slate_reference_ffsl = &ffsl;
static __typeof__(ffsll) *const slate_reference_ffsll = &ffsll;
static __typeof__(index) *const slate_reference_index = &index;
static __typeof__(rindex) *const slate_reference_rindex = &rindex;
static __typeof__(strcasecmp) *const slate_reference_strcasecmp = &strcasecmp;
static __typeof__(strcasecmp_l) *const slate_reference_strcasecmp_l = &strcasecmp_l;
static __typeof__(strncasecmp) *const slate_reference_strncasecmp = &strncasecmp;
static __typeof__(strncasecmp_l) *const slate_reference_strncasecmp_l = &strncasecmp_l;

typedef struct __locale_struct * slate_oracle_typedef_locale_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_locale_t, locale_t), "typedef locale_t differs from oracle");

int main(void) { return 0; }
