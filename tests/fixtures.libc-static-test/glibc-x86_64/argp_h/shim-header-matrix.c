#include <argp.h>

extern char * slate_oracle_optarg;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_optarg), __typeof__(optarg)), "optarg object type differs from oracle");

static __typeof__(optarg) *const slate_reference_optarg = &optarg;

typedef int slate_oracle_typedef_error_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_error_t, error_t), "typedef error_t differs from oracle");

typedef struct __locale_struct * slate_oracle_typedef_locale_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_locale_t, locale_t), "typedef locale_t differs from oracle");

_Static_assert(sizeof(struct argp_option) == 48, "struct argp_option size differs from oracle");

_Static_assert(_Alignof(struct argp_option) == 8, "struct argp_option alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, name) == 0, "struct argp_option.name offset differs from oracle");

typedef const char * slate_oracle_struct_argp_option_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->name), slate_oracle_struct_argp_option_name), "struct argp_option.name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, key) == 8, "struct argp_option.key offset differs from oracle");

typedef int slate_oracle_struct_argp_option_key;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->key), slate_oracle_struct_argp_option_key), "struct argp_option.key field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, arg) == 16, "struct argp_option.arg offset differs from oracle");

typedef const char * slate_oracle_struct_argp_option_arg;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->arg), slate_oracle_struct_argp_option_arg), "struct argp_option.arg field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, flags) == 24, "struct argp_option.flags offset differs from oracle");

typedef int slate_oracle_struct_argp_option_flags;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->flags), slate_oracle_struct_argp_option_flags), "struct argp_option.flags field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, doc) == 32, "struct argp_option.doc offset differs from oracle");

typedef const char * slate_oracle_struct_argp_option_doc;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->doc), slate_oracle_struct_argp_option_doc), "struct argp_option.doc field type differs from oracle");

_Static_assert(__builtin_offsetof(struct argp_option, group) == 40, "struct argp_option.group offset differs from oracle");

typedef int slate_oracle_struct_argp_option_group;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct argp_option *)0)->group), slate_oracle_struct_argp_option_group), "struct argp_option.group field type differs from oracle");

_Static_assert(sizeof(struct option) == 32, "struct option size differs from oracle");

_Static_assert(_Alignof(struct option) == 8, "struct option alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct option, name) == 0, "struct option.name offset differs from oracle");

typedef const char * slate_oracle_struct_option_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct option *)0)->name), slate_oracle_struct_option_name), "struct option.name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct option, has_arg) == 8, "struct option.has_arg offset differs from oracle");

typedef int slate_oracle_struct_option_has_arg;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct option *)0)->has_arg), slate_oracle_struct_option_has_arg), "struct option.has_arg field type differs from oracle");

_Static_assert(__builtin_offsetof(struct option, flag) == 16, "struct option.flag offset differs from oracle");

typedef int * slate_oracle_struct_option_flag;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct option *)0)->flag), slate_oracle_struct_option_flag), "struct option.flag field type differs from oracle");

_Static_assert(__builtin_offsetof(struct option, val) == 24, "struct option.val offset differs from oracle");

typedef int slate_oracle_struct_option_val;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct option *)0)->val), slate_oracle_struct_option_val), "struct option.val field type differs from oracle");

#ifndef AIO_PRIO_DELTA_MAX
#error "argp.h:AIO_PRIO_DELTA_MAX macro is missing from libc-shim"
#endif

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

#ifndef BC_BASE_MAX
#error "argp.h:BC_BASE_MAX macro is missing from libc-shim"
#endif

#ifndef BC_DIM_MAX
#error "argp.h:BC_DIM_MAX macro is missing from libc-shim"
#endif

#ifndef BC_SCALE_MAX
#error "argp.h:BC_SCALE_MAX macro is missing from libc-shim"
#endif

#ifndef BC_STRING_MAX
#error "argp.h:BC_STRING_MAX macro is missing from libc-shim"
#endif

#ifndef CHARCLASS_NAME_MAX
#error "argp.h:CHARCLASS_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef COLL_WEIGHTS_MAX
#error "argp.h:COLL_WEIGHTS_MAX macro is missing from libc-shim"
#endif

#ifndef DELAYTIMER_MAX
#error "argp.h:DELAYTIMER_MAX macro is missing from libc-shim"
#endif

#ifndef ENOTSUP
#error "argp.h:ENOTSUP macro is missing from libc-shim"
#endif

#ifndef EXPR_NEST_MAX
#error "argp.h:EXPR_NEST_MAX macro is missing from libc-shim"
#endif

#ifndef FILENAME_MAX
#error "argp.h:FILENAME_MAX macro is missing from libc-shim"
#endif

#ifndef HOST_NAME_MAX
#error "argp.h:HOST_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef IOV_MAX
#error "argp.h:IOV_MAX macro is missing from libc-shim"
#endif

#ifndef LINE_MAX
#error "argp.h:LINE_MAX macro is missing from libc-shim"
#endif

#ifndef LOGIN_NAME_MAX
#error "argp.h:LOGIN_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef LONG_BIT
#error "argp.h:LONG_BIT macro is missing from libc-shim"
#endif

#ifndef MQ_PRIO_MAX
#error "argp.h:MQ_PRIO_MAX macro is missing from libc-shim"
#endif

#ifndef NL_ARGMAX
#error "argp.h:NL_ARGMAX macro is missing from libc-shim"
#endif

#ifndef NL_LANGMAX
#error "argp.h:NL_LANGMAX macro is missing from libc-shim"
#endif

#ifndef NL_MSGMAX
#error "argp.h:NL_MSGMAX macro is missing from libc-shim"
#endif

#ifndef NL_NMAX
#error "argp.h:NL_NMAX macro is missing from libc-shim"
#endif

#ifndef NL_SETMAX
#error "argp.h:NL_SETMAX macro is missing from libc-shim"
#endif

#ifndef NL_TEXTMAX
#error "argp.h:NL_TEXTMAX macro is missing from libc-shim"
#endif

#ifndef NZERO
#error "argp.h:NZERO macro is missing from libc-shim"
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

#ifndef PTHREAD_DESTRUCTOR_ITERATIONS
#error "argp.h:PTHREAD_DESTRUCTOR_ITERATIONS macro is missing from libc-shim"
#endif

#ifndef PTHREAD_KEYS_MAX
#error "argp.h:PTHREAD_KEYS_MAX macro is missing from libc-shim"
#endif

#ifndef PTHREAD_STACK_MIN
#error "argp.h:PTHREAD_STACK_MIN macro is missing from libc-shim"
#endif

#ifndef RE_DUP_MAX
#error "argp.h:RE_DUP_MAX macro is missing from libc-shim"
#endif

#ifndef SEM_VALUE_MAX
#error "argp.h:SEM_VALUE_MAX macro is missing from libc-shim"
#endif

#ifndef SSIZE_MAX
#error "argp.h:SSIZE_MAX macro is missing from libc-shim"
#endif

#ifndef TTY_NAME_MAX
#error "argp.h:TTY_NAME_MAX macro is missing from libc-shim"
#endif

#ifndef WORD_BIT
#error "argp.h:WORD_BIT macro is missing from libc-shim"
#endif

#ifndef no_argument
#error "argp.h:no_argument macro is missing from libc-shim"
#endif

#ifndef optional_argument
#error "argp.h:optional_argument macro is missing from libc-shim"
#endif

#ifndef required_argument
#error "argp.h:required_argument macro is missing from libc-shim"
#endif

int main(void) { return 0; }
