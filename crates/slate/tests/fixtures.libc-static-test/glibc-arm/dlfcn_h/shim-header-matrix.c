#include <dlfcn.h>

typedef long slate_oracle_typedef_Lmid_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_Lmid_t, Lmid_t), "typedef Lmid_t differs from oracle");

#ifndef DLFO_FLAG_SFRAME
#error "dlfcn.h:DLFO_FLAG_SFRAME macro is missing from libc-shim"
#endif

#ifndef DL_CALL_FCT
#error "dlfcn.h:DL_CALL_FCT macro is missing from libc-shim"
#endif

#ifndef LM_ID_BASE
#error "dlfcn.h:LM_ID_BASE macro is missing from libc-shim"
#endif

#ifndef LM_ID_NEWLM
#error "dlfcn.h:LM_ID_NEWLM macro is missing from libc-shim"
#endif

#ifndef RTLD_BINDING_MASK
#error "dlfcn.h:RTLD_BINDING_MASK macro is missing from libc-shim"
#endif

#ifndef RTLD_DEEPBIND
#error "dlfcn.h:RTLD_DEEPBIND macro is missing from libc-shim"
#endif

#ifndef RTLD_DEFAULT
#error "dlfcn.h:RTLD_DEFAULT macro is missing from libc-shim"
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
