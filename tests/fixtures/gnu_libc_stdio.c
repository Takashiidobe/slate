#define _GNU_SOURCE
#include <printf.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

struct GNUCookie {
  char   bytes[32];
  size_t length;
  int    closed;
};

static ssize_t gnu_cookie_write(void *state, const char *buffer, size_t size) {
  struct GNUCookie *cookie = state;
  memcpy(cookie->bytes + cookie->length, buffer, size);
  cookie->length += size;
  return (ssize_t)size;
}

static int gnu_cookie_close(void *state) {
  struct GNUCookie *cookie = state;
  cookie->closed           = 1;
  return 0;
}

static int gnu_allocating_stdio(void) {
  char  *formatted   = NULL;
  char  *stream_data = NULL;
  size_t stream_size = 0;
  FILE  *stream;
  int    total = 0;

  total += asprintf(&formatted, "%s:%d", "gnu", 23) == 6;
  total += strcmp(formatted, "gnu:23") == 0;
  free(formatted);

  stream  = open_memstream(&stream_data, &stream_size);
  total  += stream != NULL;
  total  += fprintf(stream, "%s-%d", "slate", 24) == 8;
  total  += __fwriting(stream) != 0;
  total  += __fpending(stream) > 0;
  total  += fflush(stream) == 0;
  total  += stream_size == 8;
  total  += strcmp(stream_data, "slate-24") == 0;
  total  += fclose(stream) == 0;
  free(stream_data);
  return total;
}

static int gnu_memory_stdio(void) {
  char   source[] = "alpha|beta\n";
  char  *line     = NULL;
  size_t capacity = 0;
  FILE  *stream   = fmemopen(source, strlen(source), "r");
  int    total    = 0;

  total += stream != NULL;
  total += getdelim(&line, &capacity, '|', stream) == 6;
  total += strcmp(line, "alpha|") == 0;
  total += getline(&line, &capacity, stream) == 5;
  total += strcmp(line, "beta\n") == 0;
  total += __freading(stream) != 0;
  total += __fbufsize(stream) > 0;
  total += fclose(stream) == 0;
  free(line);
  return total;
}

static int gnu_cookie_stdio(void) {
  struct GNUCookie      cookie    = {};
  cookie_io_functions_t functions = {};
  FILE                 *stream;
  int                   total = 0;

  functions.write  = gnu_cookie_write;
  functions.close  = gnu_cookie_close;
  stream           = fopencookie(&cookie, "w", functions);
  total           += stream != NULL;
  total           += fprintf(stream, "%s:%d", "cookie", 7) == 8;
  total           += fclose(stream) == 0;
  total           += cookie.closed == 1;
  total           += cookie.length == 8;
  total           += memcmp(cookie.bytes, "cookie:7", 8) == 0;
  return total;
}

static int gnu_printf_introspection(void) {
  int    types[4]  = {};
  size_t arguments = parse_printf_format("%2$d %1$s", 4, types);
  return arguments == 2 && (types[0] & ~PA_FLAG_MASK) == PA_STRING &&
         (types[1] & ~PA_FLAG_MASK) == PA_INT;
}

int main(void) {
  printf("%d %d %d %d\n", gnu_allocating_stdio(), gnu_memory_stdio(),
         gnu_cookie_stdio(), gnu_printf_introspection());
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
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
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct GNUCookie {
// COMMON-LOWERING-NEXT:     length: u64,
// COMMON-LOWERING-NEXT:     closed: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct _IO_cookie_io_functions_t {
// COMMON-LOWERING-NEXT:     seek: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i64, i32) -> i32>,
// COMMON-LOWERING-NEXT:     close: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn asprintf(_0: *mut *mut core::ffi::c_char, _1: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     fn open_memstream(_0: *mut *mut core::ffi::c_char, _1: *mut usize) -> *mut libc::FILE;
// COMMON-LOWERING-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn __fwriting(_0: *mut libc::FILE) -> i32;
// COMMON-LOWERING-NEXT:     fn __fpending(_0: *mut libc::FILE) -> usize;
// COMMON-LOWERING-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// COMMON-LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// COMMON-LOWERING-NEXT:     fn fmemopen(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: usize,
// COMMON-LOWERING-NEXT:         _2: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:     ) -> *mut libc::FILE;
// COMMON-LOWERING-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// COMMON-LOWERING-NEXT:     fn getdelim(
// COMMON-LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _1: *mut usize,
// COMMON-LOWERING-NEXT:         _2: i32,
// COMMON-LOWERING-NEXT:         _3: *mut libc::FILE,
// COMMON-LOWERING-NEXT:     ) -> isize;
// COMMON-LOWERING-NEXT:     fn getline(_0: *mut *mut core::ffi::c_char, _1: *mut usize, _2: *mut libc::FILE) -> isize;
// COMMON-LOWERING-NEXT:     fn __freading(_0: *mut libc::FILE) -> i32;
// COMMON-LOWERING-NEXT:     fn __fbufsize(_0: *mut libc::FILE) -> usize;
// COMMON-LOWERING-NEXT:     fn fopencookie(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         _2: _IO_cookie_io_functions_t,
// COMMON-LOWERING-NEXT:     ) -> *mut libc::FILE;
// COMMON-LOWERING-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// COMMON-LOWERING-NEXT:     fn memcpy(
// COMMON-LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _1: *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:         _2: usize,
// COMMON-LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn parse_printf_format(_0: *const core::ffi::c_char, _1: usize, _2: *mut i32) -> usize;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_allocating_stdio();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_memory_stdio();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_cookie_stdio();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_printf_introspection();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_allocating_stdio() -> i32 {
// COMMON-LOWERING-NEXT:     let mut stream_size: u64 = 0;
// COMMON-LOWERING-NEXT:     formatted = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     stream_data = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     stream_size = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 23;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         asprintf(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(formatted) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// COMMON-LOWERING-NEXT:         open_memstream(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(stream_data) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(stream_size) as *mut usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 24;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         fprintf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __fwriting({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { __fpending({{__v[0-9]+}} as *mut libc::FILE) }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fflush({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = stream_size;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_memory_stdio() -> i32 {
// COMMON-LOWERING-NEXT:     let mut capacity: u64 = 0;
// COMMON-LOWERING-NEXT:     source = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     line = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     capacity = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// COMMON-LOWERING-NEXT:         fmemopen(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 124;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = (unsafe {
// COMMON-LOWERING-NEXT:         getdelim(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(line) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(capacity) as *mut usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = (unsafe {
// COMMON-LOWERING-NEXT:         getline(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(line) as *mut *mut core::ffi::c_char,
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(capacity) as *mut usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as i64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         strcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __freading({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { __fbufsize({{__v[0-9]+}} as *mut libc::FILE) }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_cookie_stdio() -> i32 {
// COMMON-LOWERING-NEXT:     let mut cookie: GNUCookie = GNUCookie {
// COMMON-LOWERING-NEXT:         bytes: [0; 32],
// COMMON-LOWERING-NEXT:         length: 0,
// COMMON-LOWERING-NEXT:         closed: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut functions: _IO_cookie_io_functions_t = _IO_cookie_io_functions_t {
// COMMON-LOWERING-NEXT:         read: None,
// COMMON-LOWERING-NEXT:         write: None,
// COMMON-LOWERING-NEXT:         seek: None,
// COMMON-LOWERING-NEXT:         close: None,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: GNUCookie = GNUCookie {
// COMMON-LOWERING-NEXT:         bytes: [0; 32],
// COMMON-LOWERING-NEXT:         length: 0,
// COMMON-LOWERING-NEXT:         closed: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     cookie = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: _IO_cookie_io_functions_t = _IO_cookie_io_functions_t {
// COMMON-LOWERING-NEXT:         read: None,
// COMMON-LOWERING-NEXT:         write: None,
// COMMON-LOWERING-NEXT:         seek: None,
// COMMON-LOWERING-NEXT:         close: None,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     functions = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     functions.write = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<
// COMMON-LOWERING-NEXT:             *const (),
// COMMON-LOWERING-NEXT:         >(gnu_cookie_write as *const ())
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     functions.close = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<
// COMMON-LOWERING-NEXT:             *const (),
// COMMON-LOWERING-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// COMMON-LOWERING-NEXT:         >(gnu_cookie_close as *const ())
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(cookie) as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: _IO_cookie_io_functions_t = functions;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// COMMON-LOWERING-NEXT:         fopencookie(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as _IO_cookie_io_functions_t,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         fprintf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = cookie.closed;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = cookie.length;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         memcmp(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn gnu_printf_introspection() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [0; 4];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = types.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-LOWERING-NEXT:         parse_printf_format(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut i32,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     }) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = types[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -65281;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} & {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = types[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -65281;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} & {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn gnu_cookie_write(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT: ) -> i64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).length };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-NEXT:         memcpy(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-NEXT:             {{arg[0-9]+}} as usize,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).length };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         (*{{__v[0-9]+}}).length = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{arg[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn gnu_cookie_close({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         (*{{__v[0-9]+}}).closed = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     bytes: [i8; 32],
// LOWERING-X86_64-GNU-NEXT:     read: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// LOWERING-X86_64-GNU-NEXT:     write: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut formatted: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut stream_data: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s:%d\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = formatted;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"gnu:23\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = formatted;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s-%d\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = stream_data;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate-24\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = stream_data;
// LOWERING-X86_64-GNU-NEXT:     let mut source: [i8; 12] = [0; 12];
// LOWERING-X86_64-GNU-NEXT:     let mut line: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 12] = [97, 108, 112, 104, 97, 124, 98, 101, 116, 97, 10, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"r\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = line;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"alpha|\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = line;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"beta\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = line;
// LOWERING-X86_64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s:%d\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cookie\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(cookie.bytes) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cookie:7\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let mut types: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     *types = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%2$d %1$s\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     bytes: [u8; 32],
// LOWERING-AARCH64-GNU-NEXT:     read: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// LOWERING-AARCH64-GNU-NEXT:     write: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut formatted: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut stream_data: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%s:%d\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"gnu\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = formatted;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"gnu:23\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = formatted;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%s-%d\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = stream_data;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate-24\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = stream_data;
// LOWERING-AARCH64-GNU-NEXT:     let mut source: [u8; 12] = [0; 12];
// LOWERING-AARCH64-GNU-NEXT:     let mut line: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 12] = [97, 108, 112, 104, 97, 124, 98, 101, 116, 97, 10, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"r\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = line;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"alpha|\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = line;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"beta\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = line;
// LOWERING-AARCH64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"w\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%s:%d\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cookie\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(cookie.bytes) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cookie:7\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let mut types: [i32; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     types = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%2$d %1$s\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
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
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct GNUCookie {
// COMMON-REWRITES-NEXT:     length: u64,
// COMMON-REWRITES-NEXT:     closed: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct _IO_cookie_io_functions_t {
// COMMON-REWRITES-NEXT:     seek: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i64, i32) -> i32>,
// COMMON-REWRITES-NEXT:     close: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn asprintf(_0: *mut *mut core::ffi::c_char, _1: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     fn open_memstream(_0: *mut *mut core::ffi::c_char, _1: *mut usize) -> *mut libc::FILE;
// COMMON-REWRITES-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn __fwriting(_0: *mut libc::FILE) -> i32;
// COMMON-REWRITES-NEXT:     fn __fpending(_0: *mut libc::FILE) -> usize;
// COMMON-REWRITES-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// COMMON-REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// COMMON-REWRITES-NEXT:     fn fmemopen(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: usize,
// COMMON-REWRITES-NEXT:         _2: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:     ) -> *mut libc::FILE;
// COMMON-REWRITES-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// COMMON-REWRITES-NEXT:     fn getdelim(
// COMMON-REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _1: *mut usize,
// COMMON-REWRITES-NEXT:         _2: i32,
// COMMON-REWRITES-NEXT:         _3: *mut libc::FILE,
// COMMON-REWRITES-NEXT:     ) -> isize;
// COMMON-REWRITES-NEXT:     fn getline(_0: *mut *mut core::ffi::c_char, _1: *mut usize, _2: *mut libc::FILE) -> isize;
// COMMON-REWRITES-NEXT:     fn __freading(_0: *mut libc::FILE) -> i32;
// COMMON-REWRITES-NEXT:     fn __fbufsize(_0: *mut libc::FILE) -> usize;
// COMMON-REWRITES-NEXT:     fn fopencookie(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:         _2: _IO_cookie_io_functions_t,
// COMMON-REWRITES-NEXT:     ) -> *mut libc::FILE;
// COMMON-REWRITES-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// COMMON-REWRITES-NEXT:     fn memcpy(
// COMMON-REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _1: *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:         _2: usize,
// COMMON-REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn parse_printf_format(_0: *const core::ffi::c_char, _1: usize, _2: *mut i32) -> usize;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             gnu_allocating_stdio(),
// COMMON-REWRITES-NEXT:             gnu_memory_stdio(),
// COMMON-REWRITES-NEXT:             gnu_cookie_stdio(),
// COMMON-REWRITES-NEXT:             gnu_printf_introspection(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_allocating_stdio() -> i32 {
// COMMON-REWRITES-NEXT:     let mut stream_size: u64 = 0;
// COMMON-REWRITES-NEXT:     stream_data = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:     stream_size = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         asprintf(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(formatted) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             c"%s:%d".as_ptr(),
// COMMON-REWRITES-NEXT:             23 as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 6) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp(formatted as *const core::ffi::c_char, c"gnu:23".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     unsafe { free(formatted as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// COMMON-REWRITES-NEXT:         open_memstream(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(stream_data) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(stream_size) as *mut usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         fprintf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-REWRITES-NEXT:             c"%s-%d".as_ptr(),
// COMMON-REWRITES-NEXT:             24 as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 8) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __fwriting({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { __fpending({{__v[0-9]+}} as *mut libc::FILE) }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fflush({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32) + ((stream_size == 8) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         strcmp(
// COMMON-REWRITES-NEXT:             stream_data as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             c"slate-24".as_ptr(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     unsafe { free(stream_data as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_memory_stdio() -> i32 {
// COMMON-REWRITES-NEXT:     let mut capacity: u64 = 0;
// COMMON-REWRITES-NEXT:     source = [97, 108, 112, 104, 97, 124, 98, 101, 116, 97, 10, 0];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE =
// COMMON-REWRITES-NEXT:         unsafe { fmemopen({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as usize, c"r".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0 + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = (unsafe {
// COMMON-REWRITES-NEXT:         getdelim(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(line) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(capacity) as *mut usize,
// COMMON-REWRITES-NEXT:             124 as i32,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as i64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 6) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp(line as *const core::ffi::c_char, c"alpha|".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = (unsafe {
// COMMON-REWRITES-NEXT:         getline(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(line) as *mut *mut core::ffi::c_char,
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(capacity) as *mut usize,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as i64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 5) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp(line as *const core::ffi::c_char, c"beta\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __freading({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { __fbufsize({{__v[0-9]+}} as *mut libc::FILE) }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// COMMON-REWRITES-NEXT:     unsafe { free(line as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_cookie_stdio() -> i32 {
// COMMON-REWRITES-NEXT:     let mut cookie: GNUCookie = GNUCookie {
// COMMON-REWRITES-NEXT:         bytes: [0; 32],
// COMMON-REWRITES-NEXT:         length: 0,
// COMMON-REWRITES-NEXT:         closed: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut functions: _IO_cookie_io_functions_t = _IO_cookie_io_functions_t {
// COMMON-REWRITES-NEXT:         read: None,
// COMMON-REWRITES-NEXT:         write: None,
// COMMON-REWRITES-NEXT:         seek: None,
// COMMON-REWRITES-NEXT:         close: None,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     cookie = GNUCookie {
// COMMON-REWRITES-NEXT:         bytes: [0; 32],
// COMMON-REWRITES-NEXT:         length: 0,
// COMMON-REWRITES-NEXT:         closed: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     functions = _IO_cookie_io_functions_t {
// COMMON-REWRITES-NEXT:         read: None,
// COMMON-REWRITES-NEXT:         write: None,
// COMMON-REWRITES-NEXT:         seek: None,
// COMMON-REWRITES-NEXT:         close: None,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     functions.write = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<
// COMMON-REWRITES-NEXT:             *const (),
// COMMON-REWRITES-NEXT:         >(gnu_cookie_write as *const ())
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     functions.close = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<
// COMMON-REWRITES-NEXT:             *const (),
// COMMON-REWRITES-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// COMMON-REWRITES-NEXT:         >(gnu_cookie_close as *const ())
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(cookie) as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// COMMON-REWRITES-NEXT:         fopencookie(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             c"w".as_ptr(),
// COMMON-REWRITES-NEXT:             functions as _IO_cookie_io_functions_t,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         fprintf(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// COMMON-REWRITES-NEXT:             c"%s:%d".as_ptr(),
// COMMON-REWRITES-NEXT:             7 as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 8) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:         + (({{__v[0-9]+}} == 0) as i32)
// COMMON-REWRITES-NEXT:         + ((cookie.closed == 1) as i32)
// COMMON-REWRITES-NEXT:         + ((cookie.length == 8) as i32);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         memcmp(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             b"cookie:7\0".as_ptr() as *const core::ffi::c_void,
// COMMON-REWRITES-NEXT:             (8 as u64) as usize,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32)
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn gnu_printf_introspection() -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = types.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// COMMON-REWRITES-NEXT:         parse_printf_format(
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-REWRITES-NEXT:             (4 as u64) as usize,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     }) as u64;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} == 2 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = types[0] & -65281 == 3;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = types[1] & -65281 == 0;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}} as i32
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn gnu_cookie_write(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-REWRITES-NEXT: ) -> i64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).length };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::copy_nonoverlapping({{arg[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, {{arg[0-9]+}} as usize) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         (*{{__v[0-9]+}}).length = (unsafe { (*{{__v[0-9]+}}).length }) + {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} as i64
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn gnu_cookie_close({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         (*({{arg[0-9]+}} as *mut GNUCookie)).closed = 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     0
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     bytes: [i8; 32],
// REWRITES-X86_64-GNU-NEXT:     read: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// REWRITES-X86_64-GNU-NEXT:     write: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// REWRITES-X86_64-GNU-NEXT:     let mut formatted: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut stream_data: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:             c"gnu".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             c"slate".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:     let mut source: [i8; 12] = [0; 12];
// REWRITES-X86_64-GNU-NEXT:     let mut line: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// REWRITES-X86_64-GNU-NEXT:             c"cookie".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(cookie.bytes) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let mut types: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     *types = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%2$d %1$s".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     bytes: [u8; 32],
// REWRITES-AARCH64-GNU-NEXT:     read: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// REWRITES-AARCH64-GNU-NEXT:     write: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// REWRITES-AARCH64-GNU-NEXT:     let mut formatted: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let mut stream_data: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:             c"gnu".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:             c"slate".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     let mut source: [u8; 12] = [0; 12];
// REWRITES-AARCH64-GNU-NEXT:     let mut line: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// REWRITES-AARCH64-GNU-NEXT:             c"cookie".as_ptr() as *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(cookie.bytes) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let mut types: [i32; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%2$d %1$s".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
