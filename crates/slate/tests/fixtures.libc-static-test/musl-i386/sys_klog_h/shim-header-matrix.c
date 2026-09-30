#include <sys/klog.h>

extern int slate_oracle_klogctl(int, char *, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_klogctl), __typeof__(klogctl)),
    "sys/klog.h:klogctl declaration differs from oracle");

static __typeof__(klogctl) *const slate_reference_klogctl = &klogctl;

int main(void) { return 0; }
