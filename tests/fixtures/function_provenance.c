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

// REWRITES-LABEL: {{^}}fn exit(status: i32) {
// REWRITES: {{^}}}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut project_state: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut number: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut real: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut ptr: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut thread: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     end = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(count), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(number), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 1.0;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(real), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-NEXT:     thread = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     strlen_call = unsafe {
// COMMON-LOWERING-NEXT:             strlen as *const (),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = malloc({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     ptr = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = ptr;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = calloc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = ptr;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:                         realloc({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = ptr;
// COMMON-LOWERING-NEXT:     free({{__v[0-9]+}} as *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = unsafe { project_state };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 77;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = memcpy(
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                     );
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = memmove(
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                     );
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:                         memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// COMMON-LOWERING-NEXT:                         memchr({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 24;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = strlen({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 12;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 13;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 14;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = strcmp({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 15;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = strncmp({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 12;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 16;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = memcmp(
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                     );
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 13;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 17;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 18;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 19;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 20;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 21;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = strspn({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 31;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 22;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = strcspn({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 23;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = atoi({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 41;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 24;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = atol({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = 42;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 25;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = strtol({{__v[0-9]+}}, std::ptr::addr_of_mut!(end), {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = 43;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 26;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = strtoul({{__v[0-9]+}}, std::ptr::addr_of_mut!(end), {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 44;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 27;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = strtod({{__v[0-9]+}}, std::ptr::addr_of_mut!(end));
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 45.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 28;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = printf({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 51;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 29;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = puts({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 52;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 30;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = fopen({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     stream = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 48;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 31;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = fputs({{__v[0-9]+}}, {{__v[0-9]+}} as *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 54;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 32;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 40;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 33;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = fread(
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                     );
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 56;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 34;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = fwrite(
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                     );
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 57;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 35;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = fclose({{__v[0-9]+}} as *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 58;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 36;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = fflush({{__v[0-9]+}} as *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 59;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 37;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = remove({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 60;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 38;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = toupper({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 61;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 39;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = tolower({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 62;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 40;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = sin({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 63.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 41;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = cos({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 64.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = tan({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 65.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 43;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = log({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 66.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 44;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = log10({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 67.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 45;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = log2({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 68.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 46;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = pow({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 69.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 47;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = sqrt({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 70.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 48;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = exp({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 71.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 49;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = exp2({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 72.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 50;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = fmod({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = 73.0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 51;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = lround({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = 74;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 52;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = llround({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i64 = 75;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 53;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = ptr;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = pthread_create(
// COMMON-LOWERING-NEXT:                         std::ptr::addr_of_mut!(thread),
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         Some(start),
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                     );
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 78;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 54;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = thread;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = pthread_join({{__v[0-9]+}}, std::ptr::addr_of_mut!(ptr));
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 79;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 55;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:     qsort(
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         Some(compare),
// COMMON-LOWERING-NEXT:     );
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = unsafe { project_state };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 76;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 56;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = bsearch(
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:                         Some(compare),
// COMMON-LOWERING-NEXT:                     );
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: u64 = 56;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 57;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     exit({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { project_state };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 80;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 58;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn malloc({{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn calloc({{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     let mut size: u64 = 0;
// COMMON-LOWERING-NEXT:     size = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = size;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn realloc({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     let mut size: u64 = 0;
// COMMON-LOWERING-NEXT:     size = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = size;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn free({{arg[0-9]+}}: *mut core::ffi::c_void) {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 77;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         project_state = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn memcpy(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT: ) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     let mut dst: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut src: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     dst = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     src = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = src;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = dst;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn memmove(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT: ) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     let mut dst: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut src: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     dst = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     src = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = src;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = dst;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn memset({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     let mut dst: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     dst = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = dst;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn memchr({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     let mut value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 24;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     dst = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     dst = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     dst = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     dst = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 12;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn memcmp({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64) -> i32 {
// COMMON-LOWERING-NEXT:     let mut rhs: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = rhs;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 13;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut value: i32 = 0;
// COMMON-LOWERING-NEXT:     value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut value: i32 = 0;
// COMMON-LOWERING-NEXT:     value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     needle = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     accept = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     accept = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 31;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     reject = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 41;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut base: i32 = 0;
// COMMON-LOWERING-NEXT:     end = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     base = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = base;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 43;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut base: i32 = 0;
// COMMON-LOWERING-NEXT:     end = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     base = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = base;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 44;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     end = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 45.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 51;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 52;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     mode = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 48;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     stream = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 54;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let mut count: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     stream = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 40;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn fread(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: ) -> u64 {
// COMMON-LOWERING-NEXT:     let mut size: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     size = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     stream = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = size;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 56;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn fwrite(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: ) -> u64 {
// COMMON-LOWERING-NEXT:     let mut size: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     size = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     stream = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = size;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = stream;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 57;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn fclose({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 58;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn fflush({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 59;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 60;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn toupper({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 61;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn tolower({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 62;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn sin({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 63.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn cos({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 64.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn tan({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 65.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn log({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 66.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn log10({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 67.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn log2({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 68.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pow({{arg[0-9]+}}: f64, {{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let mut rhs: f64 = 0.0;
// COMMON-LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = rhs;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 69.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn sqrt({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 70.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn exp({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 71.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn exp2({{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 72.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn fmod({{arg[0-9]+}}: f64, {{arg[0-9]+}}: f64) -> f64 {
// COMMON-LOWERING-NEXT:     let mut rhs: f64 = 0.0;
// COMMON-LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: f64 = rhs;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 73.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f64 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn lround({{arg[0-9]+}}: f64) -> i64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 74;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn llround({{arg[0-9]+}}: f64) -> i64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 75;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pthread_create(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void>,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: ) -> i32 {
// COMMON-LOWERING-NEXT:     let mut attr: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut start: Option<
// COMMON-LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     > = None;
// COMMON-LOWERING-NEXT:     let mut arg: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     attr = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     start = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     arg = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = attr;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: Option<
// COMMON-LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         > = start;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_some();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = arg;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 78;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn start({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     return {{arg[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn pthread_join({{arg[0-9]+}}: u64, {{arg[0-9]+}}: *mut *mut core::ffi::c_void) -> i32 {
// COMMON-LOWERING-NEXT:     let mut result: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     result = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut *mut core::ffi::c_void = result;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 79;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn qsort(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: Option<
// COMMON-LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:     >,
// COMMON-LOWERING-NEXT: ) {
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut size: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut callback: Option<
// COMMON-LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:     > = None;
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     size = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     callback = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = size;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: Option<
// COMMON-LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:         > = callback;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_some();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 76;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         project_state = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn compare({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} == {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn bsearch(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: Option<
// COMMON-LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:     >,
// COMMON-LOWERING-NEXT: ) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     let mut base: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut count: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut size: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut callback: Option<
// COMMON-LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:     > = None;
// COMMON-LOWERING-NEXT:     base = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     count = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     size = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     callback = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = base;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = count;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = size;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: Option<
// COMMON-LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-LOWERING-NEXT:         > = callback;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_some();
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 56;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn exit({{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 80;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         project_state = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut a: [i8; 8] = [0; 8];
// LOWERING-X86_64-GNU-NEXT:     let mut b: [i8; 8] = [0; 8];
// LOWERING-X86_64-GNU-NEXT:     let mut end: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut strlen_call: Option<unsafe extern "C-unwind" fn(*mut i8) -> u64> = None;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 8] = [97, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 8] = [98, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(*mut i8) -> u64>>(
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut i8) -> u64> = strlen_call;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = strcpy({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = strcat({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = strncpy({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = strncat({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = strchr({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = strrchr({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = strstr({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = strpbrk({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = fgets({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT: extern "C-unwind" fn strlen({{arg[0-9]+}}: *mut i8) -> u64 {
// LOWERING-X86_64-GNU-NEXT: fn strcpy({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let mut dst: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = dst;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT: fn strcat({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let mut dst: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = dst;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT: fn strncpy({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let mut dst: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = dst;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT: fn strncat({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let mut dst: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = dst;
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT: fn strcmp({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut rhs: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = rhs;
// LOWERING-X86_64-GNU-NEXT: fn strncmp({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut rhs: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = rhs;
// LOWERING-X86_64-GNU-NEXT: fn strchr({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT: fn strrchr({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT: fn strstr({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let mut needle: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = needle;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT: fn strpbrk({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let mut accept: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = accept;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT: fn strspn({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let mut accept: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = accept;
// LOWERING-X86_64-GNU-NEXT: fn strcspn({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let mut reject: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = reject;
// LOWERING-X86_64-GNU-NEXT: fn atoi({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-X86_64-GNU-NEXT: fn atol({{arg[0-9]+}}: *mut i8) -> i64 {
// LOWERING-X86_64-GNU-NEXT: fn strtol({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut *mut i8, {{arg[0-9]+}}: i32) -> i64 {
// LOWERING-X86_64-GNU-NEXT:     let mut end: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT: fn strtoul({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut *mut i8, {{arg[0-9]+}}: i32) -> u64 {
// LOWERING-X86_64-GNU-NEXT:     let mut end: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT: fn strtod({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut *mut i8) -> f64 {
// LOWERING-X86_64-GNU-NEXT:     let mut end: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT: fn printf({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-X86_64-GNU-NEXT: fn puts({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-X86_64-GNU-NEXT: fn fopen({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut core::ffi::c_void {
// LOWERING-X86_64-GNU-NEXT:     let mut mode: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = mode;
// LOWERING-X86_64-GNU-NEXT: fn fputs({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-X86_64-GNU-NEXT: fn fgets({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = {{__v[0-9]+}} as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT: fn remove({{arg[0-9]+}}: *mut i8) -> i32 {
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut a: [u8; 8] = [0; 8];
// LOWERING-AARCH64-GNU-NEXT:     let mut b: [u8; 8] = [0; 8];
// LOWERING-AARCH64-GNU-NEXT:     let mut end: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut strlen_call: Option<unsafe extern "C-unwind" fn(*mut u8) -> u64> = None;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 8] = [97, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 8] = [98, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(*mut u8) -> u64>>(
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut u8) -> u64> = strlen_call;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = strcpy({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = strcat({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = strncpy({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = strncat({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = strchr({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = strrchr({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = strstr({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = strpbrk({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = fgets({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT: extern "C-unwind" fn strlen({{arg[0-9]+}}: *mut u8) -> u64 {
// LOWERING-AARCH64-GNU-NEXT: fn strcpy({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let mut dst: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} {
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = dst;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT: fn strcat({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let mut dst: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} {
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = dst;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT: fn strncpy({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: u64) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let mut dst: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} {
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = dst;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT: fn strncat({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: u64) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let mut dst: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} {
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = dst;
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT: fn strcmp({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> i32 {
// LOWERING-AARCH64-GNU-NEXT:     let mut rhs: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = rhs;
// LOWERING-AARCH64-GNU-NEXT: fn strncmp({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-AARCH64-GNU-NEXT:     let mut rhs: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = rhs;
// LOWERING-AARCH64-GNU-NEXT: fn strchr({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT: fn strrchr({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT: fn strstr({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let mut needle: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = needle;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT: fn strpbrk({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let mut accept: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = accept;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT: fn strspn({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> u64 {
// LOWERING-AARCH64-GNU-NEXT:     let mut accept: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = accept;
// LOWERING-AARCH64-GNU-NEXT: fn strcspn({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> u64 {
// LOWERING-AARCH64-GNU-NEXT:     let mut reject: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = reject;
// LOWERING-AARCH64-GNU-NEXT: fn atoi({{arg[0-9]+}}: *mut u8) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn atol({{arg[0-9]+}}: *mut u8) -> i64 {
// LOWERING-AARCH64-GNU-NEXT: fn strtol({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut *mut u8, {{arg[0-9]+}}: i32) -> i64 {
// LOWERING-AARCH64-GNU-NEXT:     let mut end: *mut *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut u8 = end;
// LOWERING-AARCH64-GNU-NEXT: fn strtoul({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut *mut u8, {{arg[0-9]+}}: i32) -> u64 {
// LOWERING-AARCH64-GNU-NEXT:     let mut end: *mut *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut u8 = end;
// LOWERING-AARCH64-GNU-NEXT: fn strtod({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut *mut u8) -> f64 {
// LOWERING-AARCH64-GNU-NEXT:     let mut end: *mut *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut *mut u8 = end;
// LOWERING-AARCH64-GNU-NEXT: fn printf({{arg[0-9]+}}: *mut u8) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn puts({{arg[0-9]+}}: *mut u8) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn fopen({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut u8) -> *mut core::ffi::c_void {
// LOWERING-AARCH64-GNU-NEXT:     let mut mode: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = mode;
// LOWERING-AARCH64-GNU-NEXT: fn fputs({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-AARCH64-GNU-NEXT: fn fgets({{arg[0-9]+}}: *mut u8, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut u8 {
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{__v[0-9]+}} as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-AARCH64-GNU-NEXT: fn remove({{arg[0-9]+}}: *mut u8) -> i32 {
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut project_state: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut count: u64 = 0;
// COMMON-REWRITES-NEXT:     let mut number: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut real: f64 = 0.0;
// COMMON-REWRITES-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut ptr: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     let mut thread: u64 = 1;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(count), 1 as u64) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(number), 1 as i32) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(real), 1.0 as f64) };
// COMMON-REWRITES-NEXT:     strlen_call = unsafe {
// COMMON-REWRITES-NEXT:             strlen as *const (),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:     ptr = malloc({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = ptr != ((16 as u64) as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 2;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = calloc({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((16 as u64) as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 3;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = ptr;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = realloc({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((16 as u64) as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 4;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     free(ptr as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (unsafe { project_state }) != 77;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 5;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = memcpy(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         );
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ({{__v[0-9]+}} as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 6;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = memmove(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         );
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ({{__v[0-9]+}} as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 7;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = memset({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ({{__v[0-9]+}} as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 8;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = memchr({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((24 as u64) as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 9;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = strlen({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 5;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 10;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 5;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 10;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 11;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 12;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 13;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 14;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = strcmp({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 11;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 15;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = strncmp({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 12;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 16;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = memcmp(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         );
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 13;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 17;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 18;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 19;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 20;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 21;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = strspn({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 31;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 22;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = strcspn({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 32;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 23;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = atoi({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 41;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 24;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = atol({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 42;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 25;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = strtol({{__v[0-9]+}}, std::ptr::addr_of_mut!(end), {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 43;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 26;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = strtoul({{__v[0-9]+}}, std::ptr::addr_of_mut!(end), {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 44;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 27;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = strtod({{__v[0-9]+}}, std::ptr::addr_of_mut!(end));
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 45.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 28;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = printf({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 51;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 29;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = puts({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 52;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 30;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     stream = fopen({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = stream != ((48 as u64) as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 31;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = fputs({{__v[0-9]+}}, stream as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 54;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 32;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 33;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = fread(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             stream as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         );
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 56;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 34;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = fwrite(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             stream as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         );
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 57;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 35;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = fclose(stream as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 58;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 36;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = fflush(stream as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 59;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 37;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = remove({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 60;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 38;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = toupper({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 61;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 39;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = tolower({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 62;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 40;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = sin({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 63.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 41;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = cos({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 64.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 42;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = tan({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 65.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 43;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = log({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 66.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 44;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = log10({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 67.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 45;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = log2({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 68.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 46;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = pow({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 69.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 47;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = sqrt({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 70.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 48;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = exp({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 71.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 49;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = exp2({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 72.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 50;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = fmod({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 73.0;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 51;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = lround({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 74;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 52;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = llround({{__v[0-9]+}});
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 75;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 53;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = pthread_create(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(thread),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             Some(start),
// COMMON-REWRITES-NEXT:             ptr as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         );
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 78;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 54;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = pthread_join(thread, std::ptr::addr_of_mut!(ptr));
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 79;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 55;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     qsort(
// COMMON-REWRITES-NEXT:         a.as_mut_ptr() as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) },
// COMMON-REWRITES-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) },
// COMMON-REWRITES-NEXT:         Some(compare),
// COMMON-REWRITES-NEXT:     );
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = (unsafe { project_state }) != 76;
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 56;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = bsearch(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             Some(compare),
// COMMON-REWRITES-NEXT:         );
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((56 as u64) as *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:         if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:             __retval = 57;
// COMMON-REWRITES-NEXT:             std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:         if !({{__v[0-9]+}} != 0) {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     exit(0);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 58;
// COMMON-REWRITES-NEXT:     __retval = if (unsafe { project_state }) == 80 {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     }) as u64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn malloc({{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = (16 as u64) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn calloc({{arg[0-9]+}}: u64, mut size: u64) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != 0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = size != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = (16 as u64) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn realloc({{arg[0-9]+}}: *mut core::ffi::c_void, mut size: u64) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = size != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = (16 as u64) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn free({{arg[0-9]+}}: *mut core::ffi::c_void) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 77;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         project_state = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn memcpy(
// COMMON-REWRITES-NEXT:     mut dst: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     mut src: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-REWRITES-NEXT: ) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != 0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = src != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = dst;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn memmove(
// COMMON-REWRITES-NEXT:     mut dst: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     mut src: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-REWRITES-NEXT: ) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != 0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = src != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = dst;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn memset(mut dst: *mut core::ffi::c_void, {{arg[0-9]+}}: i32, mut count: u64) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != 0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = dst;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn memchr({{arg[0-9]+}}: *mut core::ffi::c_void, mut value: i32, mut count: u64) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = value != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = (24 as u64) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = rhs != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 11;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = rhs != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 12;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn memcmp({{arg[0-9]+}}: *mut core::ffi::c_void, mut rhs: *mut core::ffi::c_void, mut count: u64) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = rhs != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 13;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = value != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = value != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = needle != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = accept != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = accept != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 31;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }) as u64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = reject != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }) as u64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 41;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     }) as i64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = end != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = base != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 43;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }) as i64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = end != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = base != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 44;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }) as u64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = end != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 45.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 51;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 52;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = mode != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = (48 as u64) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = stream != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 54;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = stream != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn fread(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     mut size: u64,
// COMMON-REWRITES-NEXT:     mut count: u64,
// COMMON-REWRITES-NEXT:     mut stream: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: ) -> u64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = size != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = stream != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 56;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }) as u64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn fwrite(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     mut size: u64,
// COMMON-REWRITES-NEXT:     mut count: u64,
// COMMON-REWRITES-NEXT:     mut stream: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: ) -> u64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = size != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = stream != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 57;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }) as u64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn fclose({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 58;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn fflush({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 59;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 60;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn toupper({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 61;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn tolower({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 62;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn sin({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 63.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn cos({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 64.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn tan({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 65.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn log({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 66.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn log10({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 67.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn log2({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 68.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pow({{arg[0-9]+}}: f64, {{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let mut rhs: f64 = 0.0;
// COMMON-REWRITES-NEXT:     rhs = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != 0.0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = rhs != 0.0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 69.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn sqrt({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 70.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn exp({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 71.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn exp2({{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 72.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn fmod({{arg[0-9]+}}: f64, {{arg[0-9]+}}: f64) -> f64 {
// COMMON-REWRITES-NEXT:     let mut rhs: f64 = 0.0;
// COMMON-REWRITES-NEXT:     rhs = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != 0.0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = rhs != 0.0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 73.0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: f64 = 0.0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn lround({{arg[0-9]+}}: f64) -> i64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 74;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }) as i64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn llround({{arg[0-9]+}}: f64) -> i64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 75;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     (if {{arg[0-9]+}} != 0.0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} }) as i64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pthread_create(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut u64,
// COMMON-REWRITES-NEXT:     mut attr: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     mut start: Option<
// COMMON-REWRITES-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     >,
// COMMON-REWRITES-NEXT:     mut arg: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: ) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = !(attr != std::ptr::null_mut());
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = start.is_some();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = arg != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 78;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn start({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn pthread_join({{arg[0-9]+}}: u64, mut result: *mut *mut core::ffi::c_void) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != 0 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = result != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 79;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn qsort(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     mut count: u64,
// COMMON-REWRITES-NEXT:     mut size: u64,
// COMMON-REWRITES-NEXT:     mut callback: Option<
// COMMON-REWRITES-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-REWRITES-NEXT:     >,
// COMMON-REWRITES-NEXT: ) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = size != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = callback.is_some();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 76;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         project_state = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn compare({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     if {{arg[0-9]+}} == {{arg[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn bsearch(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     mut base: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     mut count: u64,
// COMMON-REWRITES-NEXT:     mut size: u64,
// COMMON-REWRITES-NEXT:     mut callback: Option<
// COMMON-REWRITES-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// COMMON-REWRITES-NEXT:     >,
// COMMON-REWRITES-NEXT: ) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{arg[0-9]+}} != std::ptr::null_mut() {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = base != std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = count != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = size != 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = callback.is_some();
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = (56 as u64) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn exit({{arg[0-9]+}}: i32) {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         project_state = {{arg[0-9]+}} + 80;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut a: [i8; 8] = [97, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut b: [i8; 8] = [98, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-X86_64-GNU-NEXT:     let mut end: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let mut strlen_call: Option<unsafe extern "C-unwind" fn(*mut i8) -> u64> = None;
// REWRITES-X86_64-GNU-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(*mut i8) -> u64>>(
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut i8) -> u64> = strlen_call;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = strcpy({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = strcat({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = strncpy({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = strncat({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = strchr({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((32 as u64) as *mut i8);
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = strrchr({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((32 as u64) as *mut i8);
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = strstr({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((32 as u64) as *mut i8);
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = strpbrk({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((32 as u64) as *mut i8);
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = fgets({{__v[0-9]+}}, {{__v[0-9]+}}, stream as *mut core::ffi::c_void);
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((40 as u64) as *mut i8);
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT: extern "C-unwind" fn strlen({{arg[0-9]+}}: *mut i8) -> u64 {
// REWRITES-X86_64-GNU-NEXT: fn strcpy(mut dst: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{arg[0-9]+}} != std::ptr::null_mut() {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = dst;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT: fn strcat(mut dst: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{arg[0-9]+}} != std::ptr::null_mut() {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = dst;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT: fn strncpy(mut dst: *mut i8, {{arg[0-9]+}}: *mut i8, mut count: u64) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = dst;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT: fn strncat(mut dst: *mut i8, {{arg[0-9]+}}: *mut i8, mut count: u64) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = if {{__v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = dst;
// REWRITES-X86_64-GNU-NEXT:         let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT: fn strcmp({{arg[0-9]+}}: *mut i8, mut rhs: *mut i8) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn strncmp({{arg[0-9]+}}: *mut i8, mut rhs: *mut i8, mut count: u64) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn strchr({{arg[0-9]+}}: *mut i8, mut value: i32) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (32 as u64) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT: fn strrchr({{arg[0-9]+}}: *mut i8, mut value: i32) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (32 as u64) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT: fn strstr({{arg[0-9]+}}: *mut i8, mut needle: *mut i8) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (32 as u64) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT: fn strpbrk({{arg[0-9]+}}: *mut i8, mut accept: *mut i8) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (32 as u64) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT: fn strspn({{arg[0-9]+}}: *mut i8, mut accept: *mut i8) -> u64 {
// REWRITES-X86_64-GNU-NEXT: fn strcspn({{arg[0-9]+}}: *mut i8, mut reject: *mut i8) -> u64 {
// REWRITES-X86_64-GNU-NEXT: fn atoi({{arg[0-9]+}}: *mut i8) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn atol({{arg[0-9]+}}: *mut i8) -> i64 {
// REWRITES-X86_64-GNU-NEXT: fn strtol({{arg[0-9]+}}: *mut i8, mut end: *mut *mut i8, mut base: i32) -> i64 {
// REWRITES-X86_64-GNU-NEXT: fn strtoul({{arg[0-9]+}}: *mut i8, mut end: *mut *mut i8, mut base: i32) -> u64 {
// REWRITES-X86_64-GNU-NEXT: fn strtod({{arg[0-9]+}}: *mut i8, mut end: *mut *mut i8) -> f64 {
// REWRITES-X86_64-GNU-NEXT: fn printf({{arg[0-9]+}}: *mut i8) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn puts({{arg[0-9]+}}: *mut i8) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn fopen({{arg[0-9]+}}: *mut i8, mut mode: *mut i8) -> *mut core::ffi::c_void {
// REWRITES-X86_64-GNU-NEXT: fn fputs({{arg[0-9]+}}: *mut i8, mut stream: *mut core::ffi::c_void) -> i32 {
// REWRITES-X86_64-GNU-NEXT: fn fgets({{arg[0-9]+}}: *mut i8, mut count: i32, mut stream: *mut core::ffi::c_void) -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (40 as u64) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT: fn remove({{arg[0-9]+}}: *mut i8) -> i32 {
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut a: [u8; 8] = [97, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut b: [u8; 8] = [98, 0, 0, 0, 0, 0, 0, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut end: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut strlen_call: Option<unsafe extern "C-unwind" fn(*mut u8) -> u64> = None;
// REWRITES-AARCH64-GNU-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(*mut u8) -> u64>>(
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut u8) -> u64> = strlen_call;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = strcpy({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = strcat({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = strncpy({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = strncat({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = strchr({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((32 as u64) as *mut u8);
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = strrchr({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((32 as u64) as *mut u8);
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = strstr({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((32 as u64) as *mut u8);
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = strpbrk({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((32 as u64) as *mut u8);
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = fgets({{__v[0-9]+}}, {{__v[0-9]+}}, stream as *mut core::ffi::c_void);
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((40 as u64) as *mut u8);
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = a.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = b.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT: extern "C-unwind" fn strlen({{arg[0-9]+}}: *mut u8) -> u64 {
// REWRITES-AARCH64-GNU-NEXT: fn strcpy(mut dst: *mut u8, {{arg[0-9]+}}: *mut u8) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{arg[0-9]+}} != std::ptr::null_mut() {
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = dst;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT: fn strcat(mut dst: *mut u8, {{arg[0-9]+}}: *mut u8) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{arg[0-9]+}} != std::ptr::null_mut() {
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = dst;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT: fn strncpy(mut dst: *mut u8, {{arg[0-9]+}}: *mut u8, mut count: u64) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = dst;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT: fn strncat(mut dst: *mut u8, {{arg[0-9]+}}: *mut u8, mut count: u64) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = if {{__v[0-9]+}} {
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = dst;
// REWRITES-AARCH64-GNU-NEXT:         let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT: fn strcmp({{arg[0-9]+}}: *mut u8, mut rhs: *mut u8) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn strncmp({{arg[0-9]+}}: *mut u8, mut rhs: *mut u8, mut count: u64) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn strchr({{arg[0-9]+}}: *mut u8, mut value: i32) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (32 as u64) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT: fn strrchr({{arg[0-9]+}}: *mut u8, mut value: i32) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (32 as u64) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT: fn strstr({{arg[0-9]+}}: *mut u8, mut needle: *mut u8) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (32 as u64) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT: fn strpbrk({{arg[0-9]+}}: *mut u8, mut accept: *mut u8) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (32 as u64) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT: fn strspn({{arg[0-9]+}}: *mut u8, mut accept: *mut u8) -> u64 {
// REWRITES-AARCH64-GNU-NEXT: fn strcspn({{arg[0-9]+}}: *mut u8, mut reject: *mut u8) -> u64 {
// REWRITES-AARCH64-GNU-NEXT: fn atoi({{arg[0-9]+}}: *mut u8) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn atol({{arg[0-9]+}}: *mut u8) -> i64 {
// REWRITES-AARCH64-GNU-NEXT: fn strtol({{arg[0-9]+}}: *mut u8, mut end: *mut *mut u8, mut base: i32) -> i64 {
// REWRITES-AARCH64-GNU-NEXT: fn strtoul({{arg[0-9]+}}: *mut u8, mut end: *mut *mut u8, mut base: i32) -> u64 {
// REWRITES-AARCH64-GNU-NEXT: fn strtod({{arg[0-9]+}}: *mut u8, mut end: *mut *mut u8) -> f64 {
// REWRITES-AARCH64-GNU-NEXT: fn printf({{arg[0-9]+}}: *mut u8) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn puts({{arg[0-9]+}}: *mut u8) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn fopen({{arg[0-9]+}}: *mut u8, mut mode: *mut u8) -> *mut core::ffi::c_void {
// REWRITES-AARCH64-GNU-NEXT: fn fputs({{arg[0-9]+}}: *mut u8, mut stream: *mut core::ffi::c_void) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn fgets({{arg[0-9]+}}: *mut u8, mut count: i32, mut stream: *mut core::ffi::c_void) -> *mut u8 {
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (40 as u64) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT: fn remove({{arg[0-9]+}}: *mut u8) -> i32 {
// SLATE-FILECHECK-END rewrites-aarch64-gnu
