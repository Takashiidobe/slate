#include <pty.h>

extern int slate_oracle_openpty(int *, int *, char *, const struct termios *, const struct winsize *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_openpty), __typeof__(openpty)),
    "pty.h:openpty declaration differs from oracle");

static __typeof__(openpty) *const slate_reference_openpty = &openpty;

int main(void) { return 0; }
