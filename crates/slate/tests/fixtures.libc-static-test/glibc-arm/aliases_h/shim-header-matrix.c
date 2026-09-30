#include <aliases.h>

_Static_assert(sizeof(struct aliasent) == 16, "struct aliasent size differs from oracle");

_Static_assert(_Alignof(struct aliasent) == 4, "struct aliasent alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct aliasent, alias_name) == 0, "struct aliasent.alias_name offset differs from oracle");

typedef char * slate_oracle_struct_aliasent_alias_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aliasent *)0)->alias_name), slate_oracle_struct_aliasent_alias_name), "struct aliasent.alias_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aliasent, alias_members_len) == 4, "struct aliasent.alias_members_len offset differs from oracle");

typedef unsigned int slate_oracle_struct_aliasent_alias_members_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aliasent *)0)->alias_members_len), slate_oracle_struct_aliasent_alias_members_len), "struct aliasent.alias_members_len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aliasent, alias_members) == 8, "struct aliasent.alias_members offset differs from oracle");

typedef char ** slate_oracle_struct_aliasent_alias_members;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aliasent *)0)->alias_members), slate_oracle_struct_aliasent_alias_members), "struct aliasent.alias_members field type differs from oracle");

_Static_assert(__builtin_offsetof(struct aliasent, alias_local) == 12, "struct aliasent.alias_local offset differs from oracle");

typedef int slate_oracle_struct_aliasent_alias_local;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct aliasent *)0)->alias_local), slate_oracle_struct_aliasent_alias_local), "struct aliasent.alias_local field type differs from oracle");

int main(void) { return 0; }
