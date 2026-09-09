#include <strings.h>

extern int slate_oracle_bcmp(const void *, const void *, __size_t);
extern int slate_oracle_strcasecmp_l(const char *, const char *, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_bcmp), __typeof__(bcmp)),
    "strings.h:bcmp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_strcasecmp_l), __typeof__(strcasecmp_l)),
    "strings.h:strcasecmp_l declaration differs from oracle");

static __typeof__(bcmp) *const slate_reference_bcmp = &bcmp;
static __typeof__(strcasecmp_l) *const slate_reference_strcasecmp_l = &strcasecmp_l;

typedef struct __locale_struct * slate_oracle_typedef_locale_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_locale_t, locale_t), "typedef locale_t differs from oracle");

int main(void) { return 0; }
