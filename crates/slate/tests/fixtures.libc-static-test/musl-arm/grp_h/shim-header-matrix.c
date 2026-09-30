#include <grp.h>

_Static_assert(sizeof(struct group) == 16, "struct group size differs from oracle");

_Static_assert(_Alignof(struct group) == 4, "struct group alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct group, gr_name) == 0, "struct group.gr_name offset differs from oracle");

typedef char * slate_oracle_struct_group_gr_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct group *)0)->gr_name), slate_oracle_struct_group_gr_name), "struct group.gr_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct group, gr_passwd) == 4, "struct group.gr_passwd offset differs from oracle");

typedef char * slate_oracle_struct_group_gr_passwd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct group *)0)->gr_passwd), slate_oracle_struct_group_gr_passwd), "struct group.gr_passwd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct group, gr_gid) == 8, "struct group.gr_gid offset differs from oracle");

typedef unsigned int slate_oracle_struct_group_gr_gid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct group *)0)->gr_gid), slate_oracle_struct_group_gr_gid), "struct group.gr_gid field type differs from oracle");

_Static_assert(__builtin_offsetof(struct group, gr_mem) == 12, "struct group.gr_mem offset differs from oracle");

typedef char ** slate_oracle_struct_group_gr_mem;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct group *)0)->gr_mem), slate_oracle_struct_group_gr_mem), "struct group.gr_mem field type differs from oracle");

int main(void) { return 0; }
