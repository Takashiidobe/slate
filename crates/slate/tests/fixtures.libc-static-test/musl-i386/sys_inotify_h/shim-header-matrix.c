#include <sys/inotify.h>

_Static_assert(sizeof(struct inotify_event) == 16, "struct inotify_event size differs from oracle");

_Static_assert(_Alignof(struct inotify_event) == 4, "struct inotify_event alignment differs from oracle");

_Static_assert(__builtin_offsetof(struct inotify_event, wd) == 0, "struct inotify_event.wd offset differs from oracle");

typedef int slate_oracle_struct_inotify_event_wd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct inotify_event *)0)->wd), slate_oracle_struct_inotify_event_wd), "struct inotify_event.wd field type differs from oracle");

_Static_assert(__builtin_offsetof(struct inotify_event, mask) == 4, "struct inotify_event.mask offset differs from oracle");

typedef unsigned int slate_oracle_struct_inotify_event_mask;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct inotify_event *)0)->mask), slate_oracle_struct_inotify_event_mask), "struct inotify_event.mask field type differs from oracle");

_Static_assert(__builtin_offsetof(struct inotify_event, cookie) == 8, "struct inotify_event.cookie offset differs from oracle");

typedef unsigned int slate_oracle_struct_inotify_event_cookie;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct inotify_event *)0)->cookie), slate_oracle_struct_inotify_event_cookie), "struct inotify_event.cookie field type differs from oracle");

_Static_assert(__builtin_offsetof(struct inotify_event, len) == 12, "struct inotify_event.len offset differs from oracle");

typedef unsigned int slate_oracle_struct_inotify_event_len;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct inotify_event *)0)->len), slate_oracle_struct_inotify_event_len), "struct inotify_event.len field type differs from oracle");

_Static_assert(__builtin_offsetof(struct inotify_event, name) == 16, "struct inotify_event.name offset differs from oracle");

#ifndef IN_ACCESS
#error "sys/inotify.h:IN_ACCESS macro is missing from libc-shim"
#endif

#ifndef IN_ALL_EVENTS
#error "sys/inotify.h:IN_ALL_EVENTS macro is missing from libc-shim"
#endif

#ifndef IN_ATTRIB
#error "sys/inotify.h:IN_ATTRIB macro is missing from libc-shim"
#endif

#ifndef IN_CLOEXEC
#error "sys/inotify.h:IN_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef IN_CLOSE
#error "sys/inotify.h:IN_CLOSE macro is missing from libc-shim"
#endif

#ifndef IN_CLOSE_NOWRITE
#error "sys/inotify.h:IN_CLOSE_NOWRITE macro is missing from libc-shim"
#endif

#ifndef IN_CLOSE_WRITE
#error "sys/inotify.h:IN_CLOSE_WRITE macro is missing from libc-shim"
#endif

#ifndef IN_CREATE
#error "sys/inotify.h:IN_CREATE macro is missing from libc-shim"
#endif

#ifndef IN_DELETE
#error "sys/inotify.h:IN_DELETE macro is missing from libc-shim"
#endif

#ifndef IN_DELETE_SELF
#error "sys/inotify.h:IN_DELETE_SELF macro is missing from libc-shim"
#endif

#ifndef IN_DONT_FOLLOW
#error "sys/inotify.h:IN_DONT_FOLLOW macro is missing from libc-shim"
#endif

#ifndef IN_EXCL_UNLINK
#error "sys/inotify.h:IN_EXCL_UNLINK macro is missing from libc-shim"
#endif

#ifndef IN_IGNORED
#error "sys/inotify.h:IN_IGNORED macro is missing from libc-shim"
#endif

#ifndef IN_ISDIR
#error "sys/inotify.h:IN_ISDIR macro is missing from libc-shim"
#endif

#ifndef IN_MASK_ADD
#error "sys/inotify.h:IN_MASK_ADD macro is missing from libc-shim"
#endif

#ifndef IN_MASK_CREATE
#error "sys/inotify.h:IN_MASK_CREATE macro is missing from libc-shim"
#endif

#ifndef IN_MODIFY
#error "sys/inotify.h:IN_MODIFY macro is missing from libc-shim"
#endif

#ifndef IN_MOVE
#error "sys/inotify.h:IN_MOVE macro is missing from libc-shim"
#endif

#ifndef IN_MOVED_FROM
#error "sys/inotify.h:IN_MOVED_FROM macro is missing from libc-shim"
#endif

#ifndef IN_MOVED_TO
#error "sys/inotify.h:IN_MOVED_TO macro is missing from libc-shim"
#endif

#ifndef IN_MOVE_SELF
#error "sys/inotify.h:IN_MOVE_SELF macro is missing from libc-shim"
#endif

#ifndef IN_NONBLOCK
#error "sys/inotify.h:IN_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef IN_ONESHOT
#error "sys/inotify.h:IN_ONESHOT macro is missing from libc-shim"
#endif

#ifndef IN_ONLYDIR
#error "sys/inotify.h:IN_ONLYDIR macro is missing from libc-shim"
#endif

#ifndef IN_OPEN
#error "sys/inotify.h:IN_OPEN macro is missing from libc-shim"
#endif

#ifndef IN_Q_OVERFLOW
#error "sys/inotify.h:IN_Q_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef IN_UNMOUNT
#error "sys/inotify.h:IN_UNMOUNT macro is missing from libc-shim"
#endif

int main(void) { return 0; }
