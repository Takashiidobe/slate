#include <sys/mtio.h>

_Static_assert(sizeof(struct mtop) == 8, "struct mtop size differs from oracle");

_Static_assert(_Alignof(struct mtop) == 4, "struct mtop alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct mtop, mt_op) == 0, "struct mtop.mt_op offset differs from oracle");

typedef short slate_oracle_struct_mtop_mt_op;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct mtop *)0)->mt_op), slate_oracle_struct_mtop_mt_op), "struct mtop.mt_op field type differs from oracle");

_Static_assert(__builtin_offsetof(struct mtop, mt_count) == 4, "struct mtop.mt_count offset differs from oracle");

typedef int slate_oracle_struct_mtop_mt_count;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct mtop *)0)->mt_count), slate_oracle_struct_mtop_mt_count), "struct mtop.mt_count field type differs from oracle");

#ifndef DEFTAPE
#error "sys/mtio.h:DEFTAPE macro is missing from libc-shim"
#endif

#ifndef GMT_BOT
#error "sys/mtio.h:GMT_BOT macro is missing from libc-shim"
#endif

#ifndef GMT_DR_OPEN
#error "sys/mtio.h:GMT_DR_OPEN macro is missing from libc-shim"
#endif

#ifndef GMT_D_1600
#error "sys/mtio.h:GMT_D_1600 macro is missing from libc-shim"
#endif

#ifndef GMT_D_6250
#error "sys/mtio.h:GMT_D_6250 macro is missing from libc-shim"
#endif

#ifndef GMT_D_800
#error "sys/mtio.h:GMT_D_800 macro is missing from libc-shim"
#endif

#ifndef GMT_EOD
#error "sys/mtio.h:GMT_EOD macro is missing from libc-shim"
#endif

#ifndef GMT_EOF
#error "sys/mtio.h:GMT_EOF macro is missing from libc-shim"
#endif

#ifndef GMT_EOT
#error "sys/mtio.h:GMT_EOT macro is missing from libc-shim"
#endif

#ifndef GMT_IM_REP_EN
#error "sys/mtio.h:GMT_IM_REP_EN macro is missing from libc-shim"
#endif

#ifndef GMT_ONLINE
#error "sys/mtio.h:GMT_ONLINE macro is missing from libc-shim"
#endif

#ifndef GMT_SM
#error "sys/mtio.h:GMT_SM macro is missing from libc-shim"
#endif

#ifndef GMT_WR_PROT
#error "sys/mtio.h:GMT_WR_PROT macro is missing from libc-shim"
#endif

#ifndef MTBSF
#error "sys/mtio.h:MTBSF macro is missing from libc-shim"
#endif

#ifndef MTBSFM
#error "sys/mtio.h:MTBSFM macro is missing from libc-shim"
#endif

#ifndef MTBSR
#error "sys/mtio.h:MTBSR macro is missing from libc-shim"
#endif

#ifndef MTBSS
#error "sys/mtio.h:MTBSS macro is missing from libc-shim"
#endif

#ifndef MTCOMPRESSION
#error "sys/mtio.h:MTCOMPRESSION macro is missing from libc-shim"
#endif

#ifndef MTEOM
#error "sys/mtio.h:MTEOM macro is missing from libc-shim"
#endif

#ifndef MTERASE
#error "sys/mtio.h:MTERASE macro is missing from libc-shim"
#endif

#ifndef MTFSF
#error "sys/mtio.h:MTFSF macro is missing from libc-shim"
#endif

#ifndef MTFSFM
#error "sys/mtio.h:MTFSFM macro is missing from libc-shim"
#endif

#ifndef MTFSR
#error "sys/mtio.h:MTFSR macro is missing from libc-shim"
#endif

#ifndef MTFSS
#error "sys/mtio.h:MTFSS macro is missing from libc-shim"
#endif

#ifndef MTIOCGET
#error "sys/mtio.h:MTIOCGET macro is missing from libc-shim"
#endif

#ifndef MTIOCGETCONFIG
#error "sys/mtio.h:MTIOCGETCONFIG macro is missing from libc-shim"
#endif

#ifndef MTIOCPOS
#error "sys/mtio.h:MTIOCPOS macro is missing from libc-shim"
#endif

#ifndef MTIOCSETCONFIG
#error "sys/mtio.h:MTIOCSETCONFIG macro is missing from libc-shim"
#endif

#ifndef MTIOCTOP
#error "sys/mtio.h:MTIOCTOP macro is missing from libc-shim"
#endif

#ifndef MTLOAD
#error "sys/mtio.h:MTLOAD macro is missing from libc-shim"
#endif

#ifndef MTLOCK
#error "sys/mtio.h:MTLOCK macro is missing from libc-shim"
#endif

#ifndef MTMKPART
#error "sys/mtio.h:MTMKPART macro is missing from libc-shim"
#endif

#ifndef MTNOP
#error "sys/mtio.h:MTNOP macro is missing from libc-shim"
#endif

#ifndef MTOFFL
#error "sys/mtio.h:MTOFFL macro is missing from libc-shim"
#endif

#ifndef MTRAS1
#error "sys/mtio.h:MTRAS1 macro is missing from libc-shim"
#endif

#ifndef MTRAS2
#error "sys/mtio.h:MTRAS2 macro is missing from libc-shim"
#endif

#ifndef MTRAS3
#error "sys/mtio.h:MTRAS3 macro is missing from libc-shim"
#endif

#ifndef MTRESET
#error "sys/mtio.h:MTRESET macro is missing from libc-shim"
#endif

#ifndef MTRETEN
#error "sys/mtio.h:MTRETEN macro is missing from libc-shim"
#endif

#ifndef MTREW
#error "sys/mtio.h:MTREW macro is missing from libc-shim"
#endif

#ifndef MTSEEK
#error "sys/mtio.h:MTSEEK macro is missing from libc-shim"
#endif

#ifndef MTSETBLK
#error "sys/mtio.h:MTSETBLK macro is missing from libc-shim"
#endif

#ifndef MTSETDENSITY
#error "sys/mtio.h:MTSETDENSITY macro is missing from libc-shim"
#endif

#ifndef MTSETDRVBUFFER
#error "sys/mtio.h:MTSETDRVBUFFER macro is missing from libc-shim"
#endif

#ifndef MTSETPART
#error "sys/mtio.h:MTSETPART macro is missing from libc-shim"
#endif

#ifndef MTTELL
#error "sys/mtio.h:MTTELL macro is missing from libc-shim"
#endif

#ifndef MTUNLOAD
#error "sys/mtio.h:MTUNLOAD macro is missing from libc-shim"
#endif

#ifndef MTUNLOCK
#error "sys/mtio.h:MTUNLOCK macro is missing from libc-shim"
#endif

#ifndef MTWEOF
#error "sys/mtio.h:MTWEOF macro is missing from libc-shim"
#endif

#ifndef MTWSM
#error "sys/mtio.h:MTWSM macro is missing from libc-shim"
#endif

#ifndef MT_ISARCHIVESC499
#error "sys/mtio.h:MT_ISARCHIVESC499 macro is missing from libc-shim"
#endif

#ifndef MT_ISARCHIVE_2060L
#error "sys/mtio.h:MT_ISARCHIVE_2060L macro is missing from libc-shim"
#endif

#ifndef MT_ISARCHIVE_2150L
#error "sys/mtio.h:MT_ISARCHIVE_2150L macro is missing from libc-shim"
#endif

#ifndef MT_ISARCHIVE_5945L2
#error "sys/mtio.h:MT_ISARCHIVE_5945L2 macro is missing from libc-shim"
#endif

#ifndef MT_ISARCHIVE_VP60I
#error "sys/mtio.h:MT_ISARCHIVE_VP60I macro is missing from libc-shim"
#endif

#ifndef MT_ISCMSJ500
#error "sys/mtio.h:MT_ISCMSJ500 macro is missing from libc-shim"
#endif

#ifndef MT_ISDDS1
#error "sys/mtio.h:MT_ISDDS1 macro is missing from libc-shim"
#endif

#ifndef MT_ISDDS2
#error "sys/mtio.h:MT_ISDDS2 macro is missing from libc-shim"
#endif

#ifndef MT_ISEVEREX_FT40A
#error "sys/mtio.h:MT_ISEVEREX_FT40A macro is missing from libc-shim"
#endif

#ifndef MT_ISFTAPE_FLAG
#error "sys/mtio.h:MT_ISFTAPE_FLAG macro is missing from libc-shim"
#endif

#ifndef MT_ISFTAPE_UNKNOWN
#error "sys/mtio.h:MT_ISFTAPE_UNKNOWN macro is missing from libc-shim"
#endif

#ifndef MT_ISQIC02
#error "sys/mtio.h:MT_ISQIC02 macro is missing from libc-shim"
#endif

#ifndef MT_ISQIC02_ALL_FEATURES
#error "sys/mtio.h:MT_ISQIC02_ALL_FEATURES macro is missing from libc-shim"
#endif

#ifndef MT_ISSCSI1
#error "sys/mtio.h:MT_ISSCSI1 macro is missing from libc-shim"
#endif

#ifndef MT_ISSCSI2
#error "sys/mtio.h:MT_ISSCSI2 macro is missing from libc-shim"
#endif

#ifndef MT_ISTDC3610
#error "sys/mtio.h:MT_ISTDC3610 macro is missing from libc-shim"
#endif

#ifndef MT_ISTEAC_MT2ST
#error "sys/mtio.h:MT_ISTEAC_MT2ST macro is missing from libc-shim"
#endif

#ifndef MT_ISUNKNOWN
#error "sys/mtio.h:MT_ISUNKNOWN macro is missing from libc-shim"
#endif

#ifndef MT_ISWT5099EEN24
#error "sys/mtio.h:MT_ISWT5099EEN24 macro is missing from libc-shim"
#endif

#ifndef MT_ISWT5150
#error "sys/mtio.h:MT_ISWT5150 macro is missing from libc-shim"
#endif

#ifndef MT_ST_ASYNC_WRITES
#error "sys/mtio.h:MT_ST_ASYNC_WRITES macro is missing from libc-shim"
#endif

#ifndef MT_ST_AUTO_LOCK
#error "sys/mtio.h:MT_ST_AUTO_LOCK macro is missing from libc-shim"
#endif

#ifndef MT_ST_BLKSIZE_MASK
#error "sys/mtio.h:MT_ST_BLKSIZE_MASK macro is missing from libc-shim"
#endif

#ifndef MT_ST_BLKSIZE_SHIFT
#error "sys/mtio.h:MT_ST_BLKSIZE_SHIFT macro is missing from libc-shim"
#endif

#ifndef MT_ST_BOOLEANS
#error "sys/mtio.h:MT_ST_BOOLEANS macro is missing from libc-shim"
#endif

#ifndef MT_ST_BUFFER_WRITES
#error "sys/mtio.h:MT_ST_BUFFER_WRITES macro is missing from libc-shim"
#endif

#ifndef MT_ST_CAN_BSR
#error "sys/mtio.h:MT_ST_CAN_BSR macro is missing from libc-shim"
#endif

#ifndef MT_ST_CAN_PARTITIONS
#error "sys/mtio.h:MT_ST_CAN_PARTITIONS macro is missing from libc-shim"
#endif

#ifndef MT_ST_CLEARBOOLEANS
#error "sys/mtio.h:MT_ST_CLEARBOOLEANS macro is missing from libc-shim"
#endif

#ifndef MT_ST_CLEAR_DEFAULT
#error "sys/mtio.h:MT_ST_CLEAR_DEFAULT macro is missing from libc-shim"
#endif

#ifndef MT_ST_DEBUGGING
#error "sys/mtio.h:MT_ST_DEBUGGING macro is missing from libc-shim"
#endif

#ifndef MT_ST_DEF_BLKSIZE
#error "sys/mtio.h:MT_ST_DEF_BLKSIZE macro is missing from libc-shim"
#endif

#ifndef MT_ST_DEF_COMPRESSION
#error "sys/mtio.h:MT_ST_DEF_COMPRESSION macro is missing from libc-shim"
#endif

#ifndef MT_ST_DEF_DENSITY
#error "sys/mtio.h:MT_ST_DEF_DENSITY macro is missing from libc-shim"
#endif

#ifndef MT_ST_DEF_DRVBUFFER
#error "sys/mtio.h:MT_ST_DEF_DRVBUFFER macro is missing from libc-shim"
#endif

#ifndef MT_ST_DEF_OPTIONS
#error "sys/mtio.h:MT_ST_DEF_OPTIONS macro is missing from libc-shim"
#endif

#ifndef MT_ST_DEF_WRITES
#error "sys/mtio.h:MT_ST_DEF_WRITES macro is missing from libc-shim"
#endif

#ifndef MT_ST_DENSITY_MASK
#error "sys/mtio.h:MT_ST_DENSITY_MASK macro is missing from libc-shim"
#endif

#ifndef MT_ST_DENSITY_SHIFT
#error "sys/mtio.h:MT_ST_DENSITY_SHIFT macro is missing from libc-shim"
#endif

#ifndef MT_ST_FAST_MTEOM
#error "sys/mtio.h:MT_ST_FAST_MTEOM macro is missing from libc-shim"
#endif

#ifndef MT_ST_HPLOADER_OFFSET
#error "sys/mtio.h:MT_ST_HPLOADER_OFFSET macro is missing from libc-shim"
#endif

#ifndef MT_ST_NO_BLKLIMS
#error "sys/mtio.h:MT_ST_NO_BLKLIMS macro is missing from libc-shim"
#endif

#ifndef MT_ST_OPTIONS
#error "sys/mtio.h:MT_ST_OPTIONS macro is missing from libc-shim"
#endif

#ifndef MT_ST_READ_AHEAD
#error "sys/mtio.h:MT_ST_READ_AHEAD macro is missing from libc-shim"
#endif

#ifndef MT_ST_SCSI2LOGICAL
#error "sys/mtio.h:MT_ST_SCSI2LOGICAL macro is missing from libc-shim"
#endif

#ifndef MT_ST_SETBOOLEANS
#error "sys/mtio.h:MT_ST_SETBOOLEANS macro is missing from libc-shim"
#endif

#ifndef MT_ST_SOFTERR_MASK
#error "sys/mtio.h:MT_ST_SOFTERR_MASK macro is missing from libc-shim"
#endif

#ifndef MT_ST_SOFTERR_SHIFT
#error "sys/mtio.h:MT_ST_SOFTERR_SHIFT macro is missing from libc-shim"
#endif

#ifndef MT_ST_TWO_FM
#error "sys/mtio.h:MT_ST_TWO_FM macro is missing from libc-shim"
#endif

#ifndef MT_ST_WRITE_THRESHOLD
#error "sys/mtio.h:MT_ST_WRITE_THRESHOLD macro is missing from libc-shim"
#endif

#ifndef MT_TAPE_INFO
#error "sys/mtio.h:MT_TAPE_INFO macro is missing from libc-shim"
#endif

int main(void) { return 0; }
