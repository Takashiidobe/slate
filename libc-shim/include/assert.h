#include <features.h>
#define __STDC_VERSION_ASSERT_H__ 202311L

#undef assert

#ifdef NDEBUG
#define assert(x) (void)0
#else
#define assert(x)                                                              \
  ((void)((x) || (__assert_fail(#x, __FILE__, __LINE__, __func__), 0)))
#endif

#if __STDC_VERSION__ >= 201112L
#define static_assert _Static_assert
#endif

_Noreturn void __assert_fail(const char *, const char *, int, const char *);
