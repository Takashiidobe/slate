#include <sys/types.h>

typedef int slate_oracle_typedef_blksize_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_blksize_t, blksize_t), "typedef blksize_t differs from oracle");

typedef unsigned char slate_oracle_typedef_u_char;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_char, u_char), "typedef u_char differs from oracle");

typedef unsigned char slate_oracle_typedef_u_int8_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_u_int8_t, u_int8_t), "typedef u_int8_t differs from oracle");

typedef unsigned long slate_oracle_typedef_ulong;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_ulong, ulong), "typedef ulong differs from oracle");

typedef unsigned int slate_oracle_typedef_useconds_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_useconds_t, useconds_t), "typedef useconds_t differs from oracle");

int main(void) { return 0; }
