#include <features.h>
#define __STDC_VERSION_ASSERT_H__ 202311L

#undef assert

#ifdef NDEBUG
#define assert(...) (void)0
#elif defined(__SLATE_LIBC_MSVC)
#define __SLATE_WIDE_INNER(value) L##value
#define __SLATE_WIDE(value)       __SLATE_WIDE_INNER(value)
#define assert(...)                                                            \
  ((void)((__VA_ARGS__) ||                                                     \
          (_wassert(__SLATE_WIDE(#__VA_ARGS__), __SLATE_WIDE(__FILE__),        \
                    (unsigned)__LINE__),                                       \
           0)))
#elif defined(__SLATE_LIBC_DARWIN)
#define assert(...)                                                            \
  ((void)((__VA_ARGS__) ||                                                     \
          (__assert_rtn(__func__, __FILE__, __LINE__, #__VA_ARGS__), 0)))
#else
#define assert(...)                                                            \
  ((void)((__VA_ARGS__) ||                                                     \
          (__assert_fail(#__VA_ARGS__, __FILE__, __LINE__, __func__), 0)))
#endif

#if __STDC_VERSION__ >= 201112L
#define static_assert _Static_assert
#endif

#if defined(__SLATE_LIBC_MSVC)
void _wassert(const __WCHAR_TYPE__ *, const __WCHAR_TYPE__ *, unsigned int);
#elif defined(__SLATE_LIBC_DARWIN)
_Noreturn void __assert_rtn(const char *, const char *, int, const char *);
#else
_Noreturn void __assert_fail(const char *, const char *, int, const char *);
#endif
