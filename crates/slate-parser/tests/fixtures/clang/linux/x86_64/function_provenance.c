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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_FILE:[0-9]+]] FILE = void;
// DEFAULT-NEXT:     type @type[[TYPE_pthread_t:[0-9]+]] pthread_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_project_state:[0-9]+]] project_state: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE_s:[0-9]+]] s: ptr<const i8>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s]]), null<ptr<const i8>>), const<i32>(5), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE_size:[0-9]+]] size: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(ne<u64>(read<u64>(%[[VALUE_size]]), const<u64>(0)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_calloc:[0-9]+]] @calloc(%[[VALUE_count:[0-9]+]] count: u64, %[[VALUE_size_2:[0-9]+]] size: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<u64>(read<u64>(%[[VALUE_count]]), const<u64>(0)), ne<u64>(read<u64>(%[[VALUE_size_2]]), const<u64>(0))), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_realloc:[0-9]+]] @realloc(%[[VALUE_ptr:[0-9]+]] ptr: ptr<void>, %[[VALUE_size_3:[0-9]+]] size: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_ptr]]), null<ptr<void>>), ne<u64>(read<u64>(%[[VALUE_size_3]]), const<u64>(0))), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE_ptr_2:[0-9]+]] ptr: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_project_state]], conditional<i32>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_ptr_2]]), null<ptr<void>>), const<i32>(77), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE_dst:[0-9]+]] dst: ptr<void>, %[[VALUE_src:[0-9]+]] src: ptr<const void>, %[[VALUE_count_2:[0-9]+]] count: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<u64>(read<u64>(%[[VALUE_count_2]]), const<u64>(0)), ne<ptr<const void>>(read<ptr<const void>>(%[[VALUE_src]]), null<ptr<const void>>)), read<ptr<void>>(%[[VALUE_dst]]), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_memmove:[0-9]+]] @memmove(%[[VALUE_dst_2:[0-9]+]] dst: ptr<void>, %[[VALUE_src_2:[0-9]+]] src: ptr<const void>, %[[VALUE_count_3:[0-9]+]] count: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<u64>(read<u64>(%[[VALUE_count_3]]), const<u64>(0)), ne<ptr<const void>>(read<ptr<const void>>(%[[VALUE_src_2]]), null<ptr<const void>>)), read<ptr<void>>(%[[VALUE_dst_2]]), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE_dst_3:[0-9]+]] dst: ptr<void>, %[[VALUE_value:[0-9]+]] value: i32, %[[VALUE_count_4:[0-9]+]] count: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_value]]), const<i32>(0)), ne<u64>(read<u64>(%[[VALUE_count_4]]), const<u64>(0))), read<ptr<void>>(%[[VALUE_dst_3]]), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_memchr:[0-9]+]] @memchr(%[[VALUE_ptr_3:[0-9]+]] ptr: ptr<const void>, %[[VALUE_value_2:[0-9]+]] value: i32, %[[VALUE_count_5:[0-9]+]] count: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(logical_and<bool>(ne<ptr<const void>>(read<ptr<const void>>(%[[VALUE_ptr_3]]), null<ptr<const void>>), ne<i32>(read<i32>(%[[VALUE_value_2]]), const<i32>(0))), ne<u64>(read<u64>(%[[VALUE_count_5]]), const<u64>(0))), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(24)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strcpy:[0-9]+]] @strcpy(%[[VALUE_dst_4:[0-9]+]] dst: ptr<i8>, %[[VALUE_src_3:[0-9]+]] src: ptr<const i8>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_src_3]]), null<ptr<const i8>>), read<ptr<i8>>(%[[VALUE_dst_4]]), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strcat:[0-9]+]] @strcat(%[[VALUE_dst_5:[0-9]+]] dst: ptr<i8>, %[[VALUE_src_4:[0-9]+]] src: ptr<const i8>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_src_4]]), null<ptr<const i8>>), read<ptr<i8>>(%[[VALUE_dst_5]]), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strncpy:[0-9]+]] @strncpy(%[[VALUE_dst_6:[0-9]+]] dst: ptr<i8>, %[[VALUE_src_5:[0-9]+]] src: ptr<const i8>, %[[VALUE_count_6:[0-9]+]] count: u64) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_src_5]]), null<ptr<const i8>>), ne<u64>(read<u64>(%[[VALUE_count_6]]), const<u64>(0))), read<ptr<i8>>(%[[VALUE_dst_6]]), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strncat:[0-9]+]] @strncat(%[[VALUE_dst_7:[0-9]+]] dst: ptr<i8>, %[[VALUE_src_6:[0-9]+]] src: ptr<const i8>, %[[VALUE_count_7:[0-9]+]] count: u64) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_src_6]]), null<ptr<const i8>>), ne<u64>(read<u64>(%[[VALUE_count_7]]), const<u64>(0))), read<ptr<i8>>(%[[VALUE_dst_7]]), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE_lhs:[0-9]+]] lhs: ptr<const i8>, %[[VALUE_rhs:[0-9]+]] rhs: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_lhs]]), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_rhs]]), null<ptr<const i8>>)), const<i32>(11), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strncmp:[0-9]+]] @strncmp(%[[VALUE_lhs_2:[0-9]+]] lhs: ptr<const i8>, %[[VALUE_rhs_2:[0-9]+]] rhs: ptr<const i8>, %[[VALUE_count_8:[0-9]+]] count: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_lhs_2]]), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_rhs_2]]), null<ptr<const i8>>)), ne<u64>(read<u64>(%[[VALUE_count_8]]), const<u64>(0))), const<i32>(12), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE_lhs_3:[0-9]+]] lhs: ptr<const void>, %[[VALUE_rhs_3:[0-9]+]] rhs: ptr<const void>, %[[VALUE_count_9:[0-9]+]] count: u64) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(logical_and<bool>(ne<ptr<const void>>(read<ptr<const void>>(%[[VALUE_lhs_3]]), null<ptr<const void>>), ne<ptr<const void>>(read<ptr<const void>>(%[[VALUE_rhs_3]]), null<ptr<const void>>)), ne<u64>(read<u64>(%[[VALUE_count_9]]), const<u64>(0))), const<i32>(13), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strchr:[0-9]+]] @strchr(%[[VALUE_s_2:[0-9]+]] s: ptr<const i8>, %[[VALUE_value_3:[0-9]+]] value: i32) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_2]]), null<ptr<const i8>>), ne<i32>(read<i32>(%[[VALUE_value_3]]), const<i32>(0))), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strrchr:[0-9]+]] @strrchr(%[[VALUE_s_3:[0-9]+]] s: ptr<const i8>, %[[VALUE_value_4:[0-9]+]] value: i32) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_3]]), null<ptr<const i8>>), ne<i32>(read<i32>(%[[VALUE_value_4]]), const<i32>(0))), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strstr:[0-9]+]] @strstr(%[[VALUE_s_4:[0-9]+]] s: ptr<const i8>, %[[VALUE_needle:[0-9]+]] needle: ptr<const i8>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_4]]), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_needle]]), null<ptr<const i8>>)), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strpbrk:[0-9]+]] @strpbrk(%[[VALUE_s_5:[0-9]+]] s: ptr<const i8>, %[[VALUE_accept:[0-9]+]] accept: ptr<const i8>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_5]]), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_accept]]), null<ptr<const i8>>)), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strspn:[0-9]+]] @strspn(%[[VALUE_s_6:[0-9]+]] s: ptr<const i8>, %[[VALUE_accept_2:[0-9]+]] accept: ptr<const i8>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_6]]), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_accept_2]]), null<ptr<const i8>>)), const<i32>(31), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strcspn:[0-9]+]] @strcspn(%[[VALUE_s_7:[0-9]+]] s: ptr<const i8>, %[[VALUE_reject:[0-9]+]] reject: ptr<const i8>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_7]]), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_reject]]), null<ptr<const i8>>)), const<i32>(32), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_atoi:[0-9]+]] @atoi(%[[VALUE_s_8:[0-9]+]] s: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_8]]), null<ptr<const i8>>), const<i32>(41), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_atol:[0-9]+]] @atol(%[[VALUE_s_9:[0-9]+]] s: ptr<const i8>) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_9]]), null<ptr<const i8>>), const<i32>(42), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strtol:[0-9]+]] @strtol(%[[VALUE_s_10:[0-9]+]] s: ptr<const i8>, %[[VALUE_end:[0-9]+]] end: ptr<ptr<i8>>, %[[VALUE_base:[0-9]+]] base: i32) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(conditional<i32>(logical_and<bool>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_10]]), null<ptr<const i8>>), ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%[[VALUE_end]]), null<ptr<ptr<i8>>>)), ne<i32>(read<i32>(%[[VALUE_base]]), const<i32>(0))), const<i32>(43), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strtoul:[0-9]+]] @strtoul(%[[VALUE_s_11:[0-9]+]] s: ptr<const i8>, %[[VALUE_end_2:[0-9]+]] end: ptr<ptr<i8>>, %[[VALUE_base_2:[0-9]+]] base: i32) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_11]]), null<ptr<const i8>>), ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%[[VALUE_end_2]]), null<ptr<ptr<i8>>>)), ne<i32>(read<i32>(%[[VALUE_base_2]]), const<i32>(0))), const<i32>(44), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_strtod:[0-9]+]] @strtod(%[[VALUE_s_12:[0-9]+]] s: ptr<const i8>, %[[VALUE_end_3:[0-9]+]] end: ptr<ptr<i8>>) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_12]]), null<ptr<const i8>>), ne<ptr<ptr<i8>>>(read<ptr<ptr<i8>>>(%[[VALUE_end_3]]), null<ptr<ptr<i8>>>)), const<f64>(45.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE_format:[0-9]+]] format: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_format]]), null<ptr<const i8>>), const<i32>(51), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_puts:[0-9]+]] @puts(%[[VALUE_s_13:[0-9]+]] s: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_13]]), null<ptr<const i8>>), const<i32>(52), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fopen:[0-9]+]] @fopen(%[[VALUE_path:[0-9]+]] path: ptr<const i8>, %[[VALUE_mode:[0-9]+]] mode: ptr<const i8>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_path]]), null<ptr<const i8>>), ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_mode]]), null<ptr<const i8>>)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(48)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fputs:[0-9]+]] @fputs(%[[VALUE_s_14:[0-9]+]] s: ptr<const i8>, %[[VALUE_stream:[0-9]+]] stream: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s_14]]), null<ptr<const i8>>), ne<ptr<void>>(read<ptr<void>>(%[[VALUE_stream]]), null<ptr<void>>)), const<i32>(54), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fgets:[0-9]+]] @fgets(%[[VALUE_s_15:[0-9]+]] s: ptr<i8>, %[[VALUE_count_10:[0-9]+]] count: i32, %[[VALUE_stream_2:[0-9]+]] stream: ptr<void>) -> ptr<i8> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<i8>>(logical_and<bool>(logical_and<bool>(ne<ptr<i8>>(read<ptr<i8>>(%[[VALUE_s_15]]), null<ptr<i8>>), ne<i32>(read<i32>(%[[VALUE_count_10]]), const<i32>(0))), ne<ptr<void>>(read<ptr<void>>(%[[VALUE_stream_2]]), null<ptr<void>>)), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(40)), null<ptr<i8>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fread:[0-9]+]] @fread(%[[VALUE_ptr_4:[0-9]+]] ptr: ptr<void>, %[[VALUE_size_4:[0-9]+]] size: u64, %[[VALUE_count_11:[0-9]+]] count: u64, %[[VALUE_stream_3:[0-9]+]] stream: ptr<void>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_ptr_4]]), null<ptr<void>>), ne<u64>(read<u64>(%[[VALUE_size_4]]), const<u64>(0))), ne<u64>(read<u64>(%[[VALUE_count_11]]), const<u64>(0))), ne<ptr<void>>(read<ptr<void>>(%[[VALUE_stream_3]]), null<ptr<void>>)), const<i32>(56), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fwrite:[0-9]+]] @fwrite(%[[VALUE_ptr_5:[0-9]+]] ptr: ptr<const void>, %[[VALUE_size_5:[0-9]+]] size: u64, %[[VALUE_count_12:[0-9]+]] count: u64, %[[VALUE_stream_4:[0-9]+]] stream: ptr<void>) -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u64, reason=return, fits=unknown>(widen<i64, reason=return>(conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<const void>>(read<ptr<const void>>(%[[VALUE_ptr_5]]), null<ptr<const void>>), ne<u64>(read<u64>(%[[VALUE_size_5]]), const<u64>(0))), ne<u64>(read<u64>(%[[VALUE_count_12]]), const<u64>(0))), ne<ptr<void>>(read<ptr<void>>(%[[VALUE_stream_4]]), null<ptr<void>>)), const<i32>(57), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fclose:[0-9]+]] @fclose(%[[VALUE_stream_5:[0-9]+]] stream: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_stream_5]]), null<ptr<void>>), const<i32>(58), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fflush:[0-9]+]] @fflush(%[[VALUE_stream_6:[0-9]+]] stream: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_stream_6]]), null<ptr<void>>), const<i32>(59), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_remove:[0-9]+]] @remove(%[[VALUE_path_2:[0-9]+]] path: ptr<const i8>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_path_2]]), null<ptr<const i8>>), const<i32>(60), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_toupper:[0-9]+]] @toupper(%[[VALUE_value_5:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%[[VALUE_value_5]]), const<i32>(0)), const<i32>(61), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tolower:[0-9]+]] @tolower(%[[VALUE_value_6:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(ne<i32>(read<i32>(%[[VALUE_value_6]]), const<i32>(0)), const<i32>(62), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sin:[0-9]+]] @sin(%[[VALUE_value_7:[0-9]+]] value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_7]]), const<f64>(0.0)), const<f64>(63.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_cos:[0-9]+]] @cos(%[[VALUE_value_8:[0-9]+]] value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_8]]), const<f64>(0.0)), const<f64>(64.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tan:[0-9]+]] @tan(%[[VALUE_value_9:[0-9]+]] value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_9]]), const<f64>(0.0)), const<f64>(65.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_log:[0-9]+]] @log(%[[VALUE_value_10:[0-9]+]] value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_10]]), const<f64>(0.0)), const<f64>(66.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_log10:[0-9]+]] @log10(%[[VALUE_value_11:[0-9]+]] value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_11]]), const<f64>(0.0)), const<f64>(67.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_log2:[0-9]+]] @log2(%[[VALUE_value_12:[0-9]+]] value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_12]]), const<f64>(0.0)), const<f64>(68.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pow:[0-9]+]] @pow(%[[VALUE_lhs_4:[0-9]+]] lhs: f64, %[[VALUE_rhs_4:[0-9]+]] rhs: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(logical_and<bool>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_lhs_4]]), const<f64>(0.0)), ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_rhs_4]]), const<f64>(0.0))), const<f64>(69.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sqrt:[0-9]+]] @sqrt(%[[VALUE_value_13:[0-9]+]] value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_13]]), const<f64>(0.0)), const<f64>(70.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_exp:[0-9]+]] @exp(%[[VALUE_value_14:[0-9]+]] value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_14]]), const<f64>(0.0)), const<f64>(71.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_exp2:[0-9]+]] @exp2(%[[VALUE_value_15:[0-9]+]] value: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_15]]), const<f64>(0.0)), const<f64>(72.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fmod:[0-9]+]] @fmod(%[[VALUE_lhs_5:[0-9]+]] lhs: f64, %[[VALUE_rhs_5:[0-9]+]] rhs: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(logical_and<bool>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_lhs_5]]), const<f64>(0.0)), ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_rhs_5]]), const<f64>(0.0))), const<f64>(73.0), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_lround:[0-9]+]] @lround(%[[VALUE_value_16:[0-9]+]] value: f64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(conditional<i32>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_16]]), const<f64>(0.0)), const<i32>(74), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_llround:[0-9]+]] @llround(%[[VALUE_value_17:[0-9]+]] value: f64) -> i64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i64, reason=return>(conditional<i32>(ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value_17]]), const<f64>(0.0)), const<i32>(75), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pthread_create:[0-9]+]] @pthread_create(%[[VALUE_thread:[0-9]+]] thread: ptr<u64>, %[[VALUE_attr:[0-9]+]] attr: ptr<const void>, %[[VALUE_start:[0-9]+]] start: ptr<fn(ptr<void>) -> ptr<void>>, %[[VALUE_arg:[0-9]+]] arg: ptr<void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<u64>>(read<ptr<u64>>(%[[VALUE_thread]]), null<ptr<u64>>), not<bool>(ne<ptr<const void>>(read<ptr<const void>>(%[[VALUE_attr]]), null<ptr<const void>>))), ne<ptr<fn(ptr<void>) -> ptr<void>>>(read<ptr<fn(ptr<void>) -> ptr<void>>>(%[[VALUE_start]]), null<ptr<fn(ptr<void>) -> ptr<void>>>)), ne<ptr<void>>(read<ptr<void>>(%[[VALUE_arg]]), null<ptr<void>>)), const<i32>(78), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pthread_join:[0-9]+]] @pthread_join(%[[VALUE_thread_2:[0-9]+]] thread: u64, %[[VALUE_result:[0-9]+]] result: ptr<ptr<void>>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(ne<u64>(read<u64>(%[[VALUE_thread_2]]), const<u64>(0)), ne<ptr<ptr<void>>>(read<ptr<ptr<void>>>(%[[VALUE_result]]), null<ptr<ptr<void>>>)), const<i32>(79), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_compare:[0-9]+]] @compare(%[[VALUE_lhs_6:[0-9]+]] lhs: ptr<const void>, %[[VALUE_rhs_6:[0-9]+]] rhs: ptr<const void>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<i32>(eq<ptr<const void>>(read<ptr<const void>>(%[[VALUE_lhs_6]]), read<ptr<const void>>(%[[VALUE_rhs_6]])), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_start_2:[0-9]+]] @start(%[[VALUE_arg_2:[0-9]+]] arg: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<void>>(%[[VALUE_arg_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_qsort:[0-9]+]] @qsort(%[[VALUE_base_3:[0-9]+]] base: ptr<void>, %[[VALUE_count_13:[0-9]+]] count: u64, %[[VALUE_size_6:[0-9]+]] size: u64, %[[VALUE_callback:[0-9]+]] callback: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_project_state]], conditional<i32>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<void>>(read<ptr<void>>(%[[VALUE_base_3]]), null<ptr<void>>), ne<u64>(read<u64>(%[[VALUE_count_13]]), const<u64>(0))), ne<u64>(read<u64>(%[[VALUE_size_6]]), const<u64>(0))), ne<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(read<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_callback]]), null<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>)), const<i32>(76), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bsearch:[0-9]+]] @bsearch(%[[VALUE_key:[0-9]+]] key: ptr<const void>, %[[VALUE_base_4:[0-9]+]] base: ptr<const void>, %[[VALUE_count_14:[0-9]+]] count: u64, %[[VALUE_size_7:[0-9]+]] size: u64, %[[VALUE_callback_2:[0-9]+]] callback: ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<ptr<void>>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(ne<ptr<const void>>(read<ptr<const void>>(%[[VALUE_key]]), null<ptr<const void>>), ne<ptr<const void>>(read<ptr<const void>>(%[[VALUE_base_4]]), null<ptr<const void>>)), ne<u64>(read<u64>(%[[VALUE_count_14]]), const<u64>(0))), ne<u64>(read<u64>(%[[VALUE_size_7]]), const<u64>(0))), ne<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(read<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_callback_2]]), null<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>)), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(56)), null<ptr<void>>);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE_status:[0-9]+]] status: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_project_state]], add<i32, overflow=ub>(read<i32>(%[[VALUE_status]]), const<i32>(80)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i8, 8> [storage=automatic] = code_units<array<i8, 8>>([97, 0, 0, 0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: array<i8, 8> [storage=automatic] = code_units<array<i8, 8>>([98, 0, 0, 0, 0, 0, 0, 0]);
// DEFAULT-NEXT:         let %[[VALUE_end_4:[0-9]+]] end: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE_count_15:[0-9]+]] count: volatile u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_number:[0-9]+]] number: volatile i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_real:[0-9]+]] real: volatile f64 [storage=automatic] = const<f64>(1.0);
// DEFAULT-NEXT:         let %[[VALUE_stream_7:[0-9]+]] stream: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ptr_6:[0-9]+]] ptr: ptr<void> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_thread_3:[0-9]+]] thread: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_strlen_call:[0-9]+]] strlen_call: ptr<fn(ptr<const i8>) -> u64> [storage=automatic] = function_decay<ptr<fn(ptr<const i8>) -> u64>>(%[[VALUE_strlen]]);
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_ptr_6]], call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], read<u64, volatile>(%[[VALUE_count_15]])));
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(%[[VALUE_ptr_6]]), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)))
// DEFAULT-NEXT:                     return const<i32>(2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(u64, u64) -> ptr<void>>(%[[VALUE_calloc]], read<u64, volatile>(%[[VALUE_count_15]]), read<u64, volatile>(%[[VALUE_count_15]])), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)))
// DEFAULT-NEXT:                     return const<i32>(3);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], read<ptr<void>>(%[[VALUE_ptr_6]]), read<u64, volatile>(%[[VALUE_count_15]])), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(16)))
// DEFAULT-NEXT:                     return const<i32>(4);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_ptr_6]]));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_project_state]]), const<i32>(77))
// DEFAULT-NEXT:                     return const<i32>(5);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]])), read<u64, volatile>(%[[VALUE_count_15]])), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])))
// DEFAULT-NEXT:                     return const<i32>(6);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memmove]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]])), read<u64, volatile>(%[[VALUE_count_15]])), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])))
// DEFAULT-NEXT:                     return const<i32>(7);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), read<i32, volatile>(%[[VALUE_number]]), read<u64, volatile>(%[[VALUE_count_15]])), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])))
// DEFAULT-NEXT:                     return const<i32>(8);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, i32, u64) -> ptr<void>>(%[[VALUE_memchr]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), read<i32, volatile>(%[[VALUE_number]]), read<u64, volatile>(%[[VALUE_count_15]])), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(24)))
// DEFAULT-NEXT:                     return const<i32>(9);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:                     return const<i32>(10);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(read<ptr<fn(ptr<const i8>) -> u64>>(%[[VALUE_strlen_call]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:                     return const<i32>(10);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcpy]], array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]]))), array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))
// DEFAULT-NEXT:                     return const<i32>(11);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strcat]], array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]]))), array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))
// DEFAULT-NEXT:                     return const<i32>(12);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]])), read<u64, volatile>(%[[VALUE_count_15]])), array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))
// DEFAULT-NEXT:                     return const<i32>(13);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncat]], array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]])), read<u64, volatile>(%[[VALUE_count_15]])), array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))
// DEFAULT-NEXT:                     return const<i32>(14);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]]))), const<i32>(11))
// DEFAULT-NEXT:                     return const<i32>(15);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%[[VALUE_strncmp]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]])), read<u64, volatile>(%[[VALUE_count_15]])), const<i32>(12))
// DEFAULT-NEXT:                     return const<i32>(16);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]])), read<u64, volatile>(%[[VALUE_count_15]])), const<i32>(13))
// DEFAULT-NEXT:                     return const<i32>(17);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), read<i32, volatile>(%[[VALUE_number]])), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)))
// DEFAULT-NEXT:                     return const<i32>(18);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strrchr]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), read<i32, volatile>(%[[VALUE_number]])), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)))
// DEFAULT-NEXT:                     return const<i32>(19);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strstr]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]]))), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)))
// DEFAULT-NEXT:                     return const<i32>(20);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE_strpbrk]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]]))), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(32)))
// DEFAULT-NEXT:                     return const<i32>(21);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE_strspn]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(31))))
// DEFAULT-NEXT:                     return const<i32>(22);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>, ptr<const i8>) -> u64>(%[[VALUE_strcspn]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))))
// DEFAULT-NEXT:                     return const<i32>(23);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))), const<i32>(41))
// DEFAULT-NEXT:                     return const<i32>(24);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE24:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i64>(call<i64, signature=fn(ptr<const i8>) -> i64>(%[[VALUE_atol]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))), widen<i64, reason=usual_arith>(const<i32>(42)))
// DEFAULT-NEXT:                     return const<i32>(25);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i64>(call<i64, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) -> i64>(%[[VALUE_strtol]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), addr_of<ptr<ptr<i8>>>(%[[VALUE_end_4]]), read<i32, volatile>(%[[VALUE_number]])), widen<i64, reason=usual_arith>(const<i32>(43)))
// DEFAULT-NEXT:                     return const<i32>(26);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE26:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const i8>, ptr<ptr<i8>>, i32) -> u64>(%[[VALUE_strtoul]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), addr_of<ptr<ptr<i8>>>(%[[VALUE_end_4]]), read<i32, volatile>(%[[VALUE_number]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(44))))
// DEFAULT-NEXT:                     return const<i32>(27);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE27:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(ptr<const i8>, ptr<ptr<i8>>) -> f64>(%[[VALUE_strtod]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), addr_of<ptr<ptr<i8>>>(%[[VALUE_end_4]])), const<f64>(45.0))
// DEFAULT-NEXT:                     return const<i32>(28);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))), const<i32>(51))
// DEFAULT-NEXT:                     return const<i32>(29);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE29:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_puts]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))), const<i32>(52))
// DEFAULT-NEXT:                     return const<i32>(30);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<ptr<void>>(%[[VALUE_stream_7]], call<ptr<void>, signature=fn(ptr<const i8>, ptr<const i8>) -> ptr<void>>(%[[VALUE_fopen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         do %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(read<ptr<void>>(%[[VALUE_stream_7]]), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(48)))
// DEFAULT-NEXT:                     return const<i32>(31);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE31:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<void>) -> i32>(%[[VALUE_fputs]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), read<ptr<void>>(%[[VALUE_stream_7]])), const<i32>(54))
// DEFAULT-NEXT:                     return const<i32>(32);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE32:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, i32, ptr<void>) -> ptr<i8>>(%[[VALUE_fgets]], array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]), read<i32, volatile>(%[[VALUE_number]]), read<ptr<void>>(%[[VALUE_stream_7]])), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(40)))
// DEFAULT-NEXT:                     return const<i32>(33);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<void>, u64, u64, ptr<void>) -> u64>(%[[VALUE_fread]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), read<u64, volatile>(%[[VALUE_count_15]]), read<u64, volatile>(%[[VALUE_count_15]]), read<ptr<void>>(%[[VALUE_stream_7]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(56))))
// DEFAULT-NEXT:                     return const<i32>(34);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE34:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<u64>(call<u64, signature=fn(ptr<const void>, u64, u64, ptr<void>) -> u64>(%[[VALUE_fwrite]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), read<u64, volatile>(%[[VALUE_count_15]]), read<u64, volatile>(%[[VALUE_count_15]]), read<ptr<void>>(%[[VALUE_stream_7]])), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(57))))
// DEFAULT-NEXT:                     return const<i32>(35);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_fclose]], read<ptr<void>>(%[[VALUE_stream_7]])), const<i32>(58))
// DEFAULT-NEXT:                     return const<i32>(36);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE36:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<void>) -> i32>(%[[VALUE_fflush]], read<ptr<void>>(%[[VALUE_stream_7]])), const<i32>(59))
// DEFAULT-NEXT:                     return const<i32>(37);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE37:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_remove]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]]))), const<i32>(60))
// DEFAULT-NEXT:                     return const<i32>(38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_toupper]], read<i32, volatile>(%[[VALUE_number]])), const<i32>(61))
// DEFAULT-NEXT:                     return const<i32>(39);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE39:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_tolower]], read<i32, volatile>(%[[VALUE_number]])), const<i32>(62))
// DEFAULT-NEXT:                     return const<i32>(40);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE40:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_sin]], read<f64, volatile>(%[[VALUE_real]])), const<f64>(63.0))
// DEFAULT-NEXT:                     return const<i32>(41);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE41:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_cos]], read<f64, volatile>(%[[VALUE_real]])), const<f64>(64.0))
// DEFAULT-NEXT:                     return const<i32>(42);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_tan]], read<f64, volatile>(%[[VALUE_real]])), const<f64>(65.0))
// DEFAULT-NEXT:                     return const<i32>(43);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE43:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_log]], read<f64, volatile>(%[[VALUE_real]])), const<f64>(66.0))
// DEFAULT-NEXT:                     return const<i32>(44);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE44:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_log10]], read<f64, volatile>(%[[VALUE_real]])), const<f64>(67.0))
// DEFAULT-NEXT:                     return const<i32>(45);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE45:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_log2]], read<f64, volatile>(%[[VALUE_real]])), const<f64>(68.0))
// DEFAULT-NEXT:                     return const<i32>(46);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE46:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64, volatile>(%[[VALUE_real]]), read<f64, volatile>(%[[VALUE_real]])), const<f64>(69.0))
// DEFAULT-NEXT:                     return const<i32>(47);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE47:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], read<f64, volatile>(%[[VALUE_real]])), const<f64>(70.0))
// DEFAULT-NEXT:                     return const<i32>(48);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE48:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64, volatile>(%[[VALUE_real]])), const<f64>(71.0))
// DEFAULT-NEXT:                     return const<i32>(49);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE49:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp2]], read<f64, volatile>(%[[VALUE_real]])), const<f64>(72.0))
// DEFAULT-NEXT:                     return const<i32>(50);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE50:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], read<f64, volatile>(%[[VALUE_real]]), read<f64, volatile>(%[[VALUE_real]])), const<f64>(73.0))
// DEFAULT-NEXT:                     return const<i32>(51);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE51:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i64>(call<i64, signature=fn(f64) -> i64>(%[[VALUE_lround]], read<f64, volatile>(%[[VALUE_real]])), widen<i64, reason=usual_arith>(const<i32>(74)))
// DEFAULT-NEXT:                     return const<i32>(52);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i64>(call<i64, signature=fn(f64) -> i64>(%[[VALUE_llround]], read<f64, volatile>(%[[VALUE_real]])), widen<i64, reason=usual_arith>(const<i32>(75)))
// DEFAULT-NEXT:                     return const<i32>(53);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE53:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<u64>, ptr<const void>, ptr<fn(ptr<void>) -> ptr<void>>, ptr<void>) -> i32>(%[[VALUE_pthread_create]], addr_of<ptr<u64>>(%[[VALUE_thread_3]]), null<ptr<const void>>, function_decay<ptr<fn(ptr<void>) -> ptr<void>>>(%[[VALUE_start_2]]), read<ptr<void>>(%[[VALUE_ptr_6]])), const<i32>(78))
// DEFAULT-NEXT:                     return const<i32>(54);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE54:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(u64, ptr<ptr<void>>) -> i32>(%[[VALUE_pthread_join]], read<u64>(%[[VALUE_thread_3]]), addr_of<ptr<ptr<void>>>(%[[VALUE_ptr_6]])), const<i32>(79))
// DEFAULT-NEXT:                     return const<i32>(55);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> void>(%[[VALUE_qsort]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), read<u64, volatile>(%[[VALUE_count_15]]), read<u64, volatile>(%[[VALUE_count_15]]), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_compare]]));
// DEFAULT-NEXT:         do %[[VALUE55:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_project_state]]), const<i32>(76))
// DEFAULT-NEXT:                     return const<i32>(56);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<const void>, ptr<const void>, u64, u64, ptr<fn(ptr<const void>, ptr<const void>) -> i32>) -> ptr<void>>(%[[VALUE_bsearch]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_b]])), read<u64, volatile>(%[[VALUE_count_15]]), read<u64, volatile>(%[[VALUE_count_15]]), function_decay<ptr<fn(ptr<const void>, ptr<const void>) -> i32>>(%[[VALUE_compare]])), int_to_ptr<ptr<void>, reason=explicit>(const<i32>(56)))
// DEFAULT-NEXT:                     return const<i32>(57);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         return conditional<i32>(eq<i32>(read<i32>(%[[VALUE_project_state]]), const<i32>(80)), const<i32>(1), const<i32>(58));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
