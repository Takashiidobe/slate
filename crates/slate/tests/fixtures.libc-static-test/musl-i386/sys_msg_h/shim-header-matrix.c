#include <sys/msg.h>

typedef unsigned long slate_oracle_typedef_msgqnum_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_msgqnum_t, msgqnum_t), "typedef msgqnum_t differs from oracle");

_Static_assert(sizeof(struct msginfo) == 32, "struct msginfo size differs from oracle");

_Static_assert(_Alignof(struct msginfo) == 4, "struct msginfo alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct msginfo, msgpool) == 0, "struct msginfo.msgpool offset differs from oracle");

typedef int slate_oracle_struct_msginfo_msgpool;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msginfo *)0)->msgpool), slate_oracle_struct_msginfo_msgpool), "struct msginfo.msgpool field type differs from oracle");

_Static_assert(__builtin_offsetof(struct msginfo, msgmap) == 4, "struct msginfo.msgmap offset differs from oracle");

typedef int slate_oracle_struct_msginfo_msgmap;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msginfo *)0)->msgmap), slate_oracle_struct_msginfo_msgmap), "struct msginfo.msgmap field type differs from oracle");

_Static_assert(__builtin_offsetof(struct msginfo, msgmax) == 8, "struct msginfo.msgmax offset differs from oracle");

typedef int slate_oracle_struct_msginfo_msgmax;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msginfo *)0)->msgmax), slate_oracle_struct_msginfo_msgmax), "struct msginfo.msgmax field type differs from oracle");

_Static_assert(__builtin_offsetof(struct msginfo, msgmnb) == 12, "struct msginfo.msgmnb offset differs from oracle");

typedef int slate_oracle_struct_msginfo_msgmnb;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msginfo *)0)->msgmnb), slate_oracle_struct_msginfo_msgmnb), "struct msginfo.msgmnb field type differs from oracle");

_Static_assert(__builtin_offsetof(struct msginfo, msgmni) == 16, "struct msginfo.msgmni offset differs from oracle");

typedef int slate_oracle_struct_msginfo_msgmni;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msginfo *)0)->msgmni), slate_oracle_struct_msginfo_msgmni), "struct msginfo.msgmni field type differs from oracle");

_Static_assert(__builtin_offsetof(struct msginfo, msgssz) == 20, "struct msginfo.msgssz offset differs from oracle");

typedef int slate_oracle_struct_msginfo_msgssz;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msginfo *)0)->msgssz), slate_oracle_struct_msginfo_msgssz), "struct msginfo.msgssz field type differs from oracle");

_Static_assert(__builtin_offsetof(struct msginfo, msgtql) == 24, "struct msginfo.msgtql offset differs from oracle");

typedef int slate_oracle_struct_msginfo_msgtql;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msginfo *)0)->msgtql), slate_oracle_struct_msginfo_msgtql), "struct msginfo.msgtql field type differs from oracle");

_Static_assert(__builtin_offsetof(struct msginfo, msgseg) == 28, "struct msginfo.msgseg offset differs from oracle");

typedef unsigned short slate_oracle_struct_msginfo_msgseg;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msginfo *)0)->msgseg), slate_oracle_struct_msginfo_msgseg), "struct msginfo.msgseg field type differs from oracle");

typedef struct ipc_perm slate_oracle_struct_msqid_ds_msg_perm;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->msg_perm), slate_oracle_struct_msqid_ds_msg_perm), "struct msqid_ds.msg_perm field type differs from oracle");

typedef unsigned long slate_oracle_struct_msqid_ds___msg_stime_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->__msg_stime_lo), slate_oracle_struct_msqid_ds___msg_stime_lo), "struct msqid_ds.__msg_stime_lo field type differs from oracle");

typedef unsigned long slate_oracle_struct_msqid_ds___msg_stime_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->__msg_stime_hi), slate_oracle_struct_msqid_ds___msg_stime_hi), "struct msqid_ds.__msg_stime_hi field type differs from oracle");

typedef unsigned long slate_oracle_struct_msqid_ds___msg_rtime_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->__msg_rtime_lo), slate_oracle_struct_msqid_ds___msg_rtime_lo), "struct msqid_ds.__msg_rtime_lo field type differs from oracle");

typedef unsigned long slate_oracle_struct_msqid_ds___msg_rtime_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->__msg_rtime_hi), slate_oracle_struct_msqid_ds___msg_rtime_hi), "struct msqid_ds.__msg_rtime_hi field type differs from oracle");

typedef unsigned long slate_oracle_struct_msqid_ds___msg_ctime_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->__msg_ctime_lo), slate_oracle_struct_msqid_ds___msg_ctime_lo), "struct msqid_ds.__msg_ctime_lo field type differs from oracle");

typedef unsigned long slate_oracle_struct_msqid_ds___msg_ctime_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->__msg_ctime_hi), slate_oracle_struct_msqid_ds___msg_ctime_hi), "struct msqid_ds.__msg_ctime_hi field type differs from oracle");

typedef unsigned long slate_oracle_struct_msqid_ds_msg_cbytes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->msg_cbytes), slate_oracle_struct_msqid_ds_msg_cbytes), "struct msqid_ds.msg_cbytes field type differs from oracle");

typedef unsigned long slate_oracle_struct_msqid_ds_msg_qnum;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->msg_qnum), slate_oracle_struct_msqid_ds_msg_qnum), "struct msqid_ds.msg_qnum field type differs from oracle");

typedef unsigned long slate_oracle_struct_msqid_ds_msg_qbytes;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->msg_qbytes), slate_oracle_struct_msqid_ds_msg_qbytes), "struct msqid_ds.msg_qbytes field type differs from oracle");

typedef int slate_oracle_struct_msqid_ds_msg_lspid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->msg_lspid), slate_oracle_struct_msqid_ds_msg_lspid), "struct msqid_ds.msg_lspid field type differs from oracle");

typedef int slate_oracle_struct_msqid_ds_msg_lrpid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->msg_lrpid), slate_oracle_struct_msqid_ds_msg_lrpid), "struct msqid_ds.msg_lrpid field type differs from oracle");

typedef long long slate_oracle_struct_msqid_ds_msg_stime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->msg_stime), slate_oracle_struct_msqid_ds_msg_stime), "struct msqid_ds.msg_stime field type differs from oracle");

typedef long long slate_oracle_struct_msqid_ds_msg_rtime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->msg_rtime), slate_oracle_struct_msqid_ds_msg_rtime), "struct msqid_ds.msg_rtime field type differs from oracle");

typedef long long slate_oracle_struct_msqid_ds_msg_ctime;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct msqid_ds *)0)->msg_ctime), slate_oracle_struct_msqid_ds_msg_ctime), "struct msqid_ds.msg_ctime field type differs from oracle");

#ifndef MSG_EXCEPT
#error "sys/msg.h:MSG_EXCEPT macro is missing from libc-shim"
#endif

#ifndef MSG_INFO
#error "sys/msg.h:MSG_INFO macro is missing from libc-shim"
#endif

#ifndef MSG_NOERROR
#error "sys/msg.h:MSG_NOERROR macro is missing from libc-shim"
#endif

#ifndef MSG_STAT
#error "sys/msg.h:MSG_STAT macro is missing from libc-shim"
#endif

#ifndef MSG_STAT_ANY
#error "sys/msg.h:MSG_STAT_ANY macro is missing from libc-shim"
#endif

int main(void) { return 0; }
