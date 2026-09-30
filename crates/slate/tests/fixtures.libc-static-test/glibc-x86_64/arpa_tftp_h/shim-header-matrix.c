#include <arpa/tftp.h>

_Static_assert(__builtin_offsetof(struct tftphdr, th_opcode) == 0, "struct tftphdr.th_opcode offset differs from oracle");

typedef short slate_oracle_struct_tftphdr_th_opcode;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct tftphdr *)0)->th_opcode), slate_oracle_struct_tftphdr_th_opcode), "struct tftphdr.th_opcode field type differs from oracle");

#ifndef ACK
#error "arpa/tftp.h:ACK macro is missing from libc-shim"
#endif

#ifndef DATA
#error "arpa/tftp.h:DATA macro is missing from libc-shim"
#endif

#ifndef EACCESS
#error "arpa/tftp.h:EACCESS macro is missing from libc-shim"
#endif

#ifndef EBADID
#error "arpa/tftp.h:EBADID macro is missing from libc-shim"
#endif

#ifndef EBADOP
#error "arpa/tftp.h:EBADOP macro is missing from libc-shim"
#endif

#ifndef EEXISTS
#error "arpa/tftp.h:EEXISTS macro is missing from libc-shim"
#endif

#ifndef ENOSPACE
#error "arpa/tftp.h:ENOSPACE macro is missing from libc-shim"
#endif

#ifndef ENOTFOUND
#error "arpa/tftp.h:ENOTFOUND macro is missing from libc-shim"
#endif

#ifndef ENOUSER
#error "arpa/tftp.h:ENOUSER macro is missing from libc-shim"
#endif

#ifndef ERROR
#error "arpa/tftp.h:ERROR macro is missing from libc-shim"
#endif

#ifndef EUNDEF
#error "arpa/tftp.h:EUNDEF macro is missing from libc-shim"
#endif

#ifndef RRQ
#error "arpa/tftp.h:RRQ macro is missing from libc-shim"
#endif

#ifndef SEGSIZE
#error "arpa/tftp.h:SEGSIZE macro is missing from libc-shim"
#endif

#ifndef WRQ
#error "arpa/tftp.h:WRQ macro is missing from libc-shim"
#endif

#ifndef th_block
#error "arpa/tftp.h:th_block macro is missing from libc-shim"
#endif

#ifndef th_code
#error "arpa/tftp.h:th_code macro is missing from libc-shim"
#endif

#ifndef th_data
#error "arpa/tftp.h:th_data macro is missing from libc-shim"
#endif

#ifndef th_msg
#error "arpa/tftp.h:th_msg macro is missing from libc-shim"
#endif

#ifndef th_stuff
#error "arpa/tftp.h:th_stuff macro is missing from libc-shim"
#endif

int main(void) { return 0; }
