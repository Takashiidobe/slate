#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

_Static_assert(sizeof(errno_t) == 4, "errno_t");
_Static_assert(sizeof(rsize_t) == 8, "rsize_t");
_Static_assert(sizeof(div_t) == 8, "div_t");
_Static_assert(sizeof(ldiv_t) == 8, "ldiv_t");
_Static_assert(sizeof(lldiv_t) == 16, "lldiv_t");
_Static_assert(sizeof(struct tm) == 36, "struct tm");
_Static_assert(CLOCKS_PER_SEC == 1000, "CLOCKS_PER_SEC");
_Static_assert(EACCES == 13, "EACCES");
_Static_assert(EINVAL == 22, "EINVAL");
_Static_assert(ERANGE == 34, "ERANGE");
_Static_assert(EILSEQ == 42, "EILSEQ");
_Static_assert(EWOULDBLOCK == 140, "EWOULDBLOCK");
_Static_assert(sizeof(sig_atomic_t) == 4, "sig_atomic_t");
_Static_assert(SIGINT == 2, "SIGINT");
_Static_assert(SIGABRT == 22, "SIGABRT");
_Static_assert(SIGABRT_COMPAT == 6, "SIGABRT_COMPAT");

int main(void) { return 0; }
