#include <assert.h>

extern void slate_oracle__wassert(const unsigned short *, const unsigned short *, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__wassert), __typeof__(_wassert)),
    "assert.h:_wassert declaration differs from oracle");

static __typeof__(_wassert) *const slate_reference__wassert = &_wassert;

#ifndef assert
#error "assert.h:assert macro is missing from libc-shim"
#endif

int main(void) { return 0; }
