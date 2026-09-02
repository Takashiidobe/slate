#include <stddef.h>
#include <sys/timeb.h>
#include <sys/utime.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(sizeof(struct _utimbuf) == 16, "_utimbuf size");
_Static_assert(_Alignof(struct _utimbuf) == 8, "_utimbuf alignment");
_Static_assert(offsetof(struct _utimbuf, actime) == 0, "_utimbuf actime");
_Static_assert(offsetof(struct _utimbuf, modtime) == 8, "_utimbuf modtime");
TYPE_IS(((struct _utimbuf *)0)->actime, time_t);
TYPE_IS(((struct _utimbuf *)0)->modtime, time_t);

_Static_assert(sizeof(struct __utimbuf32) == 8, "__utimbuf32 size");
_Static_assert(_Alignof(struct __utimbuf32) == 4, "__utimbuf32 alignment");
_Static_assert(offsetof(struct __utimbuf32, modtime) == 4,
               "__utimbuf32 modtime");
TYPE_IS(((struct __utimbuf32 *)0)->actime, __time32_t);
TYPE_IS(((struct __utimbuf32 *)0)->modtime, __time32_t);

_Static_assert(sizeof(struct __utimbuf64) == 16, "__utimbuf64 size");
_Static_assert(_Alignof(struct __utimbuf64) == 8, "__utimbuf64 alignment");
_Static_assert(offsetof(struct __utimbuf64, modtime) == 8,
               "__utimbuf64 modtime");
TYPE_IS(((struct __utimbuf64 *)0)->actime, __time64_t);
TYPE_IS(((struct __utimbuf64 *)0)->modtime, __time64_t);

TYPE_IS(&_utime32, int (*)(const char *, struct __utimbuf32 *));
TYPE_IS(&_futime32, int (*)(int, struct __utimbuf32 *));
TYPE_IS(&_wutime32, int (*)(const wchar_t *, struct __utimbuf32 *));
TYPE_IS(&_utime64, int (*)(const char *, struct __utimbuf64 *));
TYPE_IS(&_futime64, int (*)(int, struct __utimbuf64 *));
TYPE_IS(&_wutime64, int (*)(const wchar_t *, struct __utimbuf64 *));

_Static_assert(sizeof(struct __timeb32) == 12, "__timeb32 size");
_Static_assert(_Alignof(struct __timeb32) == 4, "__timeb32 alignment");
_Static_assert(offsetof(struct __timeb32, time) == 0, "__timeb32 time");
_Static_assert(offsetof(struct __timeb32, millitm) == 4, "__timeb32 millitm");
_Static_assert(offsetof(struct __timeb32, timezone) == 6, "__timeb32 timezone");
_Static_assert(offsetof(struct __timeb32, dstflag) == 8, "__timeb32 dstflag");
TYPE_IS(((struct __timeb32 *)0)->time, __time32_t);
TYPE_IS(((struct __timeb32 *)0)->millitm, unsigned short);
TYPE_IS(((struct __timeb32 *)0)->timezone, short);
TYPE_IS(((struct __timeb32 *)0)->dstflag, short);

_Static_assert(sizeof(struct __timeb64) == 16, "__timeb64 size");
_Static_assert(_Alignof(struct __timeb64) == 8, "__timeb64 alignment");
_Static_assert(offsetof(struct __timeb64, time) == 0, "__timeb64 time");
_Static_assert(offsetof(struct __timeb64, millitm) == 8, "__timeb64 millitm");
_Static_assert(offsetof(struct __timeb64, timezone) == 10,
               "__timeb64 timezone");
_Static_assert(offsetof(struct __timeb64, dstflag) == 12, "__timeb64 dstflag");
TYPE_IS(((struct __timeb64 *)0)->time, __time64_t);
TYPE_IS(((struct __timeb64 *)0)->millitm, unsigned short);
TYPE_IS(((struct __timeb64 *)0)->timezone, short);
TYPE_IS(((struct __timeb64 *)0)->dstflag, short);

TYPE_IS(&_ftime32, void (*)(struct __timeb32 *));
TYPE_IS(&_ftime64, void (*)(struct __timeb64 *));

int main(void) { return 0; }
