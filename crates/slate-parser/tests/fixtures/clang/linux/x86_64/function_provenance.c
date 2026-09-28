typedef __SIZE_TYPE__ size_t;
typedef void          FILE;
typedef unsigned long pthread_t;

#include "function_provenance/string.h"
#include "function_provenance/strings.h"

static int   project_state;
static void *malloc(size_t size) { return size ? (void *)16 : 0; }
static void *calloc(size_t count, size_t size) {
  return count && size ? (void *)16 : 0;
}
static void *realloc(void *ptr, size_t size) {
  return ptr && size ? (void *)16 : 0;
}
static void  free(void *ptr) { project_state = ptr ? 77 : 0; }
static void *memcpy(void *dst, const void *src, size_t count) {
  return count && src ? dst : 0;
}
static void *memmove(void *dst, const void *src, size_t count) {
  return count && src ? dst : 0;
}
static void *memset(void *dst, int value, size_t count) {
  return value && count ? dst : 0;
}
static void *memchr(const void *ptr, int value, size_t count) {
  return ptr && value && count ? (void *)24 : 0;
}
static size_t strlen(const char *s) { return s ? 5 : 0; }
static char  *strcpy(char *dst, const char *src) { return src ? dst : 0; }
static char  *strcat(char *dst, const char *src) { return src ? dst : 0; }
static char  *strncpy(char *dst, const char *src, size_t count) {
  return src && count ? dst : 0;
}
static char *strncat(char *dst, const char *src, size_t count) {
  return src && count ? dst : 0;
}
static int strcmp(const char *lhs, const char *rhs) {
  return lhs && rhs ? 11 : 0;
}
static int strncmp(const char *lhs, const char *rhs, size_t count) {
  return lhs && rhs && count ? 12 : 0;
}
static int memcmp(const void *lhs, const void *rhs, size_t count) {
  return lhs && rhs && count ? 13 : 0;
}
static char *strchr(const char *s, int value) {
  return s && value ? (char *)32 : 0;
}
static char *strrchr(const char *s, int value) {
  return s && value ? (char *)32 : 0;
}
static char *strstr(const char *s, const char *needle) {
  return s && needle ? (char *)32 : 0;
}
static char *strpbrk(const char *s, const char *accept) {
  return s && accept ? (char *)32 : 0;
}
static size_t strspn(const char *s, const char *accept) {
  return s && accept ? 31 : 0;
}
static size_t strcspn(const char *s, const char *reject) {
  return s && reject ? 32 : 0;
}
static int  atoi(const char *s) { return s ? 41 : 0; }
static long atol(const char *s) { return s ? 42 : 0; }
static long strtol(const char *s, char **end, int base) {
  return s && end && base ? 43 : 0;
}
static unsigned long strtoul(const char *s, char **end, int base) {
  return s && end && base ? 44 : 0;
}
static double strtod(const char *s, char **end) {
  return s && end ? 45.0 : 0.0;
}
static int   printf(const char *format) { return format ? 51 : 0; }
static int   puts(const char *s) { return s ? 52 : 0; }
static FILE *fopen(const char *path, const char *mode) {
  return path && mode ? (FILE *)48 : 0;
}
static int   fputs(const char *s, FILE *stream) { return s && stream ? 54 : 0; }
static char *fgets(char *s, int count, FILE *stream) {
  return s && count && stream ? (char *)40 : 0;
}
static size_t fread(void *ptr, size_t size, size_t count, FILE *stream) {
  return ptr && size && count && stream ? 56 : 0;
}
static size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream) {
  return ptr && size && count && stream ? 57 : 0;
}
static int    fclose(FILE *stream) { return stream ? 58 : 0; }
static int    fflush(FILE *stream) { return stream ? 59 : 0; }
static int    remove(const char *path) { return path ? 60 : 0; }
static int    toupper(int value) { return value ? 61 : 0; }
static int    tolower(int value) { return value ? 62 : 0; }
static double sin(double value) { return value ? 63.0 : 0.0; }
static double cos(double value) { return value ? 64.0 : 0.0; }
static double tan(double value) { return value ? 65.0 : 0.0; }
static double log(double value) { return value ? 66.0 : 0.0; }
static double log10(double value) { return value ? 67.0 : 0.0; }
static double log2(double value) { return value ? 68.0 : 0.0; }
static double pow(double lhs, double rhs) { return lhs && rhs ? 69.0 : 0.0; }
static double sqrt(double value) { return value ? 70.0 : 0.0; }
static double exp(double value) { return value ? 71.0 : 0.0; }
static double exp2(double value) { return value ? 72.0 : 0.0; }
static double fmod(double lhs, double rhs) { return lhs && rhs ? 73.0 : 0.0; }
static long   lround(double value) { return value ? 74 : 0; }
static long long llround(double value) { return value ? 75 : 0; }
static int       pthread_create(pthread_t *thread, const void *attr,
                                void *(*start)(void *), void *arg) {
  return thread && !attr && start && arg ? 78 : 0;
}
static int pthread_join(pthread_t thread, void **result) {
  return thread && result ? 79 : 0;
}

static int compare(const void *lhs, const void *rhs) {
  return lhs == rhs ? 0 : 1;
}
static void *start(void *arg) { return arg; }
static void  qsort(void *base, size_t count, size_t size,
                   int (*callback)(const void *, const void *)) {
  project_state = base && count && size && callback ? 76 : 0;
}
static void *bsearch(const void *key, const void *base, size_t count,
                     size_t size, int (*callback)(const void *, const void *)) {
  return key && base && count && size && callback ? (void *)56 : 0;
}

static void exit(int status) { project_state = status + 80; }

#define CHECK(value, expected, code)                                           \
  do {                                                                         \
    if ((value) != (expected))                                                 \
      return code;                                                             \
  } while (0)

int main(void) {
  char            a[8]   = "a";
  char            b[8]   = "b";
  char           *end    = a;
  volatile size_t count  = 1;
  volatile int    number = 1;
  volatile double real   = 1.0;
  FILE           *stream;
  void           *ptr;
  pthread_t       thread                       = 1;
  size_t          (*strlen_call)(const char *) = strlen;

  ptr = malloc(count);
  CHECK(ptr, (void *)16, 2);
  CHECK(calloc(count, count), (void *)16, 3);
  CHECK(realloc(ptr, count), (void *)16, 4);
  free(ptr);
  CHECK(project_state, 77, 5);
  CHECK(memcpy(a, b, count), a, 6);
  CHECK(memmove(a, b, count), a, 7);
  CHECK(memset(a, number, count), a, 8);
  CHECK(memchr(a, number, count), (void *)24, 9);
  CHECK(strlen(a), 5, 10);
  CHECK(strlen_call(a), 5, 10);
  CHECK(strcpy(a, b), a, 11);
  CHECK(strcat(a, b), a, 12);
  CHECK(strncpy(a, b, count), a, 13);
  CHECK(strncat(a, b, count), a, 14);
  CHECK(strcmp(a, b), 11, 15);
  CHECK(strncmp(a, b, count), 12, 16);
  CHECK(memcmp(a, b, count), 13, 17);
  CHECK(strchr(a, number), (char *)32, 18);
  CHECK(strrchr(a, number), (char *)32, 19);
  CHECK(strstr(a, b), (char *)32, 20);
  CHECK(strpbrk(a, b), (char *)32, 21);
  CHECK(strspn(a, b), 31, 22);
  CHECK(strcspn(a, b), 32, 23);
  CHECK(atoi(a), 41, 24);
  CHECK(atol(a), 42, 25);
  CHECK(strtol(a, &end, number), 43, 26);
  CHECK(strtoul(a, &end, number), 44, 27);
  CHECK(strtod(a, &end), 45.0, 28);
  CHECK(printf(a), 51, 29);
  CHECK(puts(a), 52, 30);
  stream = fopen(a, b);
  CHECK(stream, (FILE *)48, 31);
  CHECK(fputs(a, stream), 54, 32);
  CHECK(fgets(a, number, stream), (char *)40, 33);
  CHECK(fread(a, count, count, stream), 56, 34);
  CHECK(fwrite(a, count, count, stream), 57, 35);
  CHECK(fclose(stream), 58, 36);
  CHECK(fflush(stream), 59, 37);
  CHECK(remove(a), 60, 38);
  CHECK(toupper(number), 61, 39);
  CHECK(tolower(number), 62, 40);
  CHECK(sin(real), 63.0, 41);
  CHECK(cos(real), 64.0, 42);
  CHECK(tan(real), 65.0, 43);
  CHECK(log(real), 66.0, 44);
  CHECK(log10(real), 67.0, 45);
  CHECK(log2(real), 68.0, 46);
  CHECK(pow(real, real), 69.0, 47);
  CHECK(sqrt(real), 70.0, 48);
  CHECK(exp(real), 71.0, 49);
  CHECK(exp2(real), 72.0, 50);
  CHECK(fmod(real, real), 73.0, 51);
  CHECK(lround(real), 74, 52);
  CHECK(llround(real), 75, 53);
  CHECK(pthread_create(&thread, 0, start, ptr), 78, 54);
  CHECK(pthread_join(thread, &ptr), 79, 55);
  qsort(a, count, count, compare);
  CHECK(project_state, 76, 56);
  CHECK(bsearch(a, b, count, count, compare), (void *)56, 57);
  exit(0);
  return project_state == 80 ? 1 : 58;
}




// SLATE-FILECHECK-ISYSTEM tests/fixtures/inputs/function-provenance-headers

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 FILE = void;
// DEFAULT-NEXT:     type @type2 pthread_t = u64;
// DEFAULT-NEXT:     global %4 project_state: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %3 @strlen(%31 s: ptr<const i8>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%31), null<ptr<const i8>>), const<i32>(5), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @malloc(%6 size: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(ne<u64>(read<u64>(%6), const<u64>(0)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @calloc(%8 count: u64, %9 size: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<u64>(read<u64>(%8), const<u64>(0)), ne<u64>(read<u64>(%9), const<u64>(0))), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @realloc(%11 ptr: ptr<void>, %12 size: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%11), null<ptr<void>>), ne<u64>(read<u64>(%12), const<u64>(0))), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @free(%14 ptr: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%4, conditional<i32>(ne<ptr<void>>(read<ptr<void>>(%14), null<ptr<void>>), const<i32>(77), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @memcpy(%16 dst: ptr<void>, %17 src: ptr<const void>, %18 count: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<u64>(read<u64>(%18), const<u64>(0)), ne<ptr<const void>>(read<ptr<const void>>(%17), null<ptr<const void>>)), read<ptr<void>>(%16), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @memmove(%20 dst: ptr<void>, %21 src: ptr<const void>, %22 count: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<u64>(read<u64>(%22), const<u64>(0)), ne<ptr<const void>>(read<ptr<const void>>(%21), null<ptr<const void>>)), read<ptr<void>>(%20), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @memset(%24 dst: ptr<void>, %25 value: i32, %26 count: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<i32>(read<i32>(%25), const<i32>(0)), ne<u64>(read<u64>(%26), const<u64>(0))), read<ptr<void>>(%24), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @memchr(%28 ptr: ptr<const void>, %29 value: i32, %30 count: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(logical_and<bool>(ne<ptr<const void>>(read<ptr<const void>>(%28), null<ptr<const void>>), ne<i32>(read<i32>(%29), const<i32>(0))), ne<u64>(read<u64>(%30), const<u64>(0))), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(24)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @strcpy(%33 dst: ptr<i8>, %34 src: ptr<const i8>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(ne<ptr<const i8>>(read<ptr<const i8>>(%34), null<ptr<const i8>>), read<ptr<i8>>(%33), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @strcat(%36 dst: ptr<i8>, %37 src: ptr<const i8>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(ne<ptr<const i8>>(read<ptr<const i8>>(%37), null<ptr<const i8>>), read<ptr<i8>>(%36), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @strncpy(%39 dst: ptr<i8>, %40 src: ptr<const i8>, %41 count: u64) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%40), null<ptr<const i8>>), ne<u64>(read<u64>(%41), const<u64>(0))), read<ptr<i8>>(%39), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %42 @strncat(%43 dst: ptr<i8>, %44 src: ptr<const i8>, %45 count: u64) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%44), null<ptr<const i8>>), ne<u64>(read<u64>(%45), const<u64>(0))), read<ptr<i8>>(%43), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @strcmp(%47 lhs: ptr<const i8>, %48 rhs: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%47), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%48), null<ptr<const i8>>)), const<i32>(11), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @strncmp(%50 lhs: ptr<const i8>, %51 rhs: ptr<const i8>, %52 count: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%50), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%51), null<ptr<const i8>>)), ne<u64>(read<u64>(%52), const<u64>(0))), const<i32>(12), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @memcmp(%54 lhs: ptr<const void>, %55 rhs: ptr<const void>, %56 count: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(logical_and<bool>(ne<ptr<const void>>(read<ptr<const void>>(%54), null<ptr<const void>>), ne<ptr<const void>>(read<ptr<const void>>(%55), null<ptr<const void>>)), ne<u64>(read<u64>(%56), const<u64>(0))), const<i32>(13), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @strchr(%58 s: ptr<const i8>, %59 value: i32) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%58), null<ptr<const i8>>), ne<i32>(read<i32>(%59), const<i32>(0))), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @strrchr(%61 s: ptr<const i8>, %62 value: i32) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%61), null<ptr<const i8>>), ne<i32>(read<i32>(%62), const<i32>(0))), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @strstr(%64 s: ptr<const i8>, %65 needle: ptr<const i8>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%64), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%65), null<ptr<const i8>>)), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @strpbrk(%67 s: ptr<const i8>, %68 accept: ptr<const i8>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%67), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%68), null<ptr<const i8>>)), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @strspn(%70 s: ptr<const i8>, %71 accept: ptr<const i8>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%70), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%71), null<ptr<const i8>>)), const<i32>(31), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %72 @strcspn(%73 s: ptr<const i8>, %74 reject: ptr<const i8>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%73), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%74), null<ptr<const i8>>)), const<i32>(32), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @atoi(%76 s: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%76), null<ptr<const i8>>), const<i32>(41), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @atol(%78 s: ptr<const i8>) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%78), null<ptr<const i8>>), const<i32>(42), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %79 @strtol(%80 s: ptr<const i8>, %81 end: ptr<ptr<i8>>, %82 base: i32) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(conditional<i32>(logical_and<bool>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%80), null<ptr<const i8>>), ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%81), null<ptr<ptr<i8>>>)), ne<i32>(read<i32>(%82), const<i32>(0))), const<i32>(43), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %83 @strtoul(%84 s: ptr<const i8>, %85 end: ptr<ptr<i8>>, %86 base: i32) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%84), null<ptr<const i8>>), ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%85), null<ptr<ptr<i8>>>)), ne<i32>(read<i32>(%86), const<i32>(0))), const<i32>(44), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @strtod(%88 s: ptr<const i8>, %89 end: ptr<ptr<i8>>) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%88), null<ptr<const i8>>), ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%89), null<ptr<ptr<i8>>>)), const<f64>(45.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @printf(%91 format: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%91), null<ptr<const i8>>), const<i32>(51), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %92 @puts(%93 s: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%93), null<ptr<const i8>>), const<i32>(52), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %94 @fopen(%95 path: ptr<const i8>, %96 mode: ptr<const i8>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%95), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%96), null<ptr<const i8>>)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(48)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %97 @fputs(%98 s: ptr<const i8>, %99 stream: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%98), null<ptr<const i8>>), ne<ptr<void>>(read<ptr<void>>(%99), null<ptr<void>>)), const<i32>(54), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @fgets(%101 s: ptr<i8>, %102 count: i32, %103 stream: ptr<void>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%101), null<ptr<i8>>), ne<i32>(read<i32>(%102), const<i32>(0))), ne<ptr<void>>(read<ptr<void>>(%103), null<ptr<void>>)), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(40)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %104 @fread(%105 ptr: ptr<void>, %106 size: u64, %107 count: u64, %108 stream: ptr<void>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%105), null<ptr<void>>), ne<u64>(read<u64>(%106), const<u64>(0))), ne<u64>(read<u64>(%107), const<u64>(0))), ne<ptr<void>>(read<ptr<void>>(%108), null<ptr<void>>)), const<i32>(56), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %109 @fwrite(%110 ptr: ptr<const void>, %111 size: u64, %112 count: u64, %113 stream: ptr<void>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<const void>>(read<ptr<const void>>(%110), null<ptr<const void>>), ne<u64>(read<u64>(%111), const<u64>(0))), ne<u64>(read<u64>(%112), const<u64>(0))), ne<ptr<void>>(read<ptr<void>>(%113), null<ptr<void>>)), const<i32>(57), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %114 @fclose(%115 stream: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<void>>(read<ptr<void>>(%115), null<ptr<void>>), const<i32>(58), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %116 @fflush(%117 stream: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<void>>(read<ptr<void>>(%117), null<ptr<void>>), const<i32>(59), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %118 @remove(%119 path: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%119), null<ptr<const i8>>), const<i32>(60), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %120 @toupper(%121 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%121), const<i32>(0)), const<i32>(61), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %122 @tolower(%123 value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%123), const<i32>(0)), const<i32>(62), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %124 @sin(%125 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%125), const<f64>(0.0)), const<f64>(63.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %126 @cos(%127 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%127), const<f64>(0.0)), const<f64>(64.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %128 @tan(%129 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%129), const<f64>(0.0)), const<f64>(65.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %130 @log(%131 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%131), const<f64>(0.0)), const<f64>(66.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %132 @log10(%133 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%133), const<f64>(0.0)), const<f64>(67.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %134 @log2(%135 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%135), const<f64>(0.0)), const<f64>(68.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %136 @pow(%137 lhs: f64, %138 rhs: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(logical_and<bool>(ne<f64, exceptions=ignore>(read<f64>(%137), const<f64>(0.0)), ne<f64, exceptions=ignore>(read<f64>(%138), const<f64>(0.0))), const<f64>(69.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %139 @sqrt(%140 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%140), const<f64>(0.0)), const<f64>(70.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %141 @exp(%142 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%142), const<f64>(0.0)), const<f64>(71.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %143 @exp2(%144 value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%144), const<f64>(0.0)), const<f64>(72.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %145 @fmod(%146 lhs: f64, %147 rhs: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(logical_and<bool>(ne<f64, exceptions=ignore>(read<f64>(%146), const<f64>(0.0)), ne<f64, exceptions=ignore>(read<f64>(%147), const<f64>(0.0))), const<f64>(73.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %148 @lround(%149 value: f64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(conditional<i32>(ne<f64, exceptions=ignore>(read<f64>(%149), const<f64>(0.0)), const<i32>(74), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @llround(%151 value: f64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(conditional<i32>(ne<f64, exceptions=ignore>(read<f64>(%151), const<f64>(0.0)), const<i32>(75), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %152 @pthread_create(%153 thread: ptr<u64>, %154 attr: ptr<const void>, %155 start: ptr<fn(ptr<void>) -> ptr<void>>, %156 arg: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<u64>>(read<ptr<u64>>(%153), null<ptr<u64>>), not<bool>(ne<ptr<const void>>(read<ptr<const void>>(%154), null<ptr<const void>>))), ne<ptr<fn(ptr<void>) -> ptr<void>>>(read<ptr<fn(ptr<void>) -> ptr<void>>>(%155), null<ptr<fn(ptr<void>) -> ptr<void>>>)), ne<ptr<void>>(read<ptr<void>>(%156), null<ptr<void>>)), const<i32>(78), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %157 @pthread_join(%158 thread: u64, %159 result: ptr<ptr<void>>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(ne<u64>(read<u64>(%158), const<u64>(0)), ne<ptr<ptr<void>>>(read<ptr<ptr<void>>>(%159), null<ptr<ptr<void>>>)), const<i32>(79), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %160 @compare(%161 lhs: ptr<const void>, %162 rhs: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(eq<ptr<const void>>(read<ptr<const void>>(%161), read<ptr<const void>>(%162)), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %163 @start(%164 arg: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<void>>(%164);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %165 @qsort(%166 base: ptr<void>, %167 count: u64, %168 size: u64, %169 callback: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%4, conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%166), null<ptr<void>>), ne<u64>(read<u64>(%167), const<u64>(0))), ne<u64>(read<u64>(%168), const<u64>(0))), ne<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(read<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%169), null<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>)), const<i32>(76), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %170 @bsearch(%171 key: ptr<const void>, %172 base: ptr<const void>, %173 count: u64, %174 size: u64, %175 callback: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<const void>>(read<ptr<const void>>(%171), null<ptr<const void>>), ne<ptr<const void>>(read<ptr<const void>>(%172), null<ptr<const void>>)), ne<u64>(read<u64>(%173), const<u64>(0))), ne<u64>(read<u64>(%174), const<u64>(0))), ne<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(read<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%175), null<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(56)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %176 @exit(%177 status: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%4, add<i32, overflow=ub>(read<i32>(%177), const<i32>(80)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %178 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %179 a: array<i8, 8> [storage=automatic] = code_units<array<i8, 8>>([97, 0, 0, 0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         let %180 b: array<i8, 8> [storage=automatic] = code_units<array<i8, 8>>([98, 0, 0, 0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         let %181 end: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(8)>(%179);
// DEFAULT-NEXT:         let %182 count: volatile u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %183 number: volatile i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %184 real: volatile f64 [storage=automatic] = const<f64>(1.0);
// DEFAULT-NEXT:         let %185 stream: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %186 ptr: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %187 thread: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %188 strlen_call: ptr<fn(ptr<const i8>) -> u64> [storage=automatic] = function_decay<ptr<fn(ptr<const i8>) -> u64>>(%3);
// DEFAULT-NEXT:         write<ptr<void>>(%186, call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, read<u64, volatile>(%182)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(u64) -> ptr<void>>(%5, read<u64, volatile>(%182));
// DEFAULT-NEXT:         do %191
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(%186), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)))
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %192
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%7, read<u64, volatile>(%182), read<u64, volatile>(%182)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)))
// DEFAULT-NEXT:                     return const<i32>(3);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %193
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%10, read<ptr<void>>(%186), read<u64, volatile>(%182)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)))
// DEFAULT-NEXT:                     return const<i32>(4);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%13, read<ptr<void>>(%186));
// DEFAULT-NEXT:         do %194
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%4), const<i32>(77))
// DEFAULT-NEXT:                     return const<i32>(5);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %195
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%15, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180)), read<u64, volatile>(%182)), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(8)>(%179)))
// DEFAULT-NEXT:                     return const<i32>(6);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %196
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%19, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180)), read<u64, volatile>(%182)), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(8)>(%179)))
// DEFAULT-NEXT:                     return const<i32>(7);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %197
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%23, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), read<i32, volatile>(%183), read<u64, volatile>(%182)), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(8)>(%179)))
// DEFAULT-NEXT:                     return const<i32>(8);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %198
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%27, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), read<i32, volatile>(%183), read<u64, volatile>(%182)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(24)))
// DEFAULT-NEXT:                     return const<i32>(9);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %199
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:                     return const<i32>(10);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %200
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(read<ptr<fn(ptr<const i8>) -> u64>>(%188), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:                     return const<i32>(10);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %201
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%32, array_decay<ptr<i8>, length=Some(8)>(%179), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180))), array_decay<ptr<i8>, length=Some(8)>(%179))
// DEFAULT-NEXT:                     return const<i32>(11);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %202
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%35, array_decay<ptr<i8>, length=Some(8)>(%179), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180))), array_decay<ptr<i8>, length=Some(8)>(%179))
// DEFAULT-NEXT:                     return const<i32>(12);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %203
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%38, array_decay<ptr<i8>, length=Some(8)>(%179), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180)), read<u64, volatile>(%182)), array_decay<ptr<i8>, length=Some(8)>(%179))
// DEFAULT-NEXT:                     return const<i32>(13);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %204
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%42, array_decay<ptr<i8>, length=Some(8)>(%179), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180)), read<u64, volatile>(%182)), array_decay<ptr<i8>, length=Some(8)>(%179))
// DEFAULT-NEXT:                     return const<i32>(14);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %205
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%46, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180))), const<i32>(11))
// DEFAULT-NEXT:                     return const<i32>(15);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %206
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%49, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180)), read<u64, volatile>(%182)), const<i32>(12))
// DEFAULT-NEXT:                     return const<i32>(16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %207
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%53, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180)), read<u64, volatile>(%182)), const<i32>(13))
// DEFAULT-NEXT:                     return const<i32>(17);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %208
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%57, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), read<i32, volatile>(%183)), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)))
// DEFAULT-NEXT:                     return const<i32>(18);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %209
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%60, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), read<i32, volatile>(%183)), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)))
// DEFAULT-NEXT:                     return const<i32>(19);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %210
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%63, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180))), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)))
// DEFAULT-NEXT:                     return const<i32>(20);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %211
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%66, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180))), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)))
// DEFAULT-NEXT:                     return const<i32>(21);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %212
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%69, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(31))))
// DEFAULT-NEXT:                     return const<i32>(22);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %213
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%72, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))))
// DEFAULT-NEXT:                     return const<i32>(23);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %214
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%75, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179))), const<i32>(41))
// DEFAULT-NEXT:                     return const<i32>(24);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %215
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i64>(call<i64, signature=fn(ptr<const i8>) -> i64>(%77, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179))), widen<i64, reason=usual_arith>(const<i32>(42)))
// DEFAULT-NEXT:                     return const<i32>(25);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %216
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i64>(call<i64, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) -> i64>(%79, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), addr_of<ptr<ptr<i8>>>(%181), read<i32, volatile>(%183)), widen<i64, reason=usual_arith>(const<i32>(43)))
// DEFAULT-NEXT:                     return const<i32>(26);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %217
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) -> u64>(%83, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), addr_of<ptr<ptr<i8>>>(%181), read<i32, volatile>(%183)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(44))))
// DEFAULT-NEXT:                     return const<i32>(27);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %218
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(ptr<const i8>, ptr<ptr<i8>>) -> f64>(%87, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), addr_of<ptr<ptr<i8>>>(%181)), const<f64>(45.0))
// DEFAULT-NEXT:                     return const<i32>(28);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %219
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%90, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179))), const<i32>(51))
// DEFAULT-NEXT:                     return const<i32>(29);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %220
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%92, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179))), const<i32>(52))
// DEFAULT-NEXT:                     return const<i32>(30);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<ptr<void>>(%185, call<ptr<void>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<void>>(%94, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<void>>(%94, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180)));
// DEFAULT-NEXT:         do %221
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(%185), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(48)))
// DEFAULT-NEXT:                     return const<i32>(31);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %222
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<void>) -> i32>(%97, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), read<ptr<void>>(%185)), const<i32>(54))
// DEFAULT-NEXT:                     return const<i32>(32);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %223
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, i32, ptr<void>) -> ptr<i8>>(%100, array_decay<ptr<i8>, length=Some(8)>(%179), read<i32, volatile>(%183), read<ptr<void>>(%185)), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(40)))
// DEFAULT-NEXT:                     return const<i32>(33);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %224
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<void>, u64, u64, ptr<void>) -> u64>(%104, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), read<u64, volatile>(%182), read<u64, volatile>(%182), read<ptr<void>>(%185)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(56))))
// DEFAULT-NEXT:                     return const<i32>(34);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %225
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const void>, u64, u64, ptr<void>) -> u64>(%109, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), read<u64, volatile>(%182), read<u64, volatile>(%182), read<ptr<void>>(%185)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(57))))
// DEFAULT-NEXT:                     return const<i32>(35);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %226
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%114, read<ptr<void>>(%185)), const<i32>(58))
// DEFAULT-NEXT:                     return const<i32>(36);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %227
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%116, read<ptr<void>>(%185)), const<i32>(59))
// DEFAULT-NEXT:                     return const<i32>(37);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %228
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%118, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179))), const<i32>(60))
// DEFAULT-NEXT:                     return const<i32>(38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %229
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%120, read<i32, volatile>(%183)), const<i32>(61))
// DEFAULT-NEXT:                     return const<i32>(39);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %230
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%122, read<i32, volatile>(%183)), const<i32>(62))
// DEFAULT-NEXT:                     return const<i32>(40);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %231
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%124, read<f64, volatile>(%184)), const<f64>(63.0))
// DEFAULT-NEXT:                     return const<i32>(41);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %232
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%126, read<f64, volatile>(%184)), const<f64>(64.0))
// DEFAULT-NEXT:                     return const<i32>(42);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %233
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%128, read<f64, volatile>(%184)), const<f64>(65.0))
// DEFAULT-NEXT:                     return const<i32>(43);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %234
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%130, read<f64, volatile>(%184)), const<f64>(66.0))
// DEFAULT-NEXT:                     return const<i32>(44);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %235
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%132, read<f64, volatile>(%184)), const<f64>(67.0))
// DEFAULT-NEXT:                     return const<i32>(45);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %236
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%134, read<f64, volatile>(%184)), const<f64>(68.0))
// DEFAULT-NEXT:                     return const<i32>(46);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %237
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%136, read<f64, volatile>(%184), read<f64, volatile>(%184)), const<f64>(69.0))
// DEFAULT-NEXT:                     return const<i32>(47);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %238
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%139, read<f64, volatile>(%184)), const<f64>(70.0))
// DEFAULT-NEXT:                     return const<i32>(48);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %239
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%141, read<f64, volatile>(%184)), const<f64>(71.0))
// DEFAULT-NEXT:                     return const<i32>(49);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %240
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%143, read<f64, volatile>(%184)), const<f64>(72.0))
// DEFAULT-NEXT:                     return const<i32>(50);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %241
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%145, read<f64, volatile>(%184), read<f64, volatile>(%184)), const<f64>(73.0))
// DEFAULT-NEXT:                     return const<i32>(51);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %242
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i64>(call<i64, signature=fn(f64) -> i64>(%148, read<f64, volatile>(%184)), widen<i64, reason=usual_arith>(const<i32>(74)))
// DEFAULT-NEXT:                     return const<i32>(52);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %243
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i64>(call<i64, signature=fn(f64) -> i64>(%150, read<f64, volatile>(%184)), widen<i64, reason=usual_arith>(const<i32>(75)))
// DEFAULT-NEXT:                     return const<i32>(53);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %244
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<u64>, ptr<const void>, ptr<fn(ptr<void>) -> ptr<void>>, ptr<void>) -> i32>(%152, addr_of<ptr<u64>>(%187), null<ptr<const void>>, function_decay<ptr<fn(ptr<void>) -> ptr<void>>>(%163), read<ptr<void>>(%186)), const<i32>(78))
// DEFAULT-NEXT:                     return const<i32>(54);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %245
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(u64, ptr<ptr<void>>) -> i32>(%157, read<u64>(%187), addr_of<ptr<ptr<void>>>(%186)), const<i32>(79))
// DEFAULT-NEXT:                     return const<i32>(55);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void>(%165, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), read<u64, volatile>(%182), read<u64, volatile>(%182), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%160));
// DEFAULT-NEXT:         do %246
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%4), const<i32>(76))
// DEFAULT-NEXT:                     return const<i32>(56);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %247
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, ptr<const void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void>>(%170, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%179)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%180)), read<u64, volatile>(%182), read<u64, volatile>(%182), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%160)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(56)))
// DEFAULT-NEXT:                     return const<i32>(57);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%176, const<i32>(0));
// DEFAULT-NEXT:         return conditional<i32>(eq<i32>(read<i32>(%4), const<i32>(80)), const<i32>(1), const<i32>(58));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
