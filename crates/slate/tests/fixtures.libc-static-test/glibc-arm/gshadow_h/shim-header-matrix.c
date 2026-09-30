#include <gshadow.h>

_Static_assert(sizeof(struct sgrp) == 16, "struct sgrp size differs from oracle");

_Static_assert(_Alignof(struct sgrp) == 4, "struct sgrp alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sgrp, sg_namp) == 0, "struct sgrp.sg_namp offset differs from oracle");

typedef char * slate_oracle_struct_sgrp_sg_namp;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sgrp *)0)->sg_namp), slate_oracle_struct_sgrp_sg_namp), "struct sgrp.sg_namp field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sgrp, sg_passwd) == 4, "struct sgrp.sg_passwd offset differs from oracle");

typedef char * slate_oracle_struct_sgrp_sg_passwd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sgrp *)0)->sg_passwd), slate_oracle_struct_sgrp_sg_passwd), "struct sgrp.sg_passwd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sgrp, sg_adm) == 8, "struct sgrp.sg_adm offset differs from oracle");

typedef char ** slate_oracle_struct_sgrp_sg_adm;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sgrp *)0)->sg_adm), slate_oracle_struct_sgrp_sg_adm), "struct sgrp.sg_adm field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sgrp, sg_mem) == 12, "struct sgrp.sg_mem offset differs from oracle");

typedef char ** slate_oracle_struct_sgrp_sg_mem;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sgrp *)0)->sg_mem), slate_oracle_struct_sgrp_sg_mem), "struct sgrp.sg_mem field type differs from oracle");

#ifndef GSHADOW
#error "gshadow.h:GSHADOW macro is missing from libc-shim"
#endif

int main(void) { return 0; }
