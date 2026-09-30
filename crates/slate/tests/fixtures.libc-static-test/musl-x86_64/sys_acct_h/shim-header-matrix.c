#include <sys/acct.h>

typedef unsigned short slate_oracle_typedef_comp_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_comp_t, comp_t), "typedef comp_t differs from oracle");

#ifndef ACCT_BYTEORDER
#error "sys/acct.h:ACCT_BYTEORDER macro is missing from libc-shim"
#endif

#ifndef ACCT_COMM
#error "sys/acct.h:ACCT_COMM macro is missing from libc-shim"
#endif

#ifndef ACORE
#error "sys/acct.h:ACORE macro is missing from libc-shim"
#endif

#ifndef AFORK
#error "sys/acct.h:AFORK macro is missing from libc-shim"
#endif

#ifndef AHZ
#error "sys/acct.h:AHZ macro is missing from libc-shim"
#endif

#ifndef ASU
#error "sys/acct.h:ASU macro is missing from libc-shim"
#endif

#ifndef AXSIG
#error "sys/acct.h:AXSIG macro is missing from libc-shim"
#endif

int main(void) { return 0; }
