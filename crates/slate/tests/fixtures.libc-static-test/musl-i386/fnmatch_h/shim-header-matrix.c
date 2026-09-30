#include <fnmatch.h>

extern int slate_oracle_fnmatch(const char *, const char *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fnmatch), __typeof__(fnmatch)),
    "fnmatch.h:fnmatch declaration differs from oracle");

static __typeof__(fnmatch) *const slate_reference_fnmatch = &fnmatch;

#ifndef FNM_CASEFOLD
#error "fnmatch.h:FNM_CASEFOLD macro is missing from libc-shim"
#endif

#ifndef FNM_FILE_NAME
#error "fnmatch.h:FNM_FILE_NAME macro is missing from libc-shim"
#endif

#ifndef FNM_LEADING_DIR
#error "fnmatch.h:FNM_LEADING_DIR macro is missing from libc-shim"
#endif

#ifndef FNM_NOESCAPE
#error "fnmatch.h:FNM_NOESCAPE macro is missing from libc-shim"
#endif

#ifndef FNM_NOMATCH
#error "fnmatch.h:FNM_NOMATCH macro is missing from libc-shim"
#endif

#ifndef FNM_NOSYS
#error "fnmatch.h:FNM_NOSYS macro is missing from libc-shim"
#endif

#ifndef FNM_PATHNAME
#error "fnmatch.h:FNM_PATHNAME macro is missing from libc-shim"
#endif

#ifndef FNM_PERIOD
#error "fnmatch.h:FNM_PERIOD macro is missing from libc-shim"
#endif

int main(void) { return 0; }
