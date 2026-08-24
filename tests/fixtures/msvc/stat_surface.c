#include <stddef.h>
#include <sys/stat.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                              \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(sizeof(_dev_t) == 4, "_dev_t size");
_Static_assert((_dev_t)-1 > 0, "_dev_t signedness");
_Static_assert(sizeof(_ino_t) == 2, "_ino_t size");
_Static_assert((_ino_t)-1 > 0, "_ino_t signedness");
_Static_assert(sizeof(_off_t) == 4, "_off_t size");
_Static_assert((_off_t)-1 < 0, "_off_t signedness");

#define CHECK_COMMON(tag)                                                      \
  TYPE_IS(((struct tag *)0)->st_dev, _dev_t);                                  \
  TYPE_IS(((struct tag *)0)->st_ino, _ino_t);                                  \
  TYPE_IS(((struct tag *)0)->st_mode, unsigned short);                         \
  TYPE_IS(((struct tag *)0)->st_nlink, short);                                 \
  TYPE_IS(((struct tag *)0)->st_uid, short);                                   \
  TYPE_IS(((struct tag *)0)->st_gid, short);                                   \
  TYPE_IS(((struct tag *)0)->st_rdev, _dev_t);                                 \
  _Static_assert(offsetof(struct tag, st_dev) == 0, #tag " dev");             \
  _Static_assert(offsetof(struct tag, st_ino) == 4, #tag " ino");             \
  _Static_assert(offsetof(struct tag, st_mode) == 6, #tag " mode");           \
  _Static_assert(offsetof(struct tag, st_nlink) == 8, #tag " nlink");         \
  _Static_assert(offsetof(struct tag, st_uid) == 10, #tag " uid");            \
  _Static_assert(offsetof(struct tag, st_gid) == 12, #tag " gid");            \
  _Static_assert(offsetof(struct tag, st_rdev) == 16, #tag " rdev")

CHECK_COMMON(_stat32);
CHECK_COMMON(_stat32i64);
CHECK_COMMON(_stat64i32);
CHECK_COMMON(_stat64);

_Static_assert(sizeof(struct _stat32) == 36, "_stat32 size");
_Static_assert(_Alignof(struct _stat32) == 4, "_stat32 alignment");
_Static_assert(offsetof(struct _stat32, st_size) == 20, "_stat32 size field");
_Static_assert(offsetof(struct _stat32, st_atime) == 24, "_stat32 atime");
_Static_assert(offsetof(struct _stat32, st_mtime) == 28, "_stat32 mtime");
_Static_assert(offsetof(struct _stat32, st_ctime) == 32, "_stat32 ctime");
TYPE_IS(((struct _stat32 *)0)->st_size, _off_t);
TYPE_IS(((struct _stat32 *)0)->st_atime, __time32_t);
TYPE_IS(((struct _stat32 *)0)->st_mtime, __time32_t);
TYPE_IS(((struct _stat32 *)0)->st_ctime, __time32_t);

_Static_assert(sizeof(struct _stat32i64) == 48, "_stat32i64 size");
_Static_assert(_Alignof(struct _stat32i64) == 8, "_stat32i64 alignment");
_Static_assert(offsetof(struct _stat32i64, st_size) == 24,
               "_stat32i64 size field");
_Static_assert(offsetof(struct _stat32i64, st_atime) == 32,
               "_stat32i64 atime");
_Static_assert(offsetof(struct _stat32i64, st_mtime) == 36,
               "_stat32i64 mtime");
_Static_assert(offsetof(struct _stat32i64, st_ctime) == 40,
               "_stat32i64 ctime");
TYPE_IS(((struct _stat32i64 *)0)->st_size, long long);
TYPE_IS(((struct _stat32i64 *)0)->st_atime, __time32_t);
TYPE_IS(((struct _stat32i64 *)0)->st_mtime, __time32_t);
TYPE_IS(((struct _stat32i64 *)0)->st_ctime, __time32_t);

_Static_assert(sizeof(struct _stat64i32) == 48, "_stat64i32 size");
_Static_assert(_Alignof(struct _stat64i32) == 8, "_stat64i32 alignment");
_Static_assert(offsetof(struct _stat64i32, st_size) == 20,
               "_stat64i32 size field");
_Static_assert(offsetof(struct _stat64i32, st_atime) == 24,
               "_stat64i32 atime");
_Static_assert(offsetof(struct _stat64i32, st_mtime) == 32,
               "_stat64i32 mtime");
_Static_assert(offsetof(struct _stat64i32, st_ctime) == 40,
               "_stat64i32 ctime");
TYPE_IS(((struct _stat64i32 *)0)->st_size, _off_t);
TYPE_IS(((struct _stat64i32 *)0)->st_atime, __time64_t);
TYPE_IS(((struct _stat64i32 *)0)->st_mtime, __time64_t);
TYPE_IS(((struct _stat64i32 *)0)->st_ctime, __time64_t);

_Static_assert(sizeof(struct _stat64) == 56, "_stat64 size");
_Static_assert(_Alignof(struct _stat64) == 8, "_stat64 alignment");
_Static_assert(offsetof(struct _stat64, st_size) == 24, "_stat64 size field");
_Static_assert(offsetof(struct _stat64, st_atime) == 32, "_stat64 atime");
_Static_assert(offsetof(struct _stat64, st_mtime) == 40, "_stat64 mtime");
_Static_assert(offsetof(struct _stat64, st_ctime) == 48, "_stat64 ctime");
TYPE_IS(((struct _stat64 *)0)->st_size, long long);
TYPE_IS(((struct _stat64 *)0)->st_atime, __time64_t);
TYPE_IS(((struct _stat64 *)0)->st_mtime, __time64_t);
TYPE_IS(((struct _stat64 *)0)->st_ctime, __time64_t);

_Static_assert(_S_IFMT == 0xF000, "_S_IFMT");
_Static_assert(_S_IFDIR == 0x4000, "_S_IFDIR");
_Static_assert(_S_IFCHR == 0x2000, "_S_IFCHR");
_Static_assert(_S_IFIFO == 0x1000, "_S_IFIFO");
_Static_assert(_S_IFREG == 0x8000, "_S_IFREG");
_Static_assert(_S_IREAD == 0x0100, "_S_IREAD");
_Static_assert(_S_IWRITE == 0x0080, "_S_IWRITE");
_Static_assert(_S_IEXEC == 0x0040, "_S_IEXEC");

TYPE_IS(&_fstat32, int (*)(int, struct _stat32 *));
TYPE_IS(&_fstat32i64, int (*)(int, struct _stat32i64 *));
TYPE_IS(&_fstat64i32, int (*)(int, struct _stat64i32 *));
TYPE_IS(&_fstat64, int (*)(int, struct _stat64 *));
TYPE_IS(&_stat32, int (*)(const char *, struct _stat32 *));
TYPE_IS(&_stat32i64, int (*)(const char *, struct _stat32i64 *));
TYPE_IS(&_stat64i32, int (*)(const char *, struct _stat64i32 *));
TYPE_IS(&_stat64, int (*)(const char *, struct _stat64 *));
TYPE_IS(&_wstat32, int (*)(const wchar_t *, struct _stat32 *));
TYPE_IS(&_wstat32i64, int (*)(const wchar_t *, struct _stat32i64 *));
TYPE_IS(&_wstat64i32, int (*)(const wchar_t *, struct _stat64i32 *));
TYPE_IS(&_wstat64, int (*)(const wchar_t *, struct _stat64 *));

#ifdef S_IFBLK
#error "MSVC sys/stat.h must not expose S_IFBLK"
#endif
#ifdef S_IFLNK
#error "MSVC sys/stat.h must not expose S_IFLNK"
#endif
#ifdef S_IFSOCK
#error "MSVC sys/stat.h must not expose S_IFSOCK"
#endif
#ifdef UTIME_NOW
#error "MSVC sys/stat.h must not expose UTIME_NOW"
#endif

extern int chmod;
extern int lstat;
extern int mkdir;
extern int mkfifo;
extern int utimensat;

int main(void) { return 0; }
