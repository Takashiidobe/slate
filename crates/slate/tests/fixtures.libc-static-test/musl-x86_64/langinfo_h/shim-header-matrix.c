#include <langinfo.h>

extern char * slate_oracle_nl_langinfo(int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nl_langinfo), __typeof__(nl_langinfo)),
    "langinfo.h:nl_langinfo declaration differs from oracle");

static __typeof__(nl_langinfo) *const slate_reference_nl_langinfo = &nl_langinfo;

#ifndef ABDAY_1
#error "langinfo.h:ABDAY_1 macro is missing from libc-shim"
#endif

#ifndef ABDAY_2
#error "langinfo.h:ABDAY_2 macro is missing from libc-shim"
#endif

#ifndef ABDAY_3
#error "langinfo.h:ABDAY_3 macro is missing from libc-shim"
#endif

#ifndef ABDAY_4
#error "langinfo.h:ABDAY_4 macro is missing from libc-shim"
#endif

#ifndef ABDAY_5
#error "langinfo.h:ABDAY_5 macro is missing from libc-shim"
#endif

#ifndef ABDAY_6
#error "langinfo.h:ABDAY_6 macro is missing from libc-shim"
#endif

#ifndef ABDAY_7
#error "langinfo.h:ABDAY_7 macro is missing from libc-shim"
#endif

#ifndef ABMON_1
#error "langinfo.h:ABMON_1 macro is missing from libc-shim"
#endif

#ifndef ABMON_10
#error "langinfo.h:ABMON_10 macro is missing from libc-shim"
#endif

#ifndef ABMON_11
#error "langinfo.h:ABMON_11 macro is missing from libc-shim"
#endif

#ifndef ABMON_12
#error "langinfo.h:ABMON_12 macro is missing from libc-shim"
#endif

#ifndef ABMON_2
#error "langinfo.h:ABMON_2 macro is missing from libc-shim"
#endif

#ifndef ABMON_3
#error "langinfo.h:ABMON_3 macro is missing from libc-shim"
#endif

#ifndef ABMON_4
#error "langinfo.h:ABMON_4 macro is missing from libc-shim"
#endif

#ifndef ABMON_5
#error "langinfo.h:ABMON_5 macro is missing from libc-shim"
#endif

#ifndef ABMON_6
#error "langinfo.h:ABMON_6 macro is missing from libc-shim"
#endif

#ifndef ABMON_7
#error "langinfo.h:ABMON_7 macro is missing from libc-shim"
#endif

#ifndef ABMON_8
#error "langinfo.h:ABMON_8 macro is missing from libc-shim"
#endif

#ifndef ABMON_9
#error "langinfo.h:ABMON_9 macro is missing from libc-shim"
#endif

#ifndef ALT_DIGITS
#error "langinfo.h:ALT_DIGITS macro is missing from libc-shim"
#endif

#ifndef AM_STR
#error "langinfo.h:AM_STR macro is missing from libc-shim"
#endif

#ifndef CODESET
#error "langinfo.h:CODESET macro is missing from libc-shim"
#endif

#ifndef CRNCYSTR
#error "langinfo.h:CRNCYSTR macro is missing from libc-shim"
#endif

#ifndef DAY_1
#error "langinfo.h:DAY_1 macro is missing from libc-shim"
#endif

#ifndef DAY_2
#error "langinfo.h:DAY_2 macro is missing from libc-shim"
#endif

#ifndef DAY_3
#error "langinfo.h:DAY_3 macro is missing from libc-shim"
#endif

#ifndef DAY_4
#error "langinfo.h:DAY_4 macro is missing from libc-shim"
#endif

#ifndef DAY_5
#error "langinfo.h:DAY_5 macro is missing from libc-shim"
#endif

#ifndef DAY_6
#error "langinfo.h:DAY_6 macro is missing from libc-shim"
#endif

#ifndef DAY_7
#error "langinfo.h:DAY_7 macro is missing from libc-shim"
#endif

#ifndef D_FMT
#error "langinfo.h:D_FMT macro is missing from libc-shim"
#endif

#ifndef D_T_FMT
#error "langinfo.h:D_T_FMT macro is missing from libc-shim"
#endif

#ifndef ERA
#error "langinfo.h:ERA macro is missing from libc-shim"
#endif

#ifndef ERA_D_FMT
#error "langinfo.h:ERA_D_FMT macro is missing from libc-shim"
#endif

#ifndef ERA_D_T_FMT
#error "langinfo.h:ERA_D_T_FMT macro is missing from libc-shim"
#endif

#ifndef ERA_T_FMT
#error "langinfo.h:ERA_T_FMT macro is missing from libc-shim"
#endif

#ifndef MON_1
#error "langinfo.h:MON_1 macro is missing from libc-shim"
#endif

#ifndef MON_10
#error "langinfo.h:MON_10 macro is missing from libc-shim"
#endif

#ifndef MON_11
#error "langinfo.h:MON_11 macro is missing from libc-shim"
#endif

#ifndef MON_12
#error "langinfo.h:MON_12 macro is missing from libc-shim"
#endif

#ifndef MON_2
#error "langinfo.h:MON_2 macro is missing from libc-shim"
#endif

#ifndef MON_3
#error "langinfo.h:MON_3 macro is missing from libc-shim"
#endif

#ifndef MON_4
#error "langinfo.h:MON_4 macro is missing from libc-shim"
#endif

#ifndef MON_5
#error "langinfo.h:MON_5 macro is missing from libc-shim"
#endif

#ifndef MON_6
#error "langinfo.h:MON_6 macro is missing from libc-shim"
#endif

#ifndef MON_7
#error "langinfo.h:MON_7 macro is missing from libc-shim"
#endif

#ifndef MON_8
#error "langinfo.h:MON_8 macro is missing from libc-shim"
#endif

#ifndef MON_9
#error "langinfo.h:MON_9 macro is missing from libc-shim"
#endif

#ifndef NL_LOCALE_NAME
#error "langinfo.h:NL_LOCALE_NAME macro is missing from libc-shim"
#endif

#ifndef NOEXPR
#error "langinfo.h:NOEXPR macro is missing from libc-shim"
#endif

#ifndef NOSTR
#error "langinfo.h:NOSTR macro is missing from libc-shim"
#endif

#ifndef PM_STR
#error "langinfo.h:PM_STR macro is missing from libc-shim"
#endif

#ifndef RADIXCHAR
#error "langinfo.h:RADIXCHAR macro is missing from libc-shim"
#endif

#ifndef THOUSEP
#error "langinfo.h:THOUSEP macro is missing from libc-shim"
#endif

#ifndef T_FMT
#error "langinfo.h:T_FMT macro is missing from libc-shim"
#endif

#ifndef T_FMT_AMPM
#error "langinfo.h:T_FMT_AMPM macro is missing from libc-shim"
#endif

#ifndef YESEXPR
#error "langinfo.h:YESEXPR macro is missing from libc-shim"
#endif

#ifndef YESSTR
#error "langinfo.h:YESSTR macro is missing from libc-shim"
#endif

int main(void) { return 0; }
