#include <sys/user.h>

typedef unsigned int slate_oracle_struct_user_fpregs_init_flag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct user_fpregs *)0)->init_flag), slate_oracle_struct_user_fpregs_init_flag), "struct user_fpregs.init_flag field type differs from oracle");

#ifndef ELF_NGREG
#error "sys/user.h:ELF_NGREG macro is missing from libc-shim"
#endif

int main(void) { return 0; }
