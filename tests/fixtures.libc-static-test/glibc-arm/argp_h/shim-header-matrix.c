#include <argp.h>

_Static_assert(sizeof(struct argp_option) == 24, "struct argp_option size differs from oracle");

_Static_assert(_Alignof(struct argp_option) == 4, "struct argp_option alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, name) == 0, "struct argp_option.name offset differs from oracle");

typedef const char * slate_oracle_struct_argp_option_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->name), slate_oracle_struct_argp_option_name), "struct argp_option.name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, key) == 4, "struct argp_option.key offset differs from oracle");

typedef int slate_oracle_struct_argp_option_key;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->key), slate_oracle_struct_argp_option_key), "struct argp_option.key field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, arg) == 8, "struct argp_option.arg offset differs from oracle");

typedef const char * slate_oracle_struct_argp_option_arg;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->arg), slate_oracle_struct_argp_option_arg), "struct argp_option.arg field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, flags) == 12, "struct argp_option.flags offset differs from oracle");

typedef int slate_oracle_struct_argp_option_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->flags), slate_oracle_struct_argp_option_flags), "struct argp_option.flags field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, doc) == 16, "struct argp_option.doc offset differs from oracle");

typedef const char * slate_oracle_struct_argp_option_doc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->doc), slate_oracle_struct_argp_option_doc), "struct argp_option.doc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, group) == 20, "struct argp_option.group offset differs from oracle");

typedef int slate_oracle_struct_argp_option_group;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->group), slate_oracle_struct_argp_option_group), "struct argp_option.group field type differs from oracle");

#ifndef ARGP_ERR_UNKNOWN
#error "argp.h:ARGP_ERR_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_BUG_ADDR
#error "argp.h:ARGP_HELP_BUG_ADDR macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_DOC
#error "argp.h:ARGP_HELP_DOC macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_EXIT_ERR
#error "argp.h:ARGP_HELP_EXIT_ERR macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_EXIT_OK
#error "argp.h:ARGP_HELP_EXIT_OK macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_LONG
#error "argp.h:ARGP_HELP_LONG macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_LONG_ONLY
#error "argp.h:ARGP_HELP_LONG_ONLY macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_POST_DOC
#error "argp.h:ARGP_HELP_POST_DOC macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_PRE_DOC
#error "argp.h:ARGP_HELP_PRE_DOC macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_SEE
#error "argp.h:ARGP_HELP_SEE macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_SHORT_USAGE
#error "argp.h:ARGP_HELP_SHORT_USAGE macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_STD_ERR
#error "argp.h:ARGP_HELP_STD_ERR macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_STD_HELP
#error "argp.h:ARGP_HELP_STD_HELP macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_STD_USAGE
#error "argp.h:ARGP_HELP_STD_USAGE macro is missing from libc-shim"
#endif

#ifndef ARGP_HELP_USAGE
#error "argp.h:ARGP_HELP_USAGE macro is missing from libc-shim"
#endif

#ifndef ARGP_IN_ORDER
#error "argp.h:ARGP_IN_ORDER macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_ARG
#error "argp.h:ARGP_KEY_ARG macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_ARGS
#error "argp.h:ARGP_KEY_ARGS macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_END
#error "argp.h:ARGP_KEY_END macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_ERROR
#error "argp.h:ARGP_KEY_ERROR macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_FINI
#error "argp.h:ARGP_KEY_FINI macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_HELP_ARGS_DOC
#error "argp.h:ARGP_KEY_HELP_ARGS_DOC macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_HELP_DUP_ARGS_NOTE
#error "argp.h:ARGP_KEY_HELP_DUP_ARGS_NOTE macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_HELP_EXTRA
#error "argp.h:ARGP_KEY_HELP_EXTRA macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_HELP_HEADER
#error "argp.h:ARGP_KEY_HELP_HEADER macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_HELP_POST_DOC
#error "argp.h:ARGP_KEY_HELP_POST_DOC macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_HELP_PRE_DOC
#error "argp.h:ARGP_KEY_HELP_PRE_DOC macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_INIT
#error "argp.h:ARGP_KEY_INIT macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_NO_ARGS
#error "argp.h:ARGP_KEY_NO_ARGS macro is missing from libc-shim"
#endif

#ifndef ARGP_KEY_SUCCESS
#error "argp.h:ARGP_KEY_SUCCESS macro is missing from libc-shim"
#endif

#ifndef ARGP_LONG_ONLY
#error "argp.h:ARGP_LONG_ONLY macro is missing from libc-shim"
#endif

#ifndef ARGP_NO_ARGS
#error "argp.h:ARGP_NO_ARGS macro is missing from libc-shim"
#endif

#ifndef ARGP_NO_ERRS
#error "argp.h:ARGP_NO_ERRS macro is missing from libc-shim"
#endif

#ifndef ARGP_NO_EXIT
#error "argp.h:ARGP_NO_EXIT macro is missing from libc-shim"
#endif

#ifndef ARGP_NO_HELP
#error "argp.h:ARGP_NO_HELP macro is missing from libc-shim"
#endif

#ifndef ARGP_PARSE_ARGV0
#error "argp.h:ARGP_PARSE_ARGV0 macro is missing from libc-shim"
#endif

#ifndef ARGP_SILENT
#error "argp.h:ARGP_SILENT macro is missing from libc-shim"
#endif

#ifndef OPTION_ALIAS
#error "argp.h:OPTION_ALIAS macro is missing from libc-shim"
#endif

#ifndef OPTION_ARG_OPTIONAL
#error "argp.h:OPTION_ARG_OPTIONAL macro is missing from libc-shim"
#endif

#ifndef OPTION_DOC
#error "argp.h:OPTION_DOC macro is missing from libc-shim"
#endif

#ifndef OPTION_HIDDEN
#error "argp.h:OPTION_HIDDEN macro is missing from libc-shim"
#endif

#ifndef OPTION_NO_USAGE
#error "argp.h:OPTION_NO_USAGE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
