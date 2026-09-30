#include <crypt.h>

extern char * slate_oracle_crypt(const char *, const char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_crypt), __typeof__(crypt)),
    "crypt.h:crypt declaration differs from oracle");

static __typeof__(crypt) *const slate_reference_crypt = &crypt;

#ifndef CRYPT_CHECKSALT_AVAILABLE
#error "crypt.h:CRYPT_CHECKSALT_AVAILABLE macro is missing from libc-shim"
#endif

#ifndef CRYPT_DATA_INTERNAL_SIZE
#error "crypt.h:CRYPT_DATA_INTERNAL_SIZE macro is missing from libc-shim"
#endif

#ifndef CRYPT_DATA_RESERVED_SIZE
#error "crypt.h:CRYPT_DATA_RESERVED_SIZE macro is missing from libc-shim"
#endif

#ifndef CRYPT_GENSALT_IMPLEMENTS_AUTO_ENTROPY
#error "crypt.h:CRYPT_GENSALT_IMPLEMENTS_AUTO_ENTROPY macro is missing from libc-shim"
#endif

#ifndef CRYPT_GENSALT_IMPLEMENTS_DEFAULT_PREFIX
#error "crypt.h:CRYPT_GENSALT_IMPLEMENTS_DEFAULT_PREFIX macro is missing from libc-shim"
#endif

#ifndef CRYPT_GENSALT_OUTPUT_SIZE
#error "crypt.h:CRYPT_GENSALT_OUTPUT_SIZE macro is missing from libc-shim"
#endif

#ifndef CRYPT_MAX_PASSPHRASE_SIZE
#error "crypt.h:CRYPT_MAX_PASSPHRASE_SIZE macro is missing from libc-shim"
#endif

#ifndef CRYPT_OUTPUT_SIZE
#error "crypt.h:CRYPT_OUTPUT_SIZE macro is missing from libc-shim"
#endif

#ifndef CRYPT_PREFERRED_METHOD_AVAILABLE
#error "crypt.h:CRYPT_PREFERRED_METHOD_AVAILABLE macro is missing from libc-shim"
#endif

#ifndef CRYPT_SALT_INVALID
#error "crypt.h:CRYPT_SALT_INVALID macro is missing from libc-shim"
#endif

#ifndef CRYPT_SALT_METHOD_DISABLED
#error "crypt.h:CRYPT_SALT_METHOD_DISABLED macro is missing from libc-shim"
#endif

#ifndef CRYPT_SALT_METHOD_LEGACY
#error "crypt.h:CRYPT_SALT_METHOD_LEGACY macro is missing from libc-shim"
#endif

#ifndef CRYPT_SALT_OK
#error "crypt.h:CRYPT_SALT_OK macro is missing from libc-shim"
#endif

#ifndef CRYPT_SALT_TOO_CHEAP
#error "crypt.h:CRYPT_SALT_TOO_CHEAP macro is missing from libc-shim"
#endif

#ifndef XCRYPT_VERSION_MAJOR
#error "crypt.h:XCRYPT_VERSION_MAJOR macro is missing from libc-shim"
#endif

#ifndef XCRYPT_VERSION_MINOR
#error "crypt.h:XCRYPT_VERSION_MINOR macro is missing from libc-shim"
#endif

#ifndef XCRYPT_VERSION_NUM
#error "crypt.h:XCRYPT_VERSION_NUM macro is missing from libc-shim"
#endif

#ifndef XCRYPT_VERSION_STR
#error "crypt.h:XCRYPT_VERSION_STR macro is missing from libc-shim"
#endif

int main(void) { return 0; }
