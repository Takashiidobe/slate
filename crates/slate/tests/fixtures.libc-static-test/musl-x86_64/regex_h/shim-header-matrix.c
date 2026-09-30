#include <regex.h>

_Static_assert(sizeof(struct re_pattern_buffer) == 64, "struct re_pattern_buffer size differs from oracle");

_Static_assert(_Alignof(struct re_pattern_buffer) == 8, "struct re_pattern_buffer alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct re_pattern_buffer, re_nsub) == 0, "struct re_pattern_buffer.re_nsub offset differs from oracle");

typedef unsigned long slate_oracle_struct_re_pattern_buffer_re_nsub;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct re_pattern_buffer *)0)->re_nsub), slate_oracle_struct_re_pattern_buffer_re_nsub), "struct re_pattern_buffer.re_nsub field type differs from oracle");

_Static_assert(__builtin_offsetof(struct re_pattern_buffer, __opaque) == 8, "struct re_pattern_buffer.__opaque offset differs from oracle");

typedef void * slate_oracle_struct_re_pattern_buffer___opaque;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct re_pattern_buffer *)0)->__opaque), slate_oracle_struct_re_pattern_buffer___opaque), "struct re_pattern_buffer.__opaque field type differs from oracle");

_Static_assert(__builtin_offsetof(struct re_pattern_buffer, __padding) == 16, "struct re_pattern_buffer.__padding offset differs from oracle");

_Static_assert(__builtin_offsetof(struct re_pattern_buffer, __nsub2) == 48, "struct re_pattern_buffer.__nsub2 offset differs from oracle");

typedef unsigned long slate_oracle_struct_re_pattern_buffer___nsub2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct re_pattern_buffer *)0)->__nsub2), slate_oracle_struct_re_pattern_buffer___nsub2), "struct re_pattern_buffer.__nsub2 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct re_pattern_buffer, __padding2) == 56, "struct re_pattern_buffer.__padding2 offset differs from oracle");

typedef char slate_oracle_struct_re_pattern_buffer___padding2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct re_pattern_buffer *)0)->__padding2), slate_oracle_struct_re_pattern_buffer___padding2), "struct re_pattern_buffer.__padding2 field type differs from oracle");

#ifndef REG_BADBR
#error "regex.h:REG_BADBR macro is missing from libc-shim"
#endif

#ifndef REG_BADPAT
#error "regex.h:REG_BADPAT macro is missing from libc-shim"
#endif

#ifndef REG_BADRPT
#error "regex.h:REG_BADRPT macro is missing from libc-shim"
#endif

#ifndef REG_EBRACE
#error "regex.h:REG_EBRACE macro is missing from libc-shim"
#endif

#ifndef REG_EBRACK
#error "regex.h:REG_EBRACK macro is missing from libc-shim"
#endif

#ifndef REG_ECOLLATE
#error "regex.h:REG_ECOLLATE macro is missing from libc-shim"
#endif

#ifndef REG_ECTYPE
#error "regex.h:REG_ECTYPE macro is missing from libc-shim"
#endif

#ifndef REG_EESCAPE
#error "regex.h:REG_EESCAPE macro is missing from libc-shim"
#endif

#ifndef REG_ENOSYS
#error "regex.h:REG_ENOSYS macro is missing from libc-shim"
#endif

#ifndef REG_EPAREN
#error "regex.h:REG_EPAREN macro is missing from libc-shim"
#endif

#ifndef REG_ERANGE
#error "regex.h:REG_ERANGE macro is missing from libc-shim"
#endif

#ifndef REG_ESPACE
#error "regex.h:REG_ESPACE macro is missing from libc-shim"
#endif

#ifndef REG_ESUBREG
#error "regex.h:REG_ESUBREG macro is missing from libc-shim"
#endif

#ifndef REG_EXTENDED
#error "regex.h:REG_EXTENDED macro is missing from libc-shim"
#endif

#ifndef REG_ICASE
#error "regex.h:REG_ICASE macro is missing from libc-shim"
#endif

#ifndef REG_NEWLINE
#error "regex.h:REG_NEWLINE macro is missing from libc-shim"
#endif

#ifndef REG_NOMATCH
#error "regex.h:REG_NOMATCH macro is missing from libc-shim"
#endif

#ifndef REG_NOSUB
#error "regex.h:REG_NOSUB macro is missing from libc-shim"
#endif

#ifndef REG_NOTBOL
#error "regex.h:REG_NOTBOL macro is missing from libc-shim"
#endif

#ifndef REG_NOTEOL
#error "regex.h:REG_NOTEOL macro is missing from libc-shim"
#endif

#ifndef REG_OK
#error "regex.h:REG_OK macro is missing from libc-shim"
#endif

int main(void) { return 0; }
