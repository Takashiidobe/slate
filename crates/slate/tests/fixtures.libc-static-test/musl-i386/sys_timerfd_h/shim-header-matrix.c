#include <sys/timerfd.h>

#ifndef TFD_CLOEXEC
#error "sys/timerfd.h:TFD_CLOEXEC macro is missing from libc-shim"
#endif

#ifndef TFD_NONBLOCK
#error "sys/timerfd.h:TFD_NONBLOCK macro is missing from libc-shim"
#endif

#ifndef TFD_TIMER_ABSTIME
#error "sys/timerfd.h:TFD_TIMER_ABSTIME macro is missing from libc-shim"
#endif

#ifndef TFD_TIMER_CANCEL_ON_SET
#error "sys/timerfd.h:TFD_TIMER_CANCEL_ON_SET macro is missing from libc-shim"
#endif

int main(void) { return 0; }
