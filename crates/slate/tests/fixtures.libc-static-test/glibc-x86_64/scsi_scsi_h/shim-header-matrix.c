#include <scsi/scsi.h>

_Static_assert(sizeof(struct ccs_modesel_head) == 12, "struct ccs_modesel_head size differs from oracle");

_Static_assert(_Alignof(struct ccs_modesel_head) == 1, "struct ccs_modesel_head alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, _r1) == 0, "struct ccs_modesel_head._r1 offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head__r1;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->_r1), slate_oracle_struct_ccs_modesel_head__r1), "struct ccs_modesel_head._r1 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, medium) == 1, "struct ccs_modesel_head.medium offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head_medium;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->medium), slate_oracle_struct_ccs_modesel_head_medium), "struct ccs_modesel_head.medium field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, _r2) == 2, "struct ccs_modesel_head._r2 offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head__r2;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->_r2), slate_oracle_struct_ccs_modesel_head__r2), "struct ccs_modesel_head._r2 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, block_desc_length) == 3, "struct ccs_modesel_head.block_desc_length offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head_block_desc_length;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->block_desc_length), slate_oracle_struct_ccs_modesel_head_block_desc_length), "struct ccs_modesel_head.block_desc_length field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, density) == 4, "struct ccs_modesel_head.density offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head_density;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->density), slate_oracle_struct_ccs_modesel_head_density), "struct ccs_modesel_head.density field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, number_blocks_hi) == 5, "struct ccs_modesel_head.number_blocks_hi offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head_number_blocks_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->number_blocks_hi), slate_oracle_struct_ccs_modesel_head_number_blocks_hi), "struct ccs_modesel_head.number_blocks_hi field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, number_blocks_med) == 6, "struct ccs_modesel_head.number_blocks_med offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head_number_blocks_med;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->number_blocks_med), slate_oracle_struct_ccs_modesel_head_number_blocks_med), "struct ccs_modesel_head.number_blocks_med field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, number_blocks_lo) == 7, "struct ccs_modesel_head.number_blocks_lo offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head_number_blocks_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->number_blocks_lo), slate_oracle_struct_ccs_modesel_head_number_blocks_lo), "struct ccs_modesel_head.number_blocks_lo field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, _r3) == 8, "struct ccs_modesel_head._r3 offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head__r3;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->_r3), slate_oracle_struct_ccs_modesel_head__r3), "struct ccs_modesel_head._r3 field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, block_length_hi) == 9, "struct ccs_modesel_head.block_length_hi offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head_block_length_hi;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->block_length_hi), slate_oracle_struct_ccs_modesel_head_block_length_hi), "struct ccs_modesel_head.block_length_hi field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, block_length_med) == 10, "struct ccs_modesel_head.block_length_med offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head_block_length_med;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->block_length_med), slate_oracle_struct_ccs_modesel_head_block_length_med), "struct ccs_modesel_head.block_length_med field type differs from oracle");

_Static_assert(__builtin_offsetof(struct ccs_modesel_head, block_length_lo) == 11, "struct ccs_modesel_head.block_length_lo offset differs from oracle");

typedef unsigned char slate_oracle_struct_ccs_modesel_head_block_length_lo;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct ccs_modesel_head *)0)->block_length_lo), slate_oracle_struct_ccs_modesel_head_block_length_lo), "struct ccs_modesel_head.block_length_lo field type differs from oracle");

#ifndef ABORT
#error "scsi/scsi.h:ABORT macro is missing from libc-shim"
#endif

#ifndef ABORTED_COMMAND
#error "scsi/scsi.h:ABORTED_COMMAND macro is missing from libc-shim"
#endif

#ifndef ALLOW_MEDIUM_REMOVAL
#error "scsi/scsi.h:ALLOW_MEDIUM_REMOVAL macro is missing from libc-shim"
#endif

#ifndef BLANK_CHECK
#error "scsi/scsi.h:BLANK_CHECK macro is missing from libc-shim"
#endif

#ifndef BUSY
#error "scsi/scsi.h:BUSY macro is missing from libc-shim"
#endif

#ifndef BUS_DEVICE_RESET
#error "scsi/scsi.h:BUS_DEVICE_RESET macro is missing from libc-shim"
#endif

#ifndef CHANGE_DEFINITION
#error "scsi/scsi.h:CHANGE_DEFINITION macro is missing from libc-shim"
#endif

#ifndef CHECK_CONDITION
#error "scsi/scsi.h:CHECK_CONDITION macro is missing from libc-shim"
#endif

#ifndef COMMAND_COMPLETE
#error "scsi/scsi.h:COMMAND_COMPLETE macro is missing from libc-shim"
#endif

#ifndef COMMAND_TERMINATED
#error "scsi/scsi.h:COMMAND_TERMINATED macro is missing from libc-shim"
#endif

#ifndef COMPARE
#error "scsi/scsi.h:COMPARE macro is missing from libc-shim"
#endif

#ifndef CONDITION_GOOD
#error "scsi/scsi.h:CONDITION_GOOD macro is missing from libc-shim"
#endif

#ifndef COPY
#error "scsi/scsi.h:COPY macro is missing from libc-shim"
#endif

#ifndef COPY_ABORTED
#error "scsi/scsi.h:COPY_ABORTED macro is missing from libc-shim"
#endif

#ifndef COPY_VERIFY
#error "scsi/scsi.h:COPY_VERIFY macro is missing from libc-shim"
#endif

#ifndef DATA_PROTECT
#error "scsi/scsi.h:DATA_PROTECT macro is missing from libc-shim"
#endif

#ifndef DISCONNECT
#error "scsi/scsi.h:DISCONNECT macro is missing from libc-shim"
#endif

#ifndef ERASE
#error "scsi/scsi.h:ERASE macro is missing from libc-shim"
#endif

#ifndef EXTENDED_EXTENDED_IDENTIFY
#error "scsi/scsi.h:EXTENDED_EXTENDED_IDENTIFY macro is missing from libc-shim"
#endif

#ifndef EXTENDED_MESSAGE
#error "scsi/scsi.h:EXTENDED_MESSAGE macro is missing from libc-shim"
#endif

#ifndef EXTENDED_MODIFY_DATA_POINTER
#error "scsi/scsi.h:EXTENDED_MODIFY_DATA_POINTER macro is missing from libc-shim"
#endif

#ifndef EXTENDED_SDTR
#error "scsi/scsi.h:EXTENDED_SDTR macro is missing from libc-shim"
#endif

#ifndef EXTENDED_WDTR
#error "scsi/scsi.h:EXTENDED_WDTR macro is missing from libc-shim"
#endif

#ifndef FORMAT_UNIT
#error "scsi/scsi.h:FORMAT_UNIT macro is missing from libc-shim"
#endif

#ifndef GOOD
#error "scsi/scsi.h:GOOD macro is missing from libc-shim"
#endif

#ifndef HARDWARE_ERROR
#error "scsi/scsi.h:HARDWARE_ERROR macro is missing from libc-shim"
#endif

#ifndef HEAD_OF_QUEUE_TAG
#error "scsi/scsi.h:HEAD_OF_QUEUE_TAG macro is missing from libc-shim"
#endif

#ifndef ILLEGAL_REQUEST
#error "scsi/scsi.h:ILLEGAL_REQUEST macro is missing from libc-shim"
#endif

#ifndef INITIATE_RECOVERY
#error "scsi/scsi.h:INITIATE_RECOVERY macro is missing from libc-shim"
#endif

#ifndef INITIATOR_ERROR
#error "scsi/scsi.h:INITIATOR_ERROR macro is missing from libc-shim"
#endif

#ifndef INQUIRY
#error "scsi/scsi.h:INQUIRY macro is missing from libc-shim"
#endif

#ifndef INTERMEDIATE_C_GOOD
#error "scsi/scsi.h:INTERMEDIATE_C_GOOD macro is missing from libc-shim"
#endif

#ifndef INTERMEDIATE_GOOD
#error "scsi/scsi.h:INTERMEDIATE_GOOD macro is missing from libc-shim"
#endif

#ifndef LINKED_CMD_COMPLETE
#error "scsi/scsi.h:LINKED_CMD_COMPLETE macro is missing from libc-shim"
#endif

#ifndef LINKED_FLG_CMD_COMPLETE
#error "scsi/scsi.h:LINKED_FLG_CMD_COMPLETE macro is missing from libc-shim"
#endif

#ifndef LOCK_UNLOCK_CACHE
#error "scsi/scsi.h:LOCK_UNLOCK_CACHE macro is missing from libc-shim"
#endif

#ifndef LOG_SELECT
#error "scsi/scsi.h:LOG_SELECT macro is missing from libc-shim"
#endif

#ifndef LOG_SENSE
#error "scsi/scsi.h:LOG_SENSE macro is missing from libc-shim"
#endif

#ifndef MEDIUM_ERROR
#error "scsi/scsi.h:MEDIUM_ERROR macro is missing from libc-shim"
#endif

#ifndef MEDIUM_SCAN
#error "scsi/scsi.h:MEDIUM_SCAN macro is missing from libc-shim"
#endif

#ifndef MESSAGE_REJECT
#error "scsi/scsi.h:MESSAGE_REJECT macro is missing from libc-shim"
#endif

#ifndef MISCOMPARE
#error "scsi/scsi.h:MISCOMPARE macro is missing from libc-shim"
#endif

#ifndef MODE_SELECT
#error "scsi/scsi.h:MODE_SELECT macro is missing from libc-shim"
#endif

#ifndef MODE_SELECT_10
#error "scsi/scsi.h:MODE_SELECT_10 macro is missing from libc-shim"
#endif

#ifndef MODE_SENSE
#error "scsi/scsi.h:MODE_SENSE macro is missing from libc-shim"
#endif

#ifndef MODE_SENSE_10
#error "scsi/scsi.h:MODE_SENSE_10 macro is missing from libc-shim"
#endif

#ifndef MOVE_MEDIUM
#error "scsi/scsi.h:MOVE_MEDIUM macro is missing from libc-shim"
#endif

#ifndef MSG_PARITY_ERROR
#error "scsi/scsi.h:MSG_PARITY_ERROR macro is missing from libc-shim"
#endif

#ifndef NOP
#error "scsi/scsi.h:NOP macro is missing from libc-shim"
#endif

#ifndef NOT_READY
#error "scsi/scsi.h:NOT_READY macro is missing from libc-shim"
#endif

#ifndef NO_SENSE
#error "scsi/scsi.h:NO_SENSE macro is missing from libc-shim"
#endif

#ifndef ORDERED_QUEUE_TAG
#error "scsi/scsi.h:ORDERED_QUEUE_TAG macro is missing from libc-shim"
#endif

#ifndef PERSISTENT_RESERVE_IN
#error "scsi/scsi.h:PERSISTENT_RESERVE_IN macro is missing from libc-shim"
#endif

#ifndef PERSISTENT_RESERVE_OUT
#error "scsi/scsi.h:PERSISTENT_RESERVE_OUT macro is missing from libc-shim"
#endif

#ifndef PRE_FETCH
#error "scsi/scsi.h:PRE_FETCH macro is missing from libc-shim"
#endif

#ifndef QUEUE_FULL
#error "scsi/scsi.h:QUEUE_FULL macro is missing from libc-shim"
#endif

#ifndef READ_10
#error "scsi/scsi.h:READ_10 macro is missing from libc-shim"
#endif

#ifndef READ_12
#error "scsi/scsi.h:READ_12 macro is missing from libc-shim"
#endif

#ifndef READ_6
#error "scsi/scsi.h:READ_6 macro is missing from libc-shim"
#endif

#ifndef READ_BLOCK_LIMITS
#error "scsi/scsi.h:READ_BLOCK_LIMITS macro is missing from libc-shim"
#endif

#ifndef READ_BUFFER
#error "scsi/scsi.h:READ_BUFFER macro is missing from libc-shim"
#endif

#ifndef READ_CAPACITY
#error "scsi/scsi.h:READ_CAPACITY macro is missing from libc-shim"
#endif

#ifndef READ_DEFECT_DATA
#error "scsi/scsi.h:READ_DEFECT_DATA macro is missing from libc-shim"
#endif

#ifndef READ_ELEMENT_STATUS
#error "scsi/scsi.h:READ_ELEMENT_STATUS macro is missing from libc-shim"
#endif

#ifndef READ_LONG
#error "scsi/scsi.h:READ_LONG macro is missing from libc-shim"
#endif

#ifndef READ_POSITION
#error "scsi/scsi.h:READ_POSITION macro is missing from libc-shim"
#endif

#ifndef READ_REVERSE
#error "scsi/scsi.h:READ_REVERSE macro is missing from libc-shim"
#endif

#ifndef READ_TOC
#error "scsi/scsi.h:READ_TOC macro is missing from libc-shim"
#endif

#ifndef REASSIGN_BLOCKS
#error "scsi/scsi.h:REASSIGN_BLOCKS macro is missing from libc-shim"
#endif

#ifndef RECEIVE_DIAGNOSTIC
#error "scsi/scsi.h:RECEIVE_DIAGNOSTIC macro is missing from libc-shim"
#endif

#ifndef RECOVERED_ERROR
#error "scsi/scsi.h:RECOVERED_ERROR macro is missing from libc-shim"
#endif

#ifndef RECOVER_BUFFERED_DATA
#error "scsi/scsi.h:RECOVER_BUFFERED_DATA macro is missing from libc-shim"
#endif

#ifndef RELEASE
#error "scsi/scsi.h:RELEASE macro is missing from libc-shim"
#endif

#ifndef RELEASE_10
#error "scsi/scsi.h:RELEASE_10 macro is missing from libc-shim"
#endif

#ifndef RELEASE_RECOVERY
#error "scsi/scsi.h:RELEASE_RECOVERY macro is missing from libc-shim"
#endif

#ifndef REQUEST_SENSE
#error "scsi/scsi.h:REQUEST_SENSE macro is missing from libc-shim"
#endif

#ifndef RESERVATION_CONFLICT
#error "scsi/scsi.h:RESERVATION_CONFLICT macro is missing from libc-shim"
#endif

#ifndef RESERVE
#error "scsi/scsi.h:RESERVE macro is missing from libc-shim"
#endif

#ifndef RESERVE_10
#error "scsi/scsi.h:RESERVE_10 macro is missing from libc-shim"
#endif

#ifndef RESTORE_POINTERS
#error "scsi/scsi.h:RESTORE_POINTERS macro is missing from libc-shim"
#endif

#ifndef REZERO_UNIT
#error "scsi/scsi.h:REZERO_UNIT macro is missing from libc-shim"
#endif

#ifndef SAVE_POINTERS
#error "scsi/scsi.h:SAVE_POINTERS macro is missing from libc-shim"
#endif

#ifndef SCSI_IOCTL_GET_BUS_NUMBER
#error "scsi/scsi.h:SCSI_IOCTL_GET_BUS_NUMBER macro is missing from libc-shim"
#endif

#ifndef SCSI_IOCTL_GET_IDLUN
#error "scsi/scsi.h:SCSI_IOCTL_GET_IDLUN macro is missing from libc-shim"
#endif

#ifndef SCSI_IOCTL_PROBE_HOST
#error "scsi/scsi.h:SCSI_IOCTL_PROBE_HOST macro is missing from libc-shim"
#endif

#ifndef SCSI_IOCTL_TAGGED_DISABLE
#error "scsi/scsi.h:SCSI_IOCTL_TAGGED_DISABLE macro is missing from libc-shim"
#endif

#ifndef SCSI_IOCTL_TAGGED_ENABLE
#error "scsi/scsi.h:SCSI_IOCTL_TAGGED_ENABLE macro is missing from libc-shim"
#endif

#ifndef SEARCH_EQUAL
#error "scsi/scsi.h:SEARCH_EQUAL macro is missing from libc-shim"
#endif

#ifndef SEARCH_EQUAL_12
#error "scsi/scsi.h:SEARCH_EQUAL_12 macro is missing from libc-shim"
#endif

#ifndef SEARCH_HIGH
#error "scsi/scsi.h:SEARCH_HIGH macro is missing from libc-shim"
#endif

#ifndef SEARCH_HIGH_12
#error "scsi/scsi.h:SEARCH_HIGH_12 macro is missing from libc-shim"
#endif

#ifndef SEARCH_LOW
#error "scsi/scsi.h:SEARCH_LOW macro is missing from libc-shim"
#endif

#ifndef SEARCH_LOW_12
#error "scsi/scsi.h:SEARCH_LOW_12 macro is missing from libc-shim"
#endif

#ifndef SEEK_10
#error "scsi/scsi.h:SEEK_10 macro is missing from libc-shim"
#endif

#ifndef SEEK_6
#error "scsi/scsi.h:SEEK_6 macro is missing from libc-shim"
#endif

#ifndef SEND_DIAGNOSTIC
#error "scsi/scsi.h:SEND_DIAGNOSTIC macro is missing from libc-shim"
#endif

#ifndef SEND_VOLUME_TAG
#error "scsi/scsi.h:SEND_VOLUME_TAG macro is missing from libc-shim"
#endif

#ifndef SET_LIMITS
#error "scsi/scsi.h:SET_LIMITS macro is missing from libc-shim"
#endif

#ifndef SET_WINDOW
#error "scsi/scsi.h:SET_WINDOW macro is missing from libc-shim"
#endif

#ifndef SIMPLE_QUEUE_TAG
#error "scsi/scsi.h:SIMPLE_QUEUE_TAG macro is missing from libc-shim"
#endif

#ifndef SPACE
#error "scsi/scsi.h:SPACE macro is missing from libc-shim"
#endif

#ifndef START_STOP
#error "scsi/scsi.h:START_STOP macro is missing from libc-shim"
#endif

#ifndef STATUS_MASK
#error "scsi/scsi.h:STATUS_MASK macro is missing from libc-shim"
#endif

#ifndef SYNCHRONIZE_CACHE
#error "scsi/scsi.h:SYNCHRONIZE_CACHE macro is missing from libc-shim"
#endif

#ifndef TEST_UNIT_READY
#error "scsi/scsi.h:TEST_UNIT_READY macro is missing from libc-shim"
#endif

#ifndef TYPE_DISK
#error "scsi/scsi.h:TYPE_DISK macro is missing from libc-shim"
#endif

#ifndef TYPE_ENCLOSURE
#error "scsi/scsi.h:TYPE_ENCLOSURE macro is missing from libc-shim"
#endif

#ifndef TYPE_MEDIUM_CHANGER
#error "scsi/scsi.h:TYPE_MEDIUM_CHANGER macro is missing from libc-shim"
#endif

#ifndef TYPE_MOD
#error "scsi/scsi.h:TYPE_MOD macro is missing from libc-shim"
#endif

#ifndef TYPE_NO_LUN
#error "scsi/scsi.h:TYPE_NO_LUN macro is missing from libc-shim"
#endif

#ifndef TYPE_PROCESSOR
#error "scsi/scsi.h:TYPE_PROCESSOR macro is missing from libc-shim"
#endif

#ifndef TYPE_ROM
#error "scsi/scsi.h:TYPE_ROM macro is missing from libc-shim"
#endif

#ifndef TYPE_SCANNER
#error "scsi/scsi.h:TYPE_SCANNER macro is missing from libc-shim"
#endif

#ifndef TYPE_TAPE
#error "scsi/scsi.h:TYPE_TAPE macro is missing from libc-shim"
#endif

#ifndef TYPE_WORM
#error "scsi/scsi.h:TYPE_WORM macro is missing from libc-shim"
#endif

#ifndef UNIT_ATTENTION
#error "scsi/scsi.h:UNIT_ATTENTION macro is missing from libc-shim"
#endif

#ifndef UPDATE_BLOCK
#error "scsi/scsi.h:UPDATE_BLOCK macro is missing from libc-shim"
#endif

#ifndef VERIFY
#error "scsi/scsi.h:VERIFY macro is missing from libc-shim"
#endif

#ifndef VOLUME_OVERFLOW
#error "scsi/scsi.h:VOLUME_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef WRITE_10
#error "scsi/scsi.h:WRITE_10 macro is missing from libc-shim"
#endif

#ifndef WRITE_12
#error "scsi/scsi.h:WRITE_12 macro is missing from libc-shim"
#endif

#ifndef WRITE_6
#error "scsi/scsi.h:WRITE_6 macro is missing from libc-shim"
#endif

#ifndef WRITE_BUFFER
#error "scsi/scsi.h:WRITE_BUFFER macro is missing from libc-shim"
#endif

#ifndef WRITE_FILEMARKS
#error "scsi/scsi.h:WRITE_FILEMARKS macro is missing from libc-shim"
#endif

#ifndef WRITE_LONG
#error "scsi/scsi.h:WRITE_LONG macro is missing from libc-shim"
#endif

#ifndef WRITE_LONG_2
#error "scsi/scsi.h:WRITE_LONG_2 macro is missing from libc-shim"
#endif

#ifndef WRITE_SAME
#error "scsi/scsi.h:WRITE_SAME macro is missing from libc-shim"
#endif

#ifndef WRITE_VERIFY
#error "scsi/scsi.h:WRITE_VERIFY macro is missing from libc-shim"
#endif

#ifndef WRITE_VERIFY_12
#error "scsi/scsi.h:WRITE_VERIFY_12 macro is missing from libc-shim"
#endif

int main(void) { return 0; }
