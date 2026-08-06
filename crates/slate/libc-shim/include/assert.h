#include <features.h>
#define __STDC_VERSION_ASSERT_H__ 202311L

#undef assert

#ifdef NDEBUG
#define assert(...) (void)0
#else
#define assert(...)                                                            \
  ((void)((__VA_ARGS__) ||                                                     \
          (__assert_fail(#__VA_ARGS__, __FILE__, __LINE__, __func__), 0)))
#endif

#if __STDC_VERSION__ >= 201112L
#define static_assert _Static_assert
#endif

_Noreturn void __assert_fail(const char *, const char *, int, const char *);
