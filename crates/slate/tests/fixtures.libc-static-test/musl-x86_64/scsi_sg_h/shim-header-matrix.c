#include <scsi/sg.h>

_Static_assert(sizeof(struct sg_iovec) == 16, "struct sg_iovec size differs from oracle");

_Static_assert(_Alignof(struct sg_iovec) == 8, "struct sg_iovec alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct sg_iovec, iov_base) == 0, "struct sg_iovec.iov_base offset differs from oracle");

typedef void * slate_oracle_struct_sg_iovec_iov_base;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sg_iovec *)0)->iov_base), slate_oracle_struct_sg_iovec_iov_base), "struct sg_iovec.iov_base field type differs from oracle");

_Static_assert(__builtin_offsetof(struct sg_iovec, iov_len) == 8, "struct sg_iovec.iov_len offset differs from oracle");

typedef unsigned long slate_oracle_struct_sg_iovec_iov_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct sg_iovec *)0)->iov_len), slate_oracle_struct_sg_iovec_iov_len), "struct sg_iovec.iov_len field type differs from oracle");

#ifndef SG_BIG_BUFF
#error "scsi/sg.h:SG_BIG_BUFF macro is missing from libc-shim"
#endif

#ifndef SG_DEFAULT_RETRIES
#error "scsi/sg.h:SG_DEFAULT_RETRIES macro is missing from libc-shim"
#endif

#ifndef SG_DEFAULT_TIMEOUT
#error "scsi/sg.h:SG_DEFAULT_TIMEOUT macro is missing from libc-shim"
#endif

#ifndef SG_DEF_COMMAND_Q
#error "scsi/sg.h:SG_DEF_COMMAND_Q macro is missing from libc-shim"
#endif

#ifndef SG_DEF_FORCE_LOW_DMA
#error "scsi/sg.h:SG_DEF_FORCE_LOW_DMA macro is missing from libc-shim"
#endif

#ifndef SG_DEF_FORCE_PACK_ID
#error "scsi/sg.h:SG_DEF_FORCE_PACK_ID macro is missing from libc-shim"
#endif

#ifndef SG_DEF_KEEP_ORPHAN
#error "scsi/sg.h:SG_DEF_KEEP_ORPHAN macro is missing from libc-shim"
#endif

#ifndef SG_DEF_RESERVED_SIZE
#error "scsi/sg.h:SG_DEF_RESERVED_SIZE macro is missing from libc-shim"
#endif

#ifndef SG_DEF_UNDERRUN_FLAG
#error "scsi/sg.h:SG_DEF_UNDERRUN_FLAG macro is missing from libc-shim"
#endif

#ifndef SG_DXFER_FROM_DEV
#error "scsi/sg.h:SG_DXFER_FROM_DEV macro is missing from libc-shim"
#endif

#ifndef SG_DXFER_NONE
#error "scsi/sg.h:SG_DXFER_NONE macro is missing from libc-shim"
#endif

#ifndef SG_DXFER_TO_DEV
#error "scsi/sg.h:SG_DXFER_TO_DEV macro is missing from libc-shim"
#endif

#ifndef SG_DXFER_TO_FROM_DEV
#error "scsi/sg.h:SG_DXFER_TO_FROM_DEV macro is missing from libc-shim"
#endif

#ifndef SG_EMULATED_HOST
#error "scsi/sg.h:SG_EMULATED_HOST macro is missing from libc-shim"
#endif

#ifndef SG_FLAG_DIRECT_IO
#error "scsi/sg.h:SG_FLAG_DIRECT_IO macro is missing from libc-shim"
#endif

#ifndef SG_FLAG_LUN_INHIBIT
#error "scsi/sg.h:SG_FLAG_LUN_INHIBIT macro is missing from libc-shim"
#endif

#ifndef SG_FLAG_NO_DXFER
#error "scsi/sg.h:SG_FLAG_NO_DXFER macro is missing from libc-shim"
#endif

#ifndef SG_GET_COMMAND_Q
#error "scsi/sg.h:SG_GET_COMMAND_Q macro is missing from libc-shim"
#endif

#ifndef SG_GET_KEEP_ORPHAN
#error "scsi/sg.h:SG_GET_KEEP_ORPHAN macro is missing from libc-shim"
#endif

#ifndef SG_GET_LOW_DMA
#error "scsi/sg.h:SG_GET_LOW_DMA macro is missing from libc-shim"
#endif

#ifndef SG_GET_NUM_WAITING
#error "scsi/sg.h:SG_GET_NUM_WAITING macro is missing from libc-shim"
#endif

#ifndef SG_GET_PACK_ID
#error "scsi/sg.h:SG_GET_PACK_ID macro is missing from libc-shim"
#endif

#ifndef SG_GET_REQUEST_TABLE
#error "scsi/sg.h:SG_GET_REQUEST_TABLE macro is missing from libc-shim"
#endif

#ifndef SG_GET_RESERVED_SIZE
#error "scsi/sg.h:SG_GET_RESERVED_SIZE macro is missing from libc-shim"
#endif

#ifndef SG_GET_SCSI_ID
#error "scsi/sg.h:SG_GET_SCSI_ID macro is missing from libc-shim"
#endif

#ifndef SG_GET_SG_TABLESIZE
#error "scsi/sg.h:SG_GET_SG_TABLESIZE macro is missing from libc-shim"
#endif

#ifndef SG_GET_TIMEOUT
#error "scsi/sg.h:SG_GET_TIMEOUT macro is missing from libc-shim"
#endif

#ifndef SG_GET_TRANSFORM
#error "scsi/sg.h:SG_GET_TRANSFORM macro is missing from libc-shim"
#endif

#ifndef SG_GET_VERSION_NUM
#error "scsi/sg.h:SG_GET_VERSION_NUM macro is missing from libc-shim"
#endif

#ifndef SG_INFO_CHECK
#error "scsi/sg.h:SG_INFO_CHECK macro is missing from libc-shim"
#endif

#ifndef SG_INFO_DIRECT_IO
#error "scsi/sg.h:SG_INFO_DIRECT_IO macro is missing from libc-shim"
#endif

#ifndef SG_INFO_DIRECT_IO_MASK
#error "scsi/sg.h:SG_INFO_DIRECT_IO_MASK macro is missing from libc-shim"
#endif

#ifndef SG_INFO_INDIRECT_IO
#error "scsi/sg.h:SG_INFO_INDIRECT_IO macro is missing from libc-shim"
#endif

#ifndef SG_INFO_MIXED_IO
#error "scsi/sg.h:SG_INFO_MIXED_IO macro is missing from libc-shim"
#endif

#ifndef SG_INFO_OK
#error "scsi/sg.h:SG_INFO_OK macro is missing from libc-shim"
#endif

#ifndef SG_INFO_OK_MASK
#error "scsi/sg.h:SG_INFO_OK_MASK macro is missing from libc-shim"
#endif

#ifndef SG_IO
#error "scsi/sg.h:SG_IO macro is missing from libc-shim"
#endif

#ifndef SG_MAX_QUEUE
#error "scsi/sg.h:SG_MAX_QUEUE macro is missing from libc-shim"
#endif

#ifndef SG_MAX_SENSE
#error "scsi/sg.h:SG_MAX_SENSE macro is missing from libc-shim"
#endif

#ifndef SG_NEXT_CMD_LEN
#error "scsi/sg.h:SG_NEXT_CMD_LEN macro is missing from libc-shim"
#endif

#ifndef SG_SCATTER_SZ
#error "scsi/sg.h:SG_SCATTER_SZ macro is missing from libc-shim"
#endif

#ifndef SG_SCSI_RESET
#error "scsi/sg.h:SG_SCSI_RESET macro is missing from libc-shim"
#endif

#ifndef SG_SCSI_RESET_BUS
#error "scsi/sg.h:SG_SCSI_RESET_BUS macro is missing from libc-shim"
#endif

#ifndef SG_SCSI_RESET_DEVICE
#error "scsi/sg.h:SG_SCSI_RESET_DEVICE macro is missing from libc-shim"
#endif

#ifndef SG_SCSI_RESET_HOST
#error "scsi/sg.h:SG_SCSI_RESET_HOST macro is missing from libc-shim"
#endif

#ifndef SG_SCSI_RESET_NOTHING
#error "scsi/sg.h:SG_SCSI_RESET_NOTHING macro is missing from libc-shim"
#endif

#ifndef SG_SET_COMMAND_Q
#error "scsi/sg.h:SG_SET_COMMAND_Q macro is missing from libc-shim"
#endif

#ifndef SG_SET_DEBUG
#error "scsi/sg.h:SG_SET_DEBUG macro is missing from libc-shim"
#endif

#ifndef SG_SET_FORCE_LOW_DMA
#error "scsi/sg.h:SG_SET_FORCE_LOW_DMA macro is missing from libc-shim"
#endif

#ifndef SG_SET_FORCE_PACK_ID
#error "scsi/sg.h:SG_SET_FORCE_PACK_ID macro is missing from libc-shim"
#endif

#ifndef SG_SET_KEEP_ORPHAN
#error "scsi/sg.h:SG_SET_KEEP_ORPHAN macro is missing from libc-shim"
#endif

#ifndef SG_SET_RESERVED_SIZE
#error "scsi/sg.h:SG_SET_RESERVED_SIZE macro is missing from libc-shim"
#endif

#ifndef SG_SET_TIMEOUT
#error "scsi/sg.h:SG_SET_TIMEOUT macro is missing from libc-shim"
#endif

#ifndef SG_SET_TRANSFORM
#error "scsi/sg.h:SG_SET_TRANSFORM macro is missing from libc-shim"
#endif

int main(void) { return 0; }
