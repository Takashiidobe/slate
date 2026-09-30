#include <sys/profil.h>

_Static_assert(sizeof(struct prof) == 16, "struct prof size differs from oracle");

_Static_assert(_Alignof(struct prof) == 4, "struct prof alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct prof, pr_base) == 0, "struct prof.pr_base offset differs from oracle");

typedef void * slate_oracle_struct_prof_pr_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prof *)0)->pr_base), slate_oracle_struct_prof_pr_base), "struct prof.pr_base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prof, pr_size) == 4, "struct prof.pr_size offset differs from oracle");

typedef unsigned int slate_oracle_struct_prof_pr_size;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prof *)0)->pr_size), slate_oracle_struct_prof_pr_size), "struct prof.pr_size field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prof, pr_off) == 8, "struct prof.pr_off offset differs from oracle");

typedef unsigned int slate_oracle_struct_prof_pr_off;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prof *)0)->pr_off), slate_oracle_struct_prof_pr_off), "struct prof.pr_off field type differs from oracle");

_Static_assert(__builtin_offsetof(struct prof, pr_scale) == 12, "struct prof.pr_scale offset differs from oracle");

typedef unsigned long slate_oracle_struct_prof_pr_scale;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct prof *)0)->pr_scale), slate_oracle_struct_prof_pr_scale), "struct prof.pr_scale field type differs from oracle");

int main(void) { return 0; }
