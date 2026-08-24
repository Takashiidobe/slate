#include <features.h>
#include <langinfo.h>
#include <stdio.h>

#if defined(EXPECT_DARWIN_FULL) ||                                             \
    (!defined(EXPECT_POSIX_2001) && !defined(EXPECT_POSIX_2008))
_Static_assert(__DARWIN_C_LEVEL == __DARWIN_C_FULL, "Darwin full level");
#if !defined(D_MD_ORDER) || !defined(YESSTR)
#error "Darwin extensions missing"
#endif
#elif defined(EXPECT_POSIX_2001)
_Static_assert(__DARWIN_C_LEVEL == 200112L, "POSIX.1-2001 level");
#if defined(D_MD_ORDER) || defined(YESSTR)
#error "Darwin extensions leaked into POSIX.1-2001"
#endif
#elif defined(EXPECT_POSIX_2008)
_Static_assert(__DARWIN_C_LEVEL == 200809L, "POSIX.1-2008 level");
#if defined(D_MD_ORDER) || defined(YESSTR)
#error "Darwin extensions leaked into POSIX.1-2008"
#endif
#endif

long feature_level(void) { return __DARWIN_C_LEVEL; }
