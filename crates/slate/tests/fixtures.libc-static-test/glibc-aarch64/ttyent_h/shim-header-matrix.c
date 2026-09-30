#include <ttyent.h>

_Static_assert(sizeof(struct ttyent) == 48, "struct ttyent size differs from oracle");

_Static_assert(_Alignof(struct ttyent) == 8, "struct ttyent alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct ttyent, ty_name) == 0, "struct ttyent.ty_name offset differs from oracle");

typedef char * slate_oracle_struct_ttyent_ty_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttyent *)0)->ty_name), slate_oracle_struct_ttyent_ty_name), "struct ttyent.ty_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttyent, ty_getty) == 8, "struct ttyent.ty_getty offset differs from oracle");

typedef char * slate_oracle_struct_ttyent_ty_getty;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttyent *)0)->ty_getty), slate_oracle_struct_ttyent_ty_getty), "struct ttyent.ty_getty field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttyent, ty_type) == 16, "struct ttyent.ty_type offset differs from oracle");

typedef char * slate_oracle_struct_ttyent_ty_type;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttyent *)0)->ty_type), slate_oracle_struct_ttyent_ty_type), "struct ttyent.ty_type field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttyent, ty_status) == 24, "struct ttyent.ty_status offset differs from oracle");

typedef int slate_oracle_struct_ttyent_ty_status;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttyent *)0)->ty_status), slate_oracle_struct_ttyent_ty_status), "struct ttyent.ty_status field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttyent, ty_window) == 32, "struct ttyent.ty_window offset differs from oracle");

typedef char * slate_oracle_struct_ttyent_ty_window;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttyent *)0)->ty_window), slate_oracle_struct_ttyent_ty_window), "struct ttyent.ty_window field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ttyent, ty_comment) == 40, "struct ttyent.ty_comment offset differs from oracle");

typedef char * slate_oracle_struct_ttyent_ty_comment;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ttyent *)0)->ty_comment), slate_oracle_struct_ttyent_ty_comment), "struct ttyent.ty_comment field type differs from oracle");

#ifndef TTY_ON
#error "ttyent.h:TTY_ON macro is missing from libc-shim"
#endif

#ifndef TTY_SECURE
#error "ttyent.h:TTY_SECURE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
