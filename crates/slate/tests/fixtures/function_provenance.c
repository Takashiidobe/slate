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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! CHECK__2 {
// CHECK-NEXT:     ($value:expr, $expected:expr, $code:expr) => {
// CHECK-NEXT:         { loop { if ($value) != (($expected) as u64) { return $code; } if true { break; }
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! CHECK__4 {
// CHECK-NEXT:     ($value:expr, $expected:expr, $code:expr) => {
// CHECK-NEXT:         { loop { if ($value) != (($expected) as * mut ()) { return $code; } if true {
// CHECK-NEXT:         break; } } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! CHECK__3 {
// CHECK-NEXT:     ($value:expr, $expected:expr, $code:expr) => {
// CHECK-NEXT:         { loop { if ($value) != (($expected) as i64) { return $code; } if true { break; }
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! CHECK {
// CHECK-NEXT:     ($value:expr, $expected:expr, $code:expr) => {
// CHECK-NEXT:         { loop { if ($value) != $expected { return $code; } if true { break; } } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: static mut project_state: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn malloc(size: u64) -> *mut () {
// CHECK-NEXT:     if size != 0 { 16i32 as *mut () } else { std::ptr::null_mut::<()>() }
// CHECK-NEXT: }
// CHECK-NEXT: fn calloc(count: u64, size: u64) -> *mut () {
// CHECK-NEXT:     if count != 0 && size != 0 { 16i32 as *mut () } else { std::ptr::null_mut::<()>() }
// CHECK-NEXT: }
// CHECK-NEXT: fn realloc(ptr: *mut (), size: u64) -> *mut () {
// CHECK-NEXT:     if !ptr.is_null() && size != 0 {
// CHECK-NEXT:         16i32 as *mut ()
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null_mut::<()>()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn free(ptr: *mut ()) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         project_state = if !ptr.is_null() { 77 } else { 0 };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn memcpy(dst: *mut (), src: *const (), count: u64) -> *mut () {
// CHECK-NEXT:     if count != 0 && !src.is_null() { dst } else { std::ptr::null_mut::<()>() }
// CHECK-NEXT: }
// CHECK-NEXT: fn memmove(dst: *mut (), src: *const (), count: u64) -> *mut () {
// CHECK-NEXT:     if count != 0 && !src.is_null() { dst } else { std::ptr::null_mut::<()>() }
// CHECK-NEXT: }
// CHECK-NEXT: fn memset(dst: *mut (), value: i32, count: u64) -> *mut () {
// CHECK-NEXT:     if value != 0 && count != 0 { dst } else { std::ptr::null_mut::<()>() }
// CHECK-NEXT: }
// CHECK-NEXT: fn memchr(ptr: *const (), value: i32, count: u64) -> *mut () {
// CHECK-NEXT:     if !ptr.is_null() && value != 0 && count != 0 {
// CHECK-NEXT:         24i32 as *mut ()
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null_mut::<()>()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.strlen")]
// CHECK-NEXT: extern "C-unwind" fn strlen(s: *const i8) -> u64 {
// CHECK-NEXT:     (if !s.is_null() { 5i32 } else { 0i32 }) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn strcpy(dst: *mut i8, src: *const i8) -> *mut i8 {
// CHECK-NEXT:     if !src.is_null() { dst } else { std::ptr::null_mut::<i8>() }
// CHECK-NEXT: }
// CHECK-NEXT: fn strcat(dst: *mut i8, src: *const i8) -> *mut i8 {
// CHECK-NEXT:     if !src.is_null() { dst } else { std::ptr::null_mut::<i8>() }
// CHECK-NEXT: }
// CHECK-NEXT: fn strncpy(dst: *mut i8, src: *const i8, count: u64) -> *mut i8 {
// CHECK-NEXT:     if !src.is_null() && count != 0 { dst } else { std::ptr::null_mut::<i8>() }
// CHECK-NEXT: }
// CHECK-NEXT: fn strncat(dst: *mut i8, src: *const i8, count: u64) -> *mut i8 {
// CHECK-NEXT:     if !src.is_null() && count != 0 { dst } else { std::ptr::null_mut::<i8>() }
// CHECK-NEXT: }
// CHECK-NEXT: fn strcmp(lhs: *const i8, rhs: *const i8) -> i32 {
// CHECK-NEXT:     if !lhs.is_null() && !rhs.is_null() { 11 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn strncmp(lhs: *const i8, rhs: *const i8, count: u64) -> i32 {
// CHECK-NEXT:     if !lhs.is_null() && !rhs.is_null() && count != 0 { 12 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn memcmp(lhs: *const (), rhs: *const (), count: u64) -> i32 {
// CHECK-NEXT:     if !lhs.is_null() && !rhs.is_null() && count != 0 { 13 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn strchr(s: *const i8, value: i32) -> *mut i8 {
// CHECK-NEXT:     if !s.is_null() && value != 0 {
// CHECK-NEXT:         32i32 as *mut i8
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null_mut::<i8>()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn strrchr(s: *const i8, value: i32) -> *mut i8 {
// CHECK-NEXT:     if !s.is_null() && value != 0 {
// CHECK-NEXT:         32i32 as *mut i8
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null_mut::<i8>()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn strstr(s: *const i8, needle: *const i8) -> *mut i8 {
// CHECK-NEXT:     if !s.is_null() && !needle.is_null() {
// CHECK-NEXT:         32i32 as *mut i8
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null_mut::<i8>()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn strpbrk(s: *const i8, accept: *const i8) -> *mut i8 {
// CHECK-NEXT:     if !s.is_null() && !accept.is_null() {
// CHECK-NEXT:         32i32 as *mut i8
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null_mut::<i8>()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn strspn(s: *const i8, accept: *const i8) -> u64 {
// CHECK-NEXT:     (if !s.is_null() && !accept.is_null() { 31i32 } else { 0i32 }) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn strcspn(s: *const i8, reject: *const i8) -> u64 {
// CHECK-NEXT:     (if !s.is_null() && !reject.is_null() { 32i32 } else { 0i32 }) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn atoi(s: *const i8) -> i32 {
// CHECK-NEXT:     if !s.is_null() { 41 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn atol(s: *const i8) -> i64 {
// CHECK-NEXT:     (if !s.is_null() { 42i32 } else { 0i32 }) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn strtol(s: *const i8, end: *mut *mut i8, base: i32) -> i64 {
// CHECK-NEXT:     (if !s.is_null() && !end.is_null() && base != 0 { 43i32 } else { 0i32 }) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn strtoul(s: *const i8, end: *mut *mut i8, base: i32) -> u64 {
// CHECK-NEXT:     (if !s.is_null() && !end.is_null() && base != 0 { 44i32 } else { 0i32 }) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn strtod(s: *const i8, end: *mut *mut i8) -> f64 {
// CHECK-NEXT:     if !s.is_null() && !end.is_null() { 45.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn printf(format: *const i8) -> i32 {
// CHECK-NEXT:     if !format.is_null() { 51 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn puts(s: *const i8) -> i32 {
// CHECK-NEXT:     if !s.is_null() { 52 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn fopen(path: *const i8, mode: *const i8) -> *mut () {
// CHECK-NEXT:     if !path.is_null() && !mode.is_null() {
// CHECK-NEXT:         48i32 as *mut ()
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null_mut::<()>()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn fputs(s: *const i8, stream: *mut ()) -> i32 {
// CHECK-NEXT:     if !s.is_null() && !stream.is_null() { 54 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn fgets(s: *mut i8, count: i32, stream: *mut ()) -> *mut i8 {
// CHECK-NEXT:     if !s.is_null() && count != 0 && !stream.is_null() {
// CHECK-NEXT:         40i32 as *mut i8
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null_mut::<i8>()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn fread(ptr: *mut (), size: u64, count: u64, stream: *mut ()) -> u64 {
// CHECK-NEXT:     (if !ptr.is_null() && size != 0 && count != 0 && !stream.is_null() {
// CHECK-NEXT:         56i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0i32
// CHECK-NEXT:     }) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn fwrite(ptr: *const (), size: u64, count: u64, stream: *mut ()) -> u64 {
// CHECK-NEXT:     (if !ptr.is_null() && size != 0 && count != 0 && !stream.is_null() {
// CHECK-NEXT:         57i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0i32
// CHECK-NEXT:     }) as u64
// CHECK-NEXT: }
// CHECK-NEXT: fn fclose(stream: *mut ()) -> i32 {
// CHECK-NEXT:     if !stream.is_null() { 58 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn fflush(stream: *mut ()) -> i32 {
// CHECK-NEXT:     if !stream.is_null() { 59 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn remove(path: *const i8) -> i32 {
// CHECK-NEXT:     if !path.is_null() { 60 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn toupper(value: i32) -> i32 {
// CHECK-NEXT:     if value != 0 { 61 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn tolower(value: i32) -> i32 {
// CHECK-NEXT:     if value != 0 { 62 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn sin(value: f64) -> f64 {
// CHECK-NEXT:     if value != 0.0f64 { 63.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn cos(value: f64) -> f64 {
// CHECK-NEXT:     if value != 0.0f64 { 64.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn tan(value: f64) -> f64 {
// CHECK-NEXT:     if value != 0.0f64 { 65.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn log(value: f64) -> f64 {
// CHECK-NEXT:     if value != 0.0f64 { 66.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn log10(value: f64) -> f64 {
// CHECK-NEXT:     if value != 0.0f64 { 67.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn log2(value: f64) -> f64 {
// CHECK-NEXT:     if value != 0.0f64 { 68.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn pow(lhs: f64, rhs: f64) -> f64 {
// CHECK-NEXT:     if lhs != 0.0f64 && rhs != 0.0f64 { 69.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn sqrt(value: f64) -> f64 {
// CHECK-NEXT:     if value != 0.0f64 { 70.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn exp(value: f64) -> f64 {
// CHECK-NEXT:     if value != 0.0f64 { 71.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn exp2(value: f64) -> f64 {
// CHECK-NEXT:     if value != 0.0f64 { 72.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn fmod(lhs: f64, rhs: f64) -> f64 {
// CHECK-NEXT:     if lhs != 0.0f64 && rhs != 0.0f64 { 73.0f64 } else { 0.0f64 }
// CHECK-NEXT: }
// CHECK-NEXT: fn lround(value: f64) -> i64 {
// CHECK-NEXT:     (if value != 0.0f64 { 74i32 } else { 0i32 }) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn llround(value: f64) -> i64 {
// CHECK-NEXT:     (if value != 0.0f64 { 75i32 } else { 0i32 }) as i64
// CHECK-NEXT: }
// CHECK-NEXT: fn pthread_create(
// CHECK-NEXT:     thread: *mut u64,
// CHECK-NEXT:     attr: *const (),
// CHECK-NEXT:     start: Option<unsafe extern "C-unwind" fn(*mut ()) -> *mut ()>,
// CHECK-NEXT:     arg: *mut (),
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     if !thread.is_null() && attr.is_null() && start != None && !arg.is_null() {
// CHECK-NEXT:         78
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn pthread_join(thread: u64, result: *mut *mut ()) -> i32 {
// CHECK-NEXT:     if thread != 0 && !result.is_null() { 79 } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.compare")]
// CHECK-NEXT: extern "C-unwind" fn compare(lhs: *const (), rhs: *const ()) -> i32 {
// CHECK-NEXT:     if lhs == rhs { 0 } else { 1 }
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn start(arg: *mut ()) -> *mut () {
// CHECK-NEXT:     arg
// CHECK-NEXT: }
// CHECK-NEXT: fn qsort(
// CHECK-NEXT:     base: *mut (),
// CHECK-NEXT:     count: u64,
// CHECK-NEXT:     size: u64,
// CHECK-NEXT:     callback: Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         project_state = if !base.is_null() && count != 0 && size != 0 && callback != None
// CHECK-NEXT:         {
// CHECK-NEXT:             76
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn bsearch(
// CHECK-NEXT:     key: *const (),
// CHECK-NEXT:     base: *const (),
// CHECK-NEXT:     count: u64,
// CHECK-NEXT:     size: u64,
// CHECK-NEXT:     callback: Option<unsafe extern "C-unwind" fn(*const (), *const ()) -> i32>,
// CHECK-NEXT: ) -> *mut () {
// CHECK-NEXT:     if !key.is_null() && !base.is_null() && count != 0 && size != 0 && callback != None {
// CHECK-NEXT:         56i32 as *mut ()
// CHECK-NEXT:     } else {
// CHECK-NEXT:         std::ptr::null_mut::<()>()
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn exit(status: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         project_state = status + 80;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: [i8; 8] = [97, 0, 0, 0, 0, 0, 0, 0];
// CHECK-NEXT:     let mut b: [i8; 8] = [98, 0, 0, 0, 0, 0, 0, 0];
// CHECK-NEXT:     let mut end: *mut i8 = a.as_mut_ptr() as *mut i8;
// CHECK-NEXT:     let count: u64 = 1;
// CHECK-NEXT:     let number: i32 = 1;
// CHECK-NEXT:     let real: f64 = 1.0f64;
// CHECK-NEXT:     let mut ptr: *mut () = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut thread: u64 = 1;
// CHECK-NEXT:     let strlen_call: Option<unsafe extern "C-unwind" fn(*const i8) -> u64> = Some(
// CHECK-NEXT:         strlen,
// CHECK-NEXT:     );
// CHECK-NEXT:     ptr = malloc(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) });
// CHECK-NEXT:     CHECK!(ptr, 16i32 as * mut (), 2i32);
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         calloc(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }), 16i32 as * mut (), 3i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         realloc(ptr, unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) }),
// CHECK-NEXT:         16i32 as * mut (), 4i32
// CHECK-NEXT:     );
// CHECK-NEXT:     free(ptr);
// CHECK-NEXT:     CHECK!(unsafe { project_state }, 77i32, 5i32);
// CHECK-NEXT:     CHECK__4!(
// CHECK-NEXT:         memcpy(a.as_mut_ptr() as * mut (), b.as_mut_ptr() as * const (), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }), a.as_mut_ptr() as * mut
// CHECK-NEXT:         i8, 6i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__4!(
// CHECK-NEXT:         memmove(a.as_mut_ptr() as * mut (), b.as_mut_ptr() as * const (), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }), a.as_mut_ptr() as * mut
// CHECK-NEXT:         i8, 7i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__4!(
// CHECK-NEXT:         memset(a.as_mut_ptr() as * mut (), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(number)) }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }), a.as_mut_ptr() as * mut
// CHECK-NEXT:         i8, 8i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         memchr(a.as_mut_ptr() as * const (), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(number)) }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }), 24i32 as * mut (), 9i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__2!(strlen(a.as_mut_ptr() as * const i8), 5i32, 10i32);
// CHECK-NEXT:     CHECK__2!(
// CHECK-NEXT:         unsafe { strlen_call.unwrap() (a.as_mut_ptr() as * const i8) }, 5i32, 10i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strcpy(a.as_mut_ptr() as * mut i8, b.as_mut_ptr() as * const i8), a.as_mut_ptr()
// CHECK-NEXT:         as * mut i8, 11i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strcat(a.as_mut_ptr() as * mut i8, b.as_mut_ptr() as * const i8), a.as_mut_ptr()
// CHECK-NEXT:         as * mut i8, 12i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strncpy(a.as_mut_ptr() as * mut i8, b.as_mut_ptr() as * const i8, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }), a.as_mut_ptr() as * mut
// CHECK-NEXT:         i8, 13i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strncat(a.as_mut_ptr() as * mut i8, b.as_mut_ptr() as * const i8, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }), a.as_mut_ptr() as * mut
// CHECK-NEXT:         i8, 14i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strcmp(a.as_mut_ptr() as * const i8, b.as_mut_ptr() as * const i8), 11i32, 15i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strncmp(a.as_mut_ptr() as * const i8, b.as_mut_ptr() as * const i8, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }), 12i32, 16i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         memcmp(a.as_mut_ptr() as * const (), b.as_mut_ptr() as * const (), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }), 13i32, 17i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strchr(a.as_mut_ptr() as * const i8, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(number)) }), 32i32 as * mut i8, 18i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strrchr(a.as_mut_ptr() as * const i8, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(number)) }), 32i32 as * mut i8, 19i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strstr(a.as_mut_ptr() as * const i8, b.as_mut_ptr() as * const i8), 32i32 as *
// CHECK-NEXT:         mut i8, 20i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strpbrk(a.as_mut_ptr() as * const i8, b.as_mut_ptr() as * const i8), 32i32 as *
// CHECK-NEXT:         mut i8, 21i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__2!(
// CHECK-NEXT:         strspn(a.as_mut_ptr() as * const i8, b.as_mut_ptr() as * const i8), 31i32, 22i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__2!(
// CHECK-NEXT:         strcspn(a.as_mut_ptr() as * const i8, b.as_mut_ptr() as * const i8), 32i32, 23i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(atoi(a.as_mut_ptr() as * const i8), 41i32, 24i32);
// CHECK-NEXT:     CHECK__3!(atol(a.as_mut_ptr() as * const i8), 42i32, 25i32);
// CHECK-NEXT:     CHECK__3!(
// CHECK-NEXT:         strtol(a.as_mut_ptr() as * const i8, std::ptr::addr_of_mut!(end), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(number)) }), 43i32, 26i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__2!(
// CHECK-NEXT:         strtoul(a.as_mut_ptr() as * const i8, std::ptr::addr_of_mut!(end), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(number)) }), 44i32, 27i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         strtod(a.as_mut_ptr() as * const i8, std::ptr::addr_of_mut!(end)), 45.0f64, 28i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(printf(a.as_mut_ptr() as * const i8), 51i32, 29i32);
// CHECK-NEXT:     CHECK!(puts(a.as_mut_ptr() as * const i8), 52i32, 30i32);
// CHECK-NEXT:     let stream: *mut () = fopen(
// CHECK-NEXT:         a.as_mut_ptr() as *const i8,
// CHECK-NEXT:         b.as_mut_ptr() as *const i8,
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(stream, 48i32 as * mut (), 31i32);
// CHECK-NEXT:     CHECK!(fputs(a.as_mut_ptr() as * const i8, stream), 54i32, 32i32);
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         fgets(a.as_mut_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(number)) }, stream), 40i32 as * mut
// CHECK-NEXT:         i8, 33i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__2!(
// CHECK-NEXT:         fread(a.as_mut_ptr() as * mut (), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }, stream), 56i32, 34i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__2!(
// CHECK-NEXT:         fwrite(a.as_mut_ptr() as * const (), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }, stream), 57i32, 35i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(fclose(stream), 58i32, 36i32);
// CHECK-NEXT:     CHECK!(fflush(stream), 59i32, 37i32);
// CHECK-NEXT:     CHECK!(remove(a.as_mut_ptr() as * const i8), 60i32, 38i32);
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         toupper(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) }), 61i32,
// CHECK-NEXT:         39i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         tolower(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(number)) }), 62i32,
// CHECK-NEXT:         40i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         sin(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 63.0f64, 41i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         cos(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 64.0f64, 42i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         tan(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 65.0f64, 43i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         log(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 66.0f64, 44i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         log10(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 67.0f64,
// CHECK-NEXT:         45i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         log2(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 68.0f64,
// CHECK-NEXT:         46i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         pow(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 69.0f64, 47i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         sqrt(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 70.0f64,
// CHECK-NEXT:         48i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         exp(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 71.0f64, 49i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         exp2(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 72.0f64,
// CHECK-NEXT:         50i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         fmod(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 73.0f64, 51i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__3!(
// CHECK-NEXT:         lround(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 74i32,
// CHECK-NEXT:         52i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK__3!(
// CHECK-NEXT:         llround(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) }), 75i32,
// CHECK-NEXT:         53i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         pthread_create(std::ptr::addr_of_mut!(thread), std::ptr::null::< () > (),
// CHECK-NEXT:         Some(start), ptr), 78i32, 54i32
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(pthread_join(thread, std::ptr::addr_of_mut!(ptr)), 79i32, 55i32);
// CHECK-NEXT:     qsort(
// CHECK-NEXT:         a.as_mut_ptr() as *mut (),
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) },
// CHECK-NEXT:         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(count)) },
// CHECK-NEXT:         Some(compare),
// CHECK-NEXT:     );
// CHECK-NEXT:     CHECK!(unsafe { project_state }, 76i32, 56i32);
// CHECK-NEXT:     CHECK!(
// CHECK-NEXT:         bsearch(a.as_mut_ptr() as * const (), b.as_mut_ptr() as * const (), unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }, unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(count)) }, Some(compare)), 56i32 as *
// CHECK-NEXT:         mut (), 57i32
// CHECK-NEXT:     );
// CHECK-NEXT:     exit(0i32);
// CHECK-NEXT:     if (unsafe { project_state }) == 80 { 1 } else { 58 }
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
