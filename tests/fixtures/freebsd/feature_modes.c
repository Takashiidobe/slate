#include <features.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

_Static_assert(__FreeBSD_version == 1501000, "FreeBSD release");

#if defined(EXPECT_FREEBSD_FULL)
_Static_assert(__BSD_VISIBLE == 1, "BSD visibility");
_Static_assert(__POSIX_VISIBLE == 202405, "POSIX visibility");
_Static_assert(__XSI_VISIBLE == 800, "XSI visibility");
void *full_declarations[] = {(void *)strlcpy, (void *)arc4random,
                             (void *)closefrom};
#elif defined(EXPECT_POSIX_2008)
_Static_assert(__BSD_VISIBLE == 0, "BSD hidden");
_Static_assert(__POSIX_VISIBLE == 200809, "POSIX.1-2008 visibility");
_Static_assert(__XSI_VISIBLE == 0, "XSI hidden");
void  strlcpy(void);
void  arc4random(void);
void  closefrom(void);
void *posix_declaration = (void *)posix_memalign;
#elif defined(EXPECT_XSI_700)
_Static_assert(__BSD_VISIBLE == 0, "BSD hidden");
_Static_assert(__POSIX_VISIBLE == 200809, "POSIX.1-2008 visibility");
_Static_assert(__XSI_VISIBLE == 700, "XSI 700 visibility");
void  strlcpy(void);
void  arc4random(void);
void  closefrom(void);
void *xsi_declaration = (void *)random;
#elif defined(EXPECT_ANSI)
_Static_assert(__BSD_VISIBLE == 0, "BSD hidden");
_Static_assert(__POSIX_VISIBLE == 0, "POSIX hidden");
_Static_assert(__XSI_VISIBLE == 0, "XSI hidden");
void strlcpy(void);
void arc4random(void);
void closefrom(void);
void posix_memalign(void);
void random(void);
#else
#error "select a FreeBSD feature mode"
#endif

int main(void) { return 0; }
