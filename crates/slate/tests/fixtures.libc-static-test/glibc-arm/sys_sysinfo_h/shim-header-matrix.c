#include <sys/sysinfo.h>

extern int slate_oracle_sysinfo(struct sysinfo *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sysinfo), __typeof__(sysinfo)),
    "sys/sysinfo.h:sysinfo declaration differs from oracle");

static __typeof__(sysinfo) *const slate_reference_sysinfo = &sysinfo;

int main(void) { return 0; }
