#include <sys/types.h>

typedef unsigned int slate_oracle_typedef_dev_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_dev_t, dev_t), "typedef dev_t differs from oracle");

typedef unsigned short slate_oracle_typedef_ino_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ino_t, ino_t), "typedef ino_t differs from oracle");

typedef long slate_oracle_typedef_off_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_off_t, off_t), "typedef off_t differs from oracle");

#ifndef _DEV_T_DEFINED
#error "sys/types.h:_DEV_T_DEFINED macro is missing from libc-shim"
#endif

#ifndef _INO_T_DEFINED
#error "sys/types.h:_INO_T_DEFINED macro is missing from libc-shim"
#endif

#ifndef _OFF_T_DEFINED
#error "sys/types.h:_OFF_T_DEFINED macro is missing from libc-shim"
#endif

int main(void) { return 0; }
