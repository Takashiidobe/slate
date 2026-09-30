#include <thread_db.h>

#ifndef BT_NBIPUI
#error "thread_db.h:BT_NBIPUI macro is missing from libc-shim"
#endif

#ifndef BT_UIMASK
#error "thread_db.h:BT_UIMASK macro is missing from libc-shim"
#endif

#ifndef BT_UISHIFT
#error "thread_db.h:BT_UISHIFT macro is missing from libc-shim"
#endif

#ifndef TD_EVENTSIZE
#error "thread_db.h:TD_EVENTSIZE macro is missing from libc-shim"
#endif

#ifndef TD_SIGNO_MASK
#error "thread_db.h:TD_SIGNO_MASK macro is missing from libc-shim"
#endif

#ifndef TD_THR_ANY_USER_FLAGS
#error "thread_db.h:TD_THR_ANY_USER_FLAGS macro is missing from libc-shim"
#endif

#ifndef TD_THR_LOWEST_PRIORITY
#error "thread_db.h:TD_THR_LOWEST_PRIORITY macro is missing from libc-shim"
#endif

#ifndef td_event_addset
#error "thread_db.h:td_event_addset macro is missing from libc-shim"
#endif

#ifndef td_event_delset
#error "thread_db.h:td_event_delset macro is missing from libc-shim"
#endif

#ifndef td_event_emptyset
#error "thread_db.h:td_event_emptyset macro is missing from libc-shim"
#endif

#ifndef td_event_fillset
#error "thread_db.h:td_event_fillset macro is missing from libc-shim"
#endif

#ifndef td_eventisempty
#error "thread_db.h:td_eventisempty macro is missing from libc-shim"
#endif

#ifndef td_eventismember
#error "thread_db.h:td_eventismember macro is missing from libc-shim"
#endif

int main(void) { return 0; }
