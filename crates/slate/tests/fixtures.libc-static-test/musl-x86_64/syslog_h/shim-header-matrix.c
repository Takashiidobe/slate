#include <syslog.h>

extern void slate_oracle_closelog(void);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_closelog), __typeof__(closelog)),
    "syslog.h:closelog declaration differs from oracle");

static __typeof__(closelog) *const slate_reference_closelog = &closelog;

extern void slate_oracle_vsyslog(int, const char *, struct __va_list_tag *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_vsyslog), __typeof__(vsyslog)),
    "syslog.h:vsyslog declaration differs from oracle");

static __typeof__(vsyslog) *const slate_reference_vsyslog = &vsyslog;

#ifndef LOG_ALERT
#error "syslog.h:LOG_ALERT macro is missing from libc-shim"
#endif

#ifndef LOG_AUTH
#error "syslog.h:LOG_AUTH macro is missing from libc-shim"
#endif

#ifndef LOG_AUTHPRIV
#error "syslog.h:LOG_AUTHPRIV macro is missing from libc-shim"
#endif

#ifndef LOG_CONS
#error "syslog.h:LOG_CONS macro is missing from libc-shim"
#endif

#ifndef LOG_CRIT
#error "syslog.h:LOG_CRIT macro is missing from libc-shim"
#endif

#ifndef LOG_CRON
#error "syslog.h:LOG_CRON macro is missing from libc-shim"
#endif

#ifndef LOG_DAEMON
#error "syslog.h:LOG_DAEMON macro is missing from libc-shim"
#endif

#ifndef LOG_DEBUG
#error "syslog.h:LOG_DEBUG macro is missing from libc-shim"
#endif

#ifndef LOG_EMERG
#error "syslog.h:LOG_EMERG macro is missing from libc-shim"
#endif

#ifndef LOG_ERR
#error "syslog.h:LOG_ERR macro is missing from libc-shim"
#endif

#ifndef LOG_FAC
#error "syslog.h:LOG_FAC macro is missing from libc-shim"
#endif

#ifndef LOG_FACMASK
#error "syslog.h:LOG_FACMASK macro is missing from libc-shim"
#endif

#ifndef LOG_FTP
#error "syslog.h:LOG_FTP macro is missing from libc-shim"
#endif

#ifndef LOG_INFO
#error "syslog.h:LOG_INFO macro is missing from libc-shim"
#endif

#ifndef LOG_KERN
#error "syslog.h:LOG_KERN macro is missing from libc-shim"
#endif

#ifndef LOG_LOCAL0
#error "syslog.h:LOG_LOCAL0 macro is missing from libc-shim"
#endif

#ifndef LOG_LOCAL1
#error "syslog.h:LOG_LOCAL1 macro is missing from libc-shim"
#endif

#ifndef LOG_LOCAL2
#error "syslog.h:LOG_LOCAL2 macro is missing from libc-shim"
#endif

#ifndef LOG_LOCAL3
#error "syslog.h:LOG_LOCAL3 macro is missing from libc-shim"
#endif

#ifndef LOG_LOCAL4
#error "syslog.h:LOG_LOCAL4 macro is missing from libc-shim"
#endif

#ifndef LOG_LOCAL5
#error "syslog.h:LOG_LOCAL5 macro is missing from libc-shim"
#endif

#ifndef LOG_LOCAL6
#error "syslog.h:LOG_LOCAL6 macro is missing from libc-shim"
#endif

#ifndef LOG_LOCAL7
#error "syslog.h:LOG_LOCAL7 macro is missing from libc-shim"
#endif

#ifndef LOG_LPR
#error "syslog.h:LOG_LPR macro is missing from libc-shim"
#endif

#ifndef LOG_MAIL
#error "syslog.h:LOG_MAIL macro is missing from libc-shim"
#endif

#ifndef LOG_MAKEPRI
#error "syslog.h:LOG_MAKEPRI macro is missing from libc-shim"
#endif

#ifndef LOG_MASK
#error "syslog.h:LOG_MASK macro is missing from libc-shim"
#endif

#ifndef LOG_NDELAY
#error "syslog.h:LOG_NDELAY macro is missing from libc-shim"
#endif

#ifndef LOG_NEWS
#error "syslog.h:LOG_NEWS macro is missing from libc-shim"
#endif

#ifndef LOG_NFACILITIES
#error "syslog.h:LOG_NFACILITIES macro is missing from libc-shim"
#endif

#ifndef LOG_NOTICE
#error "syslog.h:LOG_NOTICE macro is missing from libc-shim"
#endif

#ifndef LOG_NOWAIT
#error "syslog.h:LOG_NOWAIT macro is missing from libc-shim"
#endif

#ifndef LOG_ODELAY
#error "syslog.h:LOG_ODELAY macro is missing from libc-shim"
#endif

#ifndef LOG_PERROR
#error "syslog.h:LOG_PERROR macro is missing from libc-shim"
#endif

#ifndef LOG_PID
#error "syslog.h:LOG_PID macro is missing from libc-shim"
#endif

#ifndef LOG_PRI
#error "syslog.h:LOG_PRI macro is missing from libc-shim"
#endif

#ifndef LOG_PRIMASK
#error "syslog.h:LOG_PRIMASK macro is missing from libc-shim"
#endif

#ifndef LOG_SYSLOG
#error "syslog.h:LOG_SYSLOG macro is missing from libc-shim"
#endif

#ifndef LOG_UPTO
#error "syslog.h:LOG_UPTO macro is missing from libc-shim"
#endif

#ifndef LOG_USER
#error "syslog.h:LOG_USER macro is missing from libc-shim"
#endif

#ifndef LOG_UUCP
#error "syslog.h:LOG_UUCP macro is missing from libc-shim"
#endif

#ifndef LOG_WARNING
#error "syslog.h:LOG_WARNING macro is missing from libc-shim"
#endif

int main(void) { return 0; }
