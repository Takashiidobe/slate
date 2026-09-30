#include <getopt.h>

extern char * slate_oracle_optarg;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_optarg), __typeof__(optarg)), "optarg object type differs from oracle");

static __typeof__(optarg) *const slate_reference_optarg = &optarg;

_Static_assert(sizeof(struct option) == 16, "struct option size differs from oracle");

_Static_assert(_Alignof(struct option) == 4, "struct option alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct option, name) == 0, "struct option.name offset differs from oracle");

typedef const char * slate_oracle_struct_option_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct option *)0)->name), slate_oracle_struct_option_name), "struct option.name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct option, has_arg) == 4, "struct option.has_arg offset differs from oracle");

typedef int slate_oracle_struct_option_has_arg;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct option *)0)->has_arg), slate_oracle_struct_option_has_arg), "struct option.has_arg field type differs from oracle");

_Static_assert(__builtin_offsetof(struct option, flag) == 8, "struct option.flag offset differs from oracle");

typedef int * slate_oracle_struct_option_flag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct option *)0)->flag), slate_oracle_struct_option_flag), "struct option.flag field type differs from oracle");

_Static_assert(__builtin_offsetof(struct option, val) == 12, "struct option.val offset differs from oracle");

typedef int slate_oracle_struct_option_val;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct option *)0)->val), slate_oracle_struct_option_val), "struct option.val field type differs from oracle");

#ifndef no_argument
#error "getopt.h:no_argument macro is missing from libc-shim"
#endif

#ifndef optional_argument
#error "getopt.h:optional_argument macro is missing from libc-shim"
#endif

#ifndef required_argument
#error "getopt.h:required_argument macro is missing from libc-shim"
#endif

int main(void) { return 0; }
