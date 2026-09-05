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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut project_state: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut a: [i8; 8] = [0; 8];
// LOWERING-NEXT:     let mut b: [i8; 8] = [0; 8];
// LOWERING-NEXT:     let mut end: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     let mut number: i32 = 0;
// LOWERING-NEXT:     let mut real: f64 = 0.0;
// LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut ptr: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut thread: u64 = 0;
// LOWERING-NEXT:     let mut strlen_call: Option<unsafe extern "C-unwind" fn(*mut i8) -> u64> = None;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i8; 8] = [97, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i8; 8] = [98, 0, 0, 0, 0, 0, 0, 0];
// LOWERING-NEXT:     b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     end = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(count), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(number), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(real), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 1;
// LOWERING-NEXT:     thread = {{_v[0-9]+}};
// LOWERING-NEXT:     strlen_call = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(*mut i8) -> u64>>(
// LOWERING-NEXT:             strlen as *const (),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = malloc({{_v[0-9]+}});
// LOWERING-NEXT:     ptr = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = ptr;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = calloc({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = ptr;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:                         realloc({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = ptr;
// LOWERING-NEXT:     free({{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { project_state };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 77;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = memcpy(
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}},
// LOWERING-NEXT:                     );
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = memmove(
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}},
// LOWERING-NEXT:                     );
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:                         memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:                         memchr({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 24;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 9;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = strlen({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut i8) -> u64> = strlen_call;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = strcpy({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = strcat({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 12;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = strncpy({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 13;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = strncat({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 14;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = strcmp({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 15;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = strncmp({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 12;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 16;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = memcmp(
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}},
// LOWERING-NEXT:                     );
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 13;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 17;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = strchr({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 18;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = strrchr({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 19;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = strstr({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = strpbrk({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 21;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = strspn({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 31;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 22;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = strcspn({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 23;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = atoi({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 24;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = atol({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = 42;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 25;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = strtol({{_v[0-9]+}}, std::ptr::addr_of_mut!(end), {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = 43;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 26;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = strtoul({{_v[0-9]+}}, std::ptr::addr_of_mut!(end), {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 44;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 27;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = strtod({{_v[0-9]+}}, std::ptr::addr_of_mut!(end));
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 45.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 28;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = printf({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 51;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 29;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = puts({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 52;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 30;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = fopen({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     stream = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 48;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 31;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = fputs({{_v[0-9]+}}, {{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 54;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 32;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = fgets({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 40;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 33;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = fread(
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}},
// LOWERING-NEXT:                         {{_v[0-9]+}},
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                     );
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 56;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 34;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = fwrite(
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}},
// LOWERING-NEXT:                         {{_v[0-9]+}},
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                     );
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 57;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 35;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = fclose({{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 58;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 36;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = fflush({{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 59;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 37;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = remove({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 60;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 38;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = toupper({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 61;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 39;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = tolower({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 62;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 40;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = sin({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 63.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = cos({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 64.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = tan({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 65.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 43;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = log({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 66.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 44;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = log10({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 67.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 45;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = log2({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 68.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 46;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = pow({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 69.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 47;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = sqrt({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 70.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 48;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = exp({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 71.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 49;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = exp2({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 72.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 50;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = fmod({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = 73.0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 51;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = lround({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = 74;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 52;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = llround({{_v[0-9]+}});
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i64 = 75;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 53;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = ptr;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = pthread_create(
// LOWERING-NEXT:                         std::ptr::addr_of_mut!(thread),
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         Some(start),
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                     );
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 78;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 54;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = thread;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = pthread_join({{_v[0-9]+}}, std::ptr::addr_of_mut!(ptr));
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 79;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 55;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:     qsort({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}}, {{_v[0-9]+}}, Some(compare));
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { project_state };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 76;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 56;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = a.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = bsearch(
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                         {{_v[0-9]+}},
// LOWERING-NEXT:                         {{_v[0-9]+}},
// LOWERING-NEXT:                         Some(compare),
// LOWERING-NEXT:                     );
// LOWERING-NEXT:                     let {{_v[0-9]+}}: u64 = 56;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 57;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     exit({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { project_state };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 80;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 58;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn strlen({{arg[0-9]+}}: *mut i8) -> u64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn malloc({{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn calloc({{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     size = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = size;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn realloc({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     size = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = size;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn free({{arg[0-9]+}}: *mut core::ffi::c_void) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 77;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         project_state = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn memcpy(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT: ) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut dst: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut src: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     dst = {{arg[0-9]+}};
// LOWERING-NEXT:     src = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = src;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = dst;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn memmove(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT: ) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut dst: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut src: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     dst = {{arg[0-9]+}};
// LOWERING-NEXT:     src = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = src;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = dst;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn memset({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut dst: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     dst = {{arg[0-9]+}};
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = dst;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn memchr({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: u64) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 24;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strcpy({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// LOWERING-NEXT:     let mut dst: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     dst = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = dst;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strcat({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// LOWERING-NEXT:     let mut dst: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     dst = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = dst;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strncpy({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> *mut i8 {
// LOWERING-NEXT:     let mut dst: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     dst = {{arg[0-9]+}};
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = dst;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strncat({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> *mut i8 {
// LOWERING-NEXT:     let mut dst: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     dst = {{arg[0-9]+}};
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = dst;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strcmp({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-NEXT:     let mut rhs: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = rhs;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strncmp({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-NEXT:     let mut rhs: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = rhs;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 12;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn memcmp({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-NEXT:     let mut rhs: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = rhs;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 13;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strchr({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> *mut i8 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strrchr({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32) -> *mut i8 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strstr({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// LOWERING-NEXT:     let mut needle: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     needle = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = needle;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strpbrk({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut i8 {
// LOWERING-NEXT:     let mut accept: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     accept = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = accept;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strspn({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> u64 {
// LOWERING-NEXT:     let mut accept: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     accept = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = accept;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 31;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strcspn({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> u64 {
// LOWERING-NEXT:     let mut reject: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     reject = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = reject;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn atoi({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn atol({{arg[0-9]+}}: *mut i8) -> i64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strtol({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut *mut i8, {{arg[0-9]+}}: i32) -> i64 {
// LOWERING-NEXT:     let mut end: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut base: i32 = 0;
// LOWERING-NEXT:     end = {{arg[0-9]+}};
// LOWERING-NEXT:     base = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut *mut i8 = end;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = base;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 43;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strtoul({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut *mut i8, {{arg[0-9]+}}: i32) -> u64 {
// LOWERING-NEXT:     let mut end: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut base: i32 = 0;
// LOWERING-NEXT:     end = {{arg[0-9]+}};
// LOWERING-NEXT:     base = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut *mut i8 = end;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = base;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 44;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn strtod({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut *mut i8) -> f64 {
// LOWERING-NEXT:     let mut end: *mut *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     end = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut *mut i8 = end;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 45.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn printf({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 51;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn puts({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 52;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fopen({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut i8) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut mode: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     mode = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut i8 = mode;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 48;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fputs({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     stream = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 54;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fgets({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut i8 {
// LOWERING-NEXT:     let mut count: i32 = 0;
// LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     stream = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 40;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fread(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT: ) -> u64 {
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     size = {{arg[0-9]+}};
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     stream = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = size;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 56;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fwrite(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT: ) -> u64 {
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     let mut stream: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     size = {{arg[0-9]+}};
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     stream = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = size;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = stream;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 57;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fclose({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 58;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fflush({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 59;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn remove({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 60;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn toupper({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 61;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn tolower({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 62;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sin({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 63.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn cos({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 64.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn tan({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 65.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn log({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 66.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn log10({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 67.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn log2({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 68.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pow({{arg[0-9]+}}: f64, {{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let mut rhs: f64 = 0.0;
// LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = rhs;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0.0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 69.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sqrt({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 70.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn exp({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 71.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn exp2({{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 72.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fmod({{arg[0-9]+}}: f64, {{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let mut rhs: f64 = 0.0;
// LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: f64 = rhs;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0.0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 73.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn lround({{arg[0-9]+}}: f64) -> i64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 74;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn llround({{arg[0-9]+}}: f64) -> i64 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 75;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pthread_create(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void>,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT: ) -> i32 {
// LOWERING-NEXT:     let mut attr: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut start: Option<
// LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void,
// LOWERING-NEXT:     > = None;
// LOWERING-NEXT:     let mut arg: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     attr = {{arg[0-9]+}};
// LOWERING-NEXT:     start = {{arg[0-9]+}};
// LOWERING-NEXT:     arg = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = attr;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<
// LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void,
// LOWERING-NEXT:         > = start;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.is_some();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = arg;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 78;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn start({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     return {{arg[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn pthread_join({{arg[0-9]+}}: u64, {{arg[0-9]+}}: *mut *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let mut result: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     result = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = result;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 79;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn qsort(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: Option<
// LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:     >,
// LOWERING-NEXT: ) {
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     let mut callback: Option<
// LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:     > = None;
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     size = {{arg[0-9]+}};
// LOWERING-NEXT:     callback = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = size;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<
// LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:         > = callback;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.is_some();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 76;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         project_state = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn compare({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} == {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn bsearch(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: Option<
// LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:     >,
// LOWERING-NEXT: ) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut base: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut count: u64 = 0;
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     let mut callback: Option<
// LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:     > = None;
// LOWERING-NEXT:     base = {{arg[0-9]+}};
// LOWERING-NEXT:     count = {{arg[0-9]+}};
// LOWERING-NEXT:     size = {{arg[0-9]+}};
// LOWERING-NEXT:     callback = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = base;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != std::ptr::null_mut();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = count;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = size;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: Option<
// LOWERING-NEXT:             unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut core::ffi::c_void) -> i32,
// LOWERING-NEXT:         > = callback;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}}.is_some();
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 56;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn exit({{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 80;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         project_state = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering
