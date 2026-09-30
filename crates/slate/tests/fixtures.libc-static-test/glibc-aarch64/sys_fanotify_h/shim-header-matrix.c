#include <sys/fanotify.h>

extern int slate_oracle_fanotify_init(unsigned int, unsigned int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fanotify_init), __typeof__(fanotify_init)),
    "sys/fanotify.h:fanotify_init declaration differs from oracle");

static __typeof__(fanotify_init) *const slate_reference_fanotify_init = &fanotify_init;

int main(void) { return 0; }
