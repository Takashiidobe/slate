#include <libintl.h>

extern char * slate_oracle_gettext(const char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gettext), __typeof__(gettext)),
    "libintl.h:gettext declaration differs from oracle");

static __typeof__(gettext) *const slate_reference_gettext = &gettext;

int main(void) { return 0; }
