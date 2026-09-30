#include <malloc.h>

extern void * slate_oracle_malloc(__SIZE_TYPE__);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_malloc), __typeof__(malloc)),
    "malloc.h:malloc declaration differs from oracle");

static __typeof__(malloc) *const slate_reference_malloc = &malloc;

int main(void) { return 0; }
