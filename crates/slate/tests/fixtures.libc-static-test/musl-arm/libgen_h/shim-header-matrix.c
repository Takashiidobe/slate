#include <libgen.h>

extern char * slate_oracle_dirname(char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_dirname), __typeof__(dirname)),
    "libgen.h:dirname declaration differs from oracle");

static __typeof__(dirname) *const slate_reference_dirname = &dirname;

int main(void) { return 0; }
