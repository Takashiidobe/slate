#include <net/if_shaper.h>

_Static_assert(__builtin_offsetof(struct shaperconf, ss_cmd) == 0, "struct shaperconf.ss_cmd offset differs from oracle");

typedef unsigned short slate_oracle_struct_shaperconf_ss_cmd;
_Static_assert(__builtin_types_compatible_p(__typeof__((( struct shaperconf *)0)->ss_cmd), slate_oracle_struct_shaperconf_ss_cmd), "struct shaperconf.ss_cmd field type differs from oracle");

#ifndef SHAPER_BURST
#error "net/if_shaper.h:SHAPER_BURST macro is missing from libc-shim"
#endif

#ifndef SHAPER_GET_DEV
#error "net/if_shaper.h:SHAPER_GET_DEV macro is missing from libc-shim"
#endif

#ifndef SHAPER_GET_SPEED
#error "net/if_shaper.h:SHAPER_GET_SPEED macro is missing from libc-shim"
#endif

#ifndef SHAPER_LATENCY
#error "net/if_shaper.h:SHAPER_LATENCY macro is missing from libc-shim"
#endif

#ifndef SHAPER_MAXSLIP
#error "net/if_shaper.h:SHAPER_MAXSLIP macro is missing from libc-shim"
#endif

#ifndef SHAPER_QLEN
#error "net/if_shaper.h:SHAPER_QLEN macro is missing from libc-shim"
#endif

#ifndef SHAPER_SET_DEV
#error "net/if_shaper.h:SHAPER_SET_DEV macro is missing from libc-shim"
#endif

#ifndef SHAPER_SET_SPEED
#error "net/if_shaper.h:SHAPER_SET_SPEED macro is missing from libc-shim"
#endif

#ifndef ss_name
#error "net/if_shaper.h:ss_name macro is missing from libc-shim"
#endif

#ifndef ss_speed
#error "net/if_shaper.h:ss_speed macro is missing from libc-shim"
#endif

int main(void) { return 0; }
