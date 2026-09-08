#include <ctype.h>

extern int slate_oracle_isalnum(int);
extern int slate_oracle_isalnum_l(int, struct __locale_struct *);
extern int slate_oracle_isalpha(int);
extern int slate_oracle_isalpha_l(int, struct __locale_struct *);
extern int slate_oracle_isascii(int);
extern int slate_oracle_isblank(int);
extern int slate_oracle_isblank_l(int, struct __locale_struct *);
extern int slate_oracle_iscntrl(int);
extern int slate_oracle_iscntrl_l(int, struct __locale_struct *);
extern int slate_oracle_isdigit(int);
extern int slate_oracle_isdigit_l(int, struct __locale_struct *);
extern int slate_oracle_isgraph(int);
extern int slate_oracle_isgraph_l(int, struct __locale_struct *);
extern int slate_oracle_islower(int);
extern int slate_oracle_islower_l(int, struct __locale_struct *);
extern int slate_oracle_isprint(int);
extern int slate_oracle_isprint_l(int, struct __locale_struct *);
extern int slate_oracle_ispunct(int);
extern int slate_oracle_ispunct_l(int, struct __locale_struct *);
extern int slate_oracle_isspace(int);
extern int slate_oracle_isspace_l(int, struct __locale_struct *);
extern int slate_oracle_isupper(int);
extern int slate_oracle_isupper_l(int, struct __locale_struct *);
extern int slate_oracle_isxdigit(int);
extern int slate_oracle_isxdigit_l(int, struct __locale_struct *);
extern int slate_oracle_toascii(int);
extern int slate_oracle_tolower(int);
extern int slate_oracle_tolower_l(int, struct __locale_struct *);
extern int slate_oracle_toupper(int);
extern int slate_oracle_toupper_l(int, struct __locale_struct *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isalnum), __typeof__(isalnum)),
    "ctype.h:isalnum declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isalnum_l), __typeof__(isalnum_l)),
    "ctype.h:isalnum_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isalpha), __typeof__(isalpha)),
    "ctype.h:isalpha declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isalpha_l), __typeof__(isalpha_l)),
    "ctype.h:isalpha_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isascii), __typeof__(isascii)),
    "ctype.h:isascii declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isblank), __typeof__(isblank)),
    "ctype.h:isblank declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isblank_l), __typeof__(isblank_l)),
    "ctype.h:isblank_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_iscntrl), __typeof__(iscntrl)),
    "ctype.h:iscntrl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_iscntrl_l), __typeof__(iscntrl_l)),
    "ctype.h:iscntrl_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isdigit), __typeof__(isdigit)),
    "ctype.h:isdigit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isdigit_l), __typeof__(isdigit_l)),
    "ctype.h:isdigit_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isgraph), __typeof__(isgraph)),
    "ctype.h:isgraph declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isgraph_l), __typeof__(isgraph_l)),
    "ctype.h:isgraph_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_islower), __typeof__(islower)),
    "ctype.h:islower declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_islower_l), __typeof__(islower_l)),
    "ctype.h:islower_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isprint), __typeof__(isprint)),
    "ctype.h:isprint declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isprint_l), __typeof__(isprint_l)),
    "ctype.h:isprint_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ispunct), __typeof__(ispunct)),
    "ctype.h:ispunct declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ispunct_l), __typeof__(ispunct_l)),
    "ctype.h:ispunct_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isspace), __typeof__(isspace)),
    "ctype.h:isspace declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isspace_l), __typeof__(isspace_l)),
    "ctype.h:isspace_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isupper), __typeof__(isupper)),
    "ctype.h:isupper declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isupper_l), __typeof__(isupper_l)),
    "ctype.h:isupper_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isxdigit), __typeof__(isxdigit)),
    "ctype.h:isxdigit declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isxdigit_l), __typeof__(isxdigit_l)),
    "ctype.h:isxdigit_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_toascii), __typeof__(toascii)),
    "ctype.h:toascii declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tolower), __typeof__(tolower)),
    "ctype.h:tolower declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tolower_l), __typeof__(tolower_l)),
    "ctype.h:tolower_l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_toupper), __typeof__(toupper)),
    "ctype.h:toupper declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_toupper_l), __typeof__(toupper_l)),
    "ctype.h:toupper_l declaration differs from oracle");

static __typeof__(isalnum) *const slate_reference_isalnum = &isalnum;
static __typeof__(isalnum_l) *const slate_reference_isalnum_l = &isalnum_l;
static __typeof__(isalpha) *const slate_reference_isalpha = &isalpha;
static __typeof__(isalpha_l) *const slate_reference_isalpha_l = &isalpha_l;
static __typeof__(isascii) *const slate_reference_isascii = &isascii;
static __typeof__(isblank) *const slate_reference_isblank = &isblank;
static __typeof__(isblank_l) *const slate_reference_isblank_l = &isblank_l;
static __typeof__(iscntrl) *const slate_reference_iscntrl = &iscntrl;
static __typeof__(iscntrl_l) *const slate_reference_iscntrl_l = &iscntrl_l;
static __typeof__(isdigit) *const slate_reference_isdigit = &isdigit;
static __typeof__(isdigit_l) *const slate_reference_isdigit_l = &isdigit_l;
static __typeof__(isgraph) *const slate_reference_isgraph = &isgraph;
static __typeof__(isgraph_l) *const slate_reference_isgraph_l = &isgraph_l;
static __typeof__(islower) *const slate_reference_islower = &islower;
static __typeof__(islower_l) *const slate_reference_islower_l = &islower_l;
static __typeof__(isprint) *const slate_reference_isprint = &isprint;
static __typeof__(isprint_l) *const slate_reference_isprint_l = &isprint_l;
static __typeof__(ispunct) *const slate_reference_ispunct = &ispunct;
static __typeof__(ispunct_l) *const slate_reference_ispunct_l = &ispunct_l;
static __typeof__(isspace) *const slate_reference_isspace = &isspace;
static __typeof__(isspace_l) *const slate_reference_isspace_l = &isspace_l;
static __typeof__(isupper) *const slate_reference_isupper = &isupper;
static __typeof__(isupper_l) *const slate_reference_isupper_l = &isupper_l;
static __typeof__(isxdigit) *const slate_reference_isxdigit = &isxdigit;
static __typeof__(isxdigit_l) *const slate_reference_isxdigit_l = &isxdigit_l;
static __typeof__(toascii) *const slate_reference_toascii = &toascii;
static __typeof__(tolower) *const slate_reference_tolower = &tolower;
static __typeof__(tolower_l) *const slate_reference_tolower_l = &tolower_l;
static __typeof__(toupper) *const slate_reference_toupper = &toupper;
static __typeof__(toupper_l) *const slate_reference_toupper_l = &toupper_l;

typedef struct __locale_struct * slate_oracle_typedef_locale_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_locale_t, locale_t), "typedef locale_t differs from oracle");

#ifndef isalpha
#error "ctype.h:isalpha macro is missing from libc-shim"
#endif

#ifndef isascii
#error "ctype.h:isascii macro is missing from libc-shim"
#endif

#ifndef isdigit
#error "ctype.h:isdigit macro is missing from libc-shim"
#endif

#ifndef isgraph
#error "ctype.h:isgraph macro is missing from libc-shim"
#endif

#ifndef islower
#error "ctype.h:islower macro is missing from libc-shim"
#endif

#ifndef isprint
#error "ctype.h:isprint macro is missing from libc-shim"
#endif

#ifndef isspace
#error "ctype.h:isspace macro is missing from libc-shim"
#endif

#ifndef isupper
#error "ctype.h:isupper macro is missing from libc-shim"
#endif

int main(void) { return 0; }
