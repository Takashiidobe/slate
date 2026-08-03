#undef assert

#define __STDC_VERSION_ASSERT_H__ 202311L

#ifdef NDEBUG
#define assert(...) ((void)0)
#else
extern _Noreturn void abort(void);
#define assert(...) ((void)((__VA_ARGS__) ? 0 : (abort(), 0)))
#endif
