#include <rpc/netdb.h>

_Static_assert(sizeof(struct rpcent) == 24, "struct rpcent size differs from oracle");

_Static_assert(_Alignof(struct rpcent) == 8, "struct rpcent alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct rpcent, r_name) == 0, "struct rpcent.r_name offset differs from oracle");

typedef char * slate_oracle_struct_rpcent_r_name;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rpcent *)0)->r_name), slate_oracle_struct_rpcent_r_name), "struct rpcent.r_name field type differs from oracle");

_Static_assert(__builtin_offsetof(struct rpcent, r_aliases) == 8, "struct rpcent.r_aliases offset differs from oracle");

typedef char ** slate_oracle_struct_rpcent_r_aliases;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rpcent *)0)->r_aliases), slate_oracle_struct_rpcent_r_aliases), "struct rpcent.r_aliases field type differs from oracle");

_Static_assert(__builtin_offsetof(struct rpcent, r_number) == 16, "struct rpcent.r_number offset differs from oracle");

typedef int slate_oracle_struct_rpcent_r_number;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct rpcent *)0)->r_number), slate_oracle_struct_rpcent_r_number), "struct rpcent.r_number field type differs from oracle");

int main(void) { return 0; }
