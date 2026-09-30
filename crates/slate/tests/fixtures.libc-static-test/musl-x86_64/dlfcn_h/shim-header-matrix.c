#include <dlfcn.h>

extern int slate_oracle_dlclose(void *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_dlclose), __typeof__(dlclose)),
    "dlfcn.h:dlclose declaration differs from oracle");

static __typeof__(dlclose) *const slate_reference_dlclose = &dlclose;

#ifndef RTLD_DEFAULT
#error "dlfcn.h:RTLD_DEFAULT macro is missing from libc-shim"
#endif

#ifndef RTLD_DI_LINKMAP
#error "dlfcn.h:RTLD_DI_LINKMAP macro is missing from libc-shim"
#endif

#ifndef RTLD_GLOBAL
#error "dlfcn.h:RTLD_GLOBAL macro is missing from libc-shim"
#endif

#ifndef RTLD_LAZY
#error "dlfcn.h:RTLD_LAZY macro is missing from libc-shim"
#endif

#ifndef RTLD_LOCAL
#error "dlfcn.h:RTLD_LOCAL macro is missing from libc-shim"
#endif

#ifndef RTLD_NEXT
#error "dlfcn.h:RTLD_NEXT macro is missing from libc-shim"
#endif

#ifndef RTLD_NODELETE
#error "dlfcn.h:RTLD_NODELETE macro is missing from libc-shim"
#endif

#ifndef RTLD_NOLOAD
#error "dlfcn.h:RTLD_NOLOAD macro is missing from libc-shim"
#endif

#ifndef RTLD_NOW
#error "dlfcn.h:RTLD_NOW macro is missing from libc-shim"
#endif

int main(void) { return 0; }
