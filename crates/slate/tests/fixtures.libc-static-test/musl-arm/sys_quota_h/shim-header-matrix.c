#include <sys/quota.h>

_Static_assert(sizeof(struct dqblk) == 72, "struct dqblk size differs from oracle");

_Static_assert(_Alignof(struct dqblk) == 8, "struct dqblk alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct dqblk, dqb_bhardlimit) == 0, "struct dqblk.dqb_bhardlimit offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dqblk_dqb_bhardlimit;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dqblk *)0)->dqb_bhardlimit), slate_oracle_struct_dqblk_dqb_bhardlimit), "struct dqblk.dqb_bhardlimit field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dqblk, dqb_bsoftlimit) == 8, "struct dqblk.dqb_bsoftlimit offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dqblk_dqb_bsoftlimit;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dqblk *)0)->dqb_bsoftlimit), slate_oracle_struct_dqblk_dqb_bsoftlimit), "struct dqblk.dqb_bsoftlimit field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dqblk, dqb_curspace) == 16, "struct dqblk.dqb_curspace offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dqblk_dqb_curspace;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dqblk *)0)->dqb_curspace), slate_oracle_struct_dqblk_dqb_curspace), "struct dqblk.dqb_curspace field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dqblk, dqb_ihardlimit) == 24, "struct dqblk.dqb_ihardlimit offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dqblk_dqb_ihardlimit;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dqblk *)0)->dqb_ihardlimit), slate_oracle_struct_dqblk_dqb_ihardlimit), "struct dqblk.dqb_ihardlimit field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dqblk, dqb_isoftlimit) == 32, "struct dqblk.dqb_isoftlimit offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dqblk_dqb_isoftlimit;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dqblk *)0)->dqb_isoftlimit), slate_oracle_struct_dqblk_dqb_isoftlimit), "struct dqblk.dqb_isoftlimit field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dqblk, dqb_curinodes) == 40, "struct dqblk.dqb_curinodes offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dqblk_dqb_curinodes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dqblk *)0)->dqb_curinodes), slate_oracle_struct_dqblk_dqb_curinodes), "struct dqblk.dqb_curinodes field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dqblk, dqb_btime) == 48, "struct dqblk.dqb_btime offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dqblk_dqb_btime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dqblk *)0)->dqb_btime), slate_oracle_struct_dqblk_dqb_btime), "struct dqblk.dqb_btime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dqblk, dqb_itime) == 56, "struct dqblk.dqb_itime offset differs from oracle");

typedef unsigned long long slate_oracle_struct_dqblk_dqb_itime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dqblk *)0)->dqb_itime), slate_oracle_struct_dqblk_dqb_itime), "struct dqblk.dqb_itime field type differs from oracle");

_Static_assert(__builtin_offsetof(struct dqblk, dqb_valid) == 64, "struct dqblk.dqb_valid offset differs from oracle");

typedef unsigned int slate_oracle_struct_dqblk_dqb_valid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct dqblk *)0)->dqb_valid), slate_oracle_struct_dqblk_dqb_valid), "struct dqblk.dqb_valid field type differs from oracle");

#ifndef GRPQUOTA
#error "sys/quota.h:GRPQUOTA macro is missing from libc-shim"
#endif

#ifndef IIF_ALL
#error "sys/quota.h:IIF_ALL macro is missing from libc-shim"
#endif

#ifndef IIF_BGRACE
#error "sys/quota.h:IIF_BGRACE macro is missing from libc-shim"
#endif

#ifndef IIF_FLAGS
#error "sys/quota.h:IIF_FLAGS macro is missing from libc-shim"
#endif

#ifndef IIF_IGRACE
#error "sys/quota.h:IIF_IGRACE macro is missing from libc-shim"
#endif

#ifndef INITQFNAMES
#error "sys/quota.h:INITQFNAMES macro is missing from libc-shim"
#endif

#ifndef MAXQUOTAS
#error "sys/quota.h:MAXQUOTAS macro is missing from libc-shim"
#endif

#ifndef MAX_DQ_TIME
#error "sys/quota.h:MAX_DQ_TIME macro is missing from libc-shim"
#endif

#ifndef MAX_IQ_TIME
#error "sys/quota.h:MAX_IQ_TIME macro is missing from libc-shim"
#endif

#ifndef NR_DQHASH
#error "sys/quota.h:NR_DQHASH macro is missing from libc-shim"
#endif

#ifndef NR_DQUOTS
#error "sys/quota.h:NR_DQUOTS macro is missing from libc-shim"
#endif

#ifndef QCMD
#error "sys/quota.h:QCMD macro is missing from libc-shim"
#endif

#ifndef QFMT_OCFS2
#error "sys/quota.h:QFMT_OCFS2 macro is missing from libc-shim"
#endif

#ifndef QFMT_VFS_OLD
#error "sys/quota.h:QFMT_VFS_OLD macro is missing from libc-shim"
#endif

#ifndef QFMT_VFS_V0
#error "sys/quota.h:QFMT_VFS_V0 macro is missing from libc-shim"
#endif

#ifndef QFMT_VFS_V1
#error "sys/quota.h:QFMT_VFS_V1 macro is missing from libc-shim"
#endif

#ifndef QIF_ALL
#error "sys/quota.h:QIF_ALL macro is missing from libc-shim"
#endif

#ifndef QIF_BLIMITS
#error "sys/quota.h:QIF_BLIMITS macro is missing from libc-shim"
#endif

#ifndef QIF_BTIME
#error "sys/quota.h:QIF_BTIME macro is missing from libc-shim"
#endif

#ifndef QIF_ILIMITS
#error "sys/quota.h:QIF_ILIMITS macro is missing from libc-shim"
#endif

#ifndef QIF_INODES
#error "sys/quota.h:QIF_INODES macro is missing from libc-shim"
#endif

#ifndef QIF_ITIME
#error "sys/quota.h:QIF_ITIME macro is missing from libc-shim"
#endif

#ifndef QIF_LIMITS
#error "sys/quota.h:QIF_LIMITS macro is missing from libc-shim"
#endif

#ifndef QIF_SPACE
#error "sys/quota.h:QIF_SPACE macro is missing from libc-shim"
#endif

#ifndef QIF_TIMES
#error "sys/quota.h:QIF_TIMES macro is missing from libc-shim"
#endif

#ifndef QIF_USAGE
#error "sys/quota.h:QIF_USAGE macro is missing from libc-shim"
#endif

#ifndef QUOTAFILENAME
#error "sys/quota.h:QUOTAFILENAME macro is missing from libc-shim"
#endif

#ifndef QUOTAGROUP
#error "sys/quota.h:QUOTAGROUP macro is missing from libc-shim"
#endif

#ifndef Q_GETFMT
#error "sys/quota.h:Q_GETFMT macro is missing from libc-shim"
#endif

#ifndef Q_GETINFO
#error "sys/quota.h:Q_GETINFO macro is missing from libc-shim"
#endif

#ifndef Q_GETQUOTA
#error "sys/quota.h:Q_GETQUOTA macro is missing from libc-shim"
#endif

#ifndef Q_QUOTAOFF
#error "sys/quota.h:Q_QUOTAOFF macro is missing from libc-shim"
#endif

#ifndef Q_QUOTAON
#error "sys/quota.h:Q_QUOTAON macro is missing from libc-shim"
#endif

#ifndef Q_SETINFO
#error "sys/quota.h:Q_SETINFO macro is missing from libc-shim"
#endif

#ifndef Q_SETQUOTA
#error "sys/quota.h:Q_SETQUOTA macro is missing from libc-shim"
#endif

#ifndef Q_SYNC
#error "sys/quota.h:Q_SYNC macro is missing from libc-shim"
#endif

#ifndef SUBCMDMASK
#error "sys/quota.h:SUBCMDMASK macro is missing from libc-shim"
#endif

#ifndef SUBCMDSHIFT
#error "sys/quota.h:SUBCMDSHIFT macro is missing from libc-shim"
#endif

#ifndef USRQUOTA
#error "sys/quota.h:USRQUOTA macro is missing from libc-shim"
#endif

#ifndef btodb
#error "sys/quota.h:btodb macro is missing from libc-shim"
#endif

#ifndef dbtob
#error "sys/quota.h:dbtob macro is missing from libc-shim"
#endif

#ifndef dq_bhardlimit
#error "sys/quota.h:dq_bhardlimit macro is missing from libc-shim"
#endif

#ifndef dq_bsoftlimit
#error "sys/quota.h:dq_bsoftlimit macro is missing from libc-shim"
#endif

#ifndef dq_btime
#error "sys/quota.h:dq_btime macro is missing from libc-shim"
#endif

#ifndef dq_curinodes
#error "sys/quota.h:dq_curinodes macro is missing from libc-shim"
#endif

#ifndef dq_curspace
#error "sys/quota.h:dq_curspace macro is missing from libc-shim"
#endif

#ifndef dq_ihardlimit
#error "sys/quota.h:dq_ihardlimit macro is missing from libc-shim"
#endif

#ifndef dq_isoftlimit
#error "sys/quota.h:dq_isoftlimit macro is missing from libc-shim"
#endif

#ifndef dq_itime
#error "sys/quota.h:dq_itime macro is missing from libc-shim"
#endif

#ifndef dq_valid
#error "sys/quota.h:dq_valid macro is missing from libc-shim"
#endif

#ifndef dqoff
#error "sys/quota.h:dqoff macro is missing from libc-shim"
#endif

#ifndef fs_to_dq_blocks
#error "sys/quota.h:fs_to_dq_blocks macro is missing from libc-shim"
#endif

int main(void) { return 0; }
