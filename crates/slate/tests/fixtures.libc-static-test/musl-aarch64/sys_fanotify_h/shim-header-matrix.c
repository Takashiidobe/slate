#include <sys/fanotify.h>

_Static_assert(sizeof(struct fanotify_event_metadata) == 24, "struct fanotify_event_metadata size differs from oracle");

_Static_assert(_Alignof(struct fanotify_event_metadata) == 8, "struct fanotify_event_metadata alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct fanotify_event_metadata, event_len) == 0, "struct fanotify_event_metadata.event_len offset differs from oracle");

typedef unsigned int slate_oracle_struct_fanotify_event_metadata_event_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fanotify_event_metadata *)0)->event_len), slate_oracle_struct_fanotify_event_metadata_event_len), "struct fanotify_event_metadata.event_len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fanotify_event_metadata, vers) == 4, "struct fanotify_event_metadata.vers offset differs from oracle");

typedef unsigned char slate_oracle_struct_fanotify_event_metadata_vers;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fanotify_event_metadata *)0)->vers), slate_oracle_struct_fanotify_event_metadata_vers), "struct fanotify_event_metadata.vers field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fanotify_event_metadata, reserved) == 5, "struct fanotify_event_metadata.reserved offset differs from oracle");

typedef unsigned char slate_oracle_struct_fanotify_event_metadata_reserved;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fanotify_event_metadata *)0)->reserved), slate_oracle_struct_fanotify_event_metadata_reserved), "struct fanotify_event_metadata.reserved field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fanotify_event_metadata, metadata_len) == 6, "struct fanotify_event_metadata.metadata_len offset differs from oracle");

typedef unsigned short slate_oracle_struct_fanotify_event_metadata_metadata_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fanotify_event_metadata *)0)->metadata_len), slate_oracle_struct_fanotify_event_metadata_metadata_len), "struct fanotify_event_metadata.metadata_len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fanotify_event_metadata, mask) == 8, "struct fanotify_event_metadata.mask offset differs from oracle");

typedef unsigned long long slate_oracle_struct_fanotify_event_metadata_mask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fanotify_event_metadata *)0)->mask), slate_oracle_struct_fanotify_event_metadata_mask), "struct fanotify_event_metadata.mask field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fanotify_event_metadata, fd) == 16, "struct fanotify_event_metadata.fd offset differs from oracle");

typedef int slate_oracle_struct_fanotify_event_metadata_fd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fanotify_event_metadata *)0)->fd), slate_oracle_struct_fanotify_event_metadata_fd), "struct fanotify_event_metadata.fd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct fanotify_event_metadata, pid) == 20, "struct fanotify_event_metadata.pid offset differs from oracle");

typedef int slate_oracle_struct_fanotify_event_metadata_pid;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct fanotify_event_metadata *)0)->pid), slate_oracle_struct_fanotify_event_metadata_pid), "struct fanotify_event_metadata.pid field type differs from oracle");

#ifndef FANOTIFY_METADATA_VERSION
#error "sys/fanotify.h:FANOTIFY_METADATA_VERSION macro is missing from libc-shim"
#endif

#ifndef FAN_ACCESS
#error "sys/fanotify.h:FAN_ACCESS macro is missing from libc-shim"
#endif

#ifndef FAN_ACCESS_PERM
#error "sys/fanotify.h:FAN_ACCESS_PERM macro is missing from libc-shim"
#endif

#ifndef FAN_ALLOW
#error "sys/fanotify.h:FAN_ALLOW macro is missing from libc-shim"
#endif

#ifndef FAN_ALL_CLASS_BITS
#error "sys/fanotify.h:FAN_ALL_CLASS_BITS macro is missing from libc-shim"
#endif

#ifndef FAN_ALL_EVENTS
#error "sys/fanotify.h:FAN_ALL_EVENTS macro is missing from libc-shim"
#endif

#ifndef FAN_ALL_INIT_FLAGS
#error "sys/fanotify.h:FAN_ALL_INIT_FLAGS macro is missing from libc-shim"
#endif

#ifndef FAN_ALL_MARK_FLAGS
#error "sys/fanotify.h:FAN_ALL_MARK_FLAGS macro is missing from libc-shim"
#endif

#ifndef FAN_ALL_OUTGOING_EVENTS
#error "sys/fanotify.h:FAN_ALL_OUTGOING_EVENTS macro is missing from libc-shim"
#endif

#ifndef FAN_ALL_PERM_EVENTS
#error "sys/fanotify.h:FAN_ALL_PERM_EVENTS macro is missing from libc-shim"
#endif

#ifndef FAN_ATTRIB
#error "sys/fanotify.h:FAN_ATTRIB macro is missing from libc-shim"
#endif

#ifndef FAN_AUDIT
#error "sys/fanotify.h:FAN_AUDIT macro is missing from libc-shim"
#endif

#ifndef FAN_CLASS_CONTENT
#error "sys/fanotify.h:FAN_CLASS_CONTENT macro is missing from libc-shim"
#endif

#ifndef FAN_CLASS_NOTIF
#error "sys/fanotify.h:FAN_CLASS_NOTIF macro is missing from libc-shim"
#endif

#ifndef FAN_CLASS_PRE_CONTENT
#error "sys/fanotify.h:FAN_CLASS_PRE_CONTENT macro is missing from libc-shim"
#endif

#ifndef FAN_CLOEXEC
#error "sys/fanotify.h:FAN_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef FAN_CLOSE
#error "sys/fanotify.h:FAN_CLOSE macro is missing from libc-shim"
#endif

#ifndef FAN_CLOSE_NOWRITE
#error "sys/fanotify.h:FAN_CLOSE_NOWRITE macro is missing from libc-shim"
#endif

#ifndef FAN_CLOSE_WRITE
#error "sys/fanotify.h:FAN_CLOSE_WRITE macro is missing from libc-shim"
#endif

#ifndef FAN_CREATE
#error "sys/fanotify.h:FAN_CREATE macro is missing from libc-shim"
#endif

#ifndef FAN_DELETE
#error "sys/fanotify.h:FAN_DELETE macro is missing from libc-shim"
#endif

#ifndef FAN_DELETE_SELF
#error "sys/fanotify.h:FAN_DELETE_SELF macro is missing from libc-shim"
#endif

#ifndef FAN_DENY
#error "sys/fanotify.h:FAN_DENY macro is missing from libc-shim"
#endif

#ifndef FAN_DIR_MODIFY
#error "sys/fanotify.h:FAN_DIR_MODIFY macro is missing from libc-shim"
#endif

#ifndef FAN_ENABLE_AUDIT
#error "sys/fanotify.h:FAN_ENABLE_AUDIT macro is missing from libc-shim"
#endif

#ifndef FAN_EVENT_INFO_TYPE_DFID
#error "sys/fanotify.h:FAN_EVENT_INFO_TYPE_DFID macro is missing from libc-shim"
#endif

#ifndef FAN_EVENT_INFO_TYPE_DFID_NAME
#error "sys/fanotify.h:FAN_EVENT_INFO_TYPE_DFID_NAME macro is missing from libc-shim"
#endif

#ifndef FAN_EVENT_INFO_TYPE_FID
#error "sys/fanotify.h:FAN_EVENT_INFO_TYPE_FID macro is missing from libc-shim"
#endif

#ifndef FAN_EVENT_METADATA_LEN
#error "sys/fanotify.h:FAN_EVENT_METADATA_LEN macro is missing from libc-shim"
#endif

#ifndef FAN_EVENT_NEXT
#error "sys/fanotify.h:FAN_EVENT_NEXT macro is missing from libc-shim"
#endif

#ifndef FAN_EVENT_OK
#error "sys/fanotify.h:FAN_EVENT_OK macro is missing from libc-shim"
#endif

#ifndef FAN_EVENT_ON_CHILD
#error "sys/fanotify.h:FAN_EVENT_ON_CHILD macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_ADD
#error "sys/fanotify.h:FAN_MARK_ADD macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_DONT_FOLLOW
#error "sys/fanotify.h:FAN_MARK_DONT_FOLLOW macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_FILESYSTEM
#error "sys/fanotify.h:FAN_MARK_FILESYSTEM macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_FLUSH
#error "sys/fanotify.h:FAN_MARK_FLUSH macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_IGNORED_MASK
#error "sys/fanotify.h:FAN_MARK_IGNORED_MASK macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_IGNORED_SURV_MODIFY
#error "sys/fanotify.h:FAN_MARK_IGNORED_SURV_MODIFY macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_INODE
#error "sys/fanotify.h:FAN_MARK_INODE macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_MOUNT
#error "sys/fanotify.h:FAN_MARK_MOUNT macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_ONLYDIR
#error "sys/fanotify.h:FAN_MARK_ONLYDIR macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_REMOVE
#error "sys/fanotify.h:FAN_MARK_REMOVE macro is missing from libc-shim"
#endif

#ifndef FAN_MARK_TYPE_MASK
#error "sys/fanotify.h:FAN_MARK_TYPE_MASK macro is missing from libc-shim"
#endif

#ifndef FAN_MODIFY
#error "sys/fanotify.h:FAN_MODIFY macro is missing from libc-shim"
#endif

#ifndef FAN_MOVE
#error "sys/fanotify.h:FAN_MOVE macro is missing from libc-shim"
#endif

#ifndef FAN_MOVED_FROM
#error "sys/fanotify.h:FAN_MOVED_FROM macro is missing from libc-shim"
#endif

#ifndef FAN_MOVED_TO
#error "sys/fanotify.h:FAN_MOVED_TO macro is missing from libc-shim"
#endif

#ifndef FAN_MOVE_SELF
#error "sys/fanotify.h:FAN_MOVE_SELF macro is missing from libc-shim"
#endif

#ifndef FAN_NOFD
#error "sys/fanotify.h:FAN_NOFD macro is missing from libc-shim"
#endif

#ifndef FAN_NONBLOCK
#error "sys/fanotify.h:FAN_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef FAN_ONDIR
#error "sys/fanotify.h:FAN_ONDIR macro is missing from libc-shim"
#endif

#ifndef FAN_OPEN
#error "sys/fanotify.h:FAN_OPEN macro is missing from libc-shim"
#endif

#ifndef FAN_OPEN_EXEC
#error "sys/fanotify.h:FAN_OPEN_EXEC macro is missing from libc-shim"
#endif

#ifndef FAN_OPEN_EXEC_PERM
#error "sys/fanotify.h:FAN_OPEN_EXEC_PERM macro is missing from libc-shim"
#endif

#ifndef FAN_OPEN_PERM
#error "sys/fanotify.h:FAN_OPEN_PERM macro is missing from libc-shim"
#endif

#ifndef FAN_Q_OVERFLOW
#error "sys/fanotify.h:FAN_Q_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef FAN_REPORT_DFID_NAME
#error "sys/fanotify.h:FAN_REPORT_DFID_NAME macro is missing from libc-shim"
#endif

#ifndef FAN_REPORT_DIR_FID
#error "sys/fanotify.h:FAN_REPORT_DIR_FID macro is missing from libc-shim"
#endif

#ifndef FAN_REPORT_FID
#error "sys/fanotify.h:FAN_REPORT_FID macro is missing from libc-shim"
#endif

#ifndef FAN_REPORT_NAME
#error "sys/fanotify.h:FAN_REPORT_NAME macro is missing from libc-shim"
#endif

#ifndef FAN_REPORT_TID
#error "sys/fanotify.h:FAN_REPORT_TID macro is missing from libc-shim"
#endif

#ifndef FAN_UNLIMITED_MARKS
#error "sys/fanotify.h:FAN_UNLIMITED_MARKS macro is missing from libc-shim"
#endif

#ifndef FAN_UNLIMITED_QUEUE
#error "sys/fanotify.h:FAN_UNLIMITED_QUEUE macro is missing from libc-shim"
#endif

int main(void) { return 0; }
