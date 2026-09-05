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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct GNUCookie {
// LOWERING-X86_64-GNU-NEXT:     bytes: [i8; 32],
// LOWERING-AARCH64-GNU-NEXT:     bytes: [u8; 32],
// LOWERING-NEXT:     length: u64,
// LOWERING-NEXT:     closed: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct _IO_cookie_io_functions_t {
// LOWERING-X86_64-GNU-NEXT:     read: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// LOWERING-X86_64-GNU-NEXT:     write: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// LOWERING-AARCH64-GNU-NEXT:     read: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// LOWERING-AARCH64-GNU-NEXT:     write: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// LOWERING-NEXT:     seek: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i64, i32) -> i32>,
// LOWERING-NEXT:     close: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn asprintf(_0: *mut *mut core::ffi::c_char, _1: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn open_memstream(_0: *mut *mut core::ffi::c_char, _1: *mut usize) -> *mut libc::FILE;
// LOWERING-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn __fwriting(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn __fpending(_0: *mut libc::FILE) -> usize;
// LOWERING-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn fmemopen(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: usize,
// LOWERING-NEXT:         _2: *const core::ffi::c_char,
// LOWERING-NEXT:     ) -> *mut libc::FILE;
// LOWERING-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT:     fn getdelim(
// LOWERING-NEXT:         _0: *mut *mut core::ffi::c_char,
// LOWERING-NEXT:         _1: *mut usize,
// LOWERING-NEXT:         _2: i32,
// LOWERING-NEXT:         _3: *mut libc::FILE,
// LOWERING-NEXT:     ) -> isize;
// LOWERING-NEXT:     fn getline(_0: *mut *mut core::ffi::c_char, _1: *mut usize, _2: *mut libc::FILE) -> isize;
// LOWERING-NEXT:     fn __freading(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn __fbufsize(_0: *mut libc::FILE) -> usize;
// LOWERING-NEXT:     fn fopencookie(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: *const core::ffi::c_char,
// LOWERING-NEXT:         _2: _IO_cookie_io_functions_t,
// LOWERING-NEXT:     ) -> *mut libc::FILE;
// LOWERING-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// LOWERING-NEXT:     fn memcpy(
// LOWERING-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-NEXT:         _1: *const core::ffi::c_void,
// LOWERING-NEXT:         _2: usize,
// LOWERING-NEXT:     ) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn parse_printf_format(_0: *const core::ffi::c_char, _1: usize, _2: *mut i32) -> usize;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_allocating_stdio();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_memory_stdio();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_cookie_stdio();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = gnu_printf_introspection();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_allocating_stdio() -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut formatted: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut stream_data: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut formatted: *mut u8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut stream_data: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut stream_size: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     formatted = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     stream_data = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     stream_size = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s:%d\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"gnu\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%s:%d\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"gnu\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 23;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         asprintf(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(formatted) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = formatted;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"gnu:23\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = formatted;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"gnu:23\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = formatted;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = formatted;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// LOWERING-NEXT:         open_memstream(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(stream_data) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(stream_size) as *mut usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s-%d\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%s-%d\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 24;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         fprintf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __fwriting({{__v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { __fpending({{__v[0-9]+}} as *mut libc::FILE) }) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fflush({{__v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = stream_size;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = stream_data;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"slate-24\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = stream_data;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"slate-24\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = stream_data;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = stream_data;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_memory_stdio() -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut source: [i8; 12] = [0; 12];
// LOWERING-X86_64-GNU-NEXT:     let mut line: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let mut source: [u8; 12] = [0; 12];
// LOWERING-AARCH64-GNU-NEXT:     let mut line: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut capacity: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 12] = [97, 108, 112, 104, 97, 124, 98, 101, 116, 97, 10, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 12] = [97, 108, 112, 104, 97, 124, 98, 101, 116, 97, 10, 0];
// LOWERING-NEXT:     source = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     line = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     capacity = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"r\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"r\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// LOWERING-NEXT:         fmemopen(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 124;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = (unsafe {
// LOWERING-NEXT:         getdelim(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(line) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(capacity) as *mut usize,
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 6;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = line;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"alpha|\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = line;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"alpha|\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = (unsafe {
// LOWERING-NEXT:         getline(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(line) as *mut *mut core::ffi::c_char,
// LOWERING-NEXT:             std::ptr::addr_of_mut!(capacity) as *mut usize,
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = line;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"beta\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = line;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"beta\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         strcmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __freading({{__v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { __fbufsize({{__v[0-9]+}} as *mut libc::FILE) }) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = line;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = line;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_cookie_stdio() -> i32 {
// LOWERING-NEXT:     let mut cookie: GNUCookie = GNUCookie {
// LOWERING-NEXT:         bytes: [0; 32],
// LOWERING-NEXT:         length: 0,
// LOWERING-NEXT:         closed: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut functions: _IO_cookie_io_functions_t = _IO_cookie_io_functions_t {
// LOWERING-NEXT:         read: None,
// LOWERING-NEXT:         write: None,
// LOWERING-NEXT:         seek: None,
// LOWERING-NEXT:         close: None,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: GNUCookie = GNUCookie {
// LOWERING-NEXT:         bytes: [0; 32],
// LOWERING-NEXT:         length: 0,
// LOWERING-NEXT:         closed: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     cookie = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: _IO_cookie_io_functions_t = _IO_cookie_io_functions_t {
// LOWERING-NEXT:         read: None,
// LOWERING-NEXT:         write: None,
// LOWERING-NEXT:         seek: None,
// LOWERING-NEXT:         close: None,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     functions = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     functions.write = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<
// LOWERING-NEXT:             *const (),
// LOWERING-X86_64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// LOWERING-AARCH64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// LOWERING-NEXT:         >(gnu_cookie_write as *const ())
// LOWERING-NEXT:     };
// LOWERING-NEXT:     functions.close = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<
// LOWERING-NEXT:             *const (),
// LOWERING-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// LOWERING-NEXT:         >(gnu_cookie_close as *const ())
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(cookie) as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"w\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: _IO_cookie_io_functions_t = functions;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// LOWERING-NEXT:         fopencookie(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as _IO_cookie_io_functions_t,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s:%d\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cookie\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%s:%d\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cookie\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         fprintf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = cookie.closed;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = cookie.length;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(cookie.bytes) as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(cookie.bytes) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cookie:7\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"cookie:7\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         memcmp(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_printf_introspection() -> i32 {
// LOWERING-X86_64-GNU-NEXT:     let mut types: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-AARCH64-GNU-NEXT:     let mut types: [i32; 4] = [0; 4];
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [0; 4];
// LOWERING-X86_64-GNU-NEXT:     *types = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%2$d %1$s\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     types = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%2$d %1$s\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = types.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// LOWERING-NEXT:         parse_printf_format(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as usize,
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut i32,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }) as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = types[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -65281;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} & {{__v[0-9]+}};
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = types[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -65281;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} & {{__v[0-9]+}};
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = false;
// LOWERING-NEXT:         {{__v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn gnu_cookie_write(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: *mut u8,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT: ) -> i64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).length };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-NEXT:         memcpy(
// LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).length };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).length = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{arg[0-9]+}} as i64;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn gnu_cookie_close({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).closed = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct GNUCookie {
// REWRITES-X86_64-GNU-NEXT:     bytes: [i8; 32],
// REWRITES-AARCH64-GNU-NEXT:     bytes: [u8; 32],
// REWRITES-NEXT:     length: u64,
// REWRITES-NEXT:     closed: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct _IO_cookie_io_functions_t {
// REWRITES-X86_64-GNU-NEXT:     read: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// REWRITES-X86_64-GNU-NEXT:     write: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// REWRITES-AARCH64-GNU-NEXT:     read: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// REWRITES-AARCH64-GNU-NEXT:     write: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// REWRITES-NEXT:     seek: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i64, i32) -> i32>,
// REWRITES-NEXT:     close: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn asprintf(_0: *mut *mut core::ffi::c_char, _1: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn open_memstream(_0: *mut *mut core::ffi::c_char, _1: *mut usize) -> *mut libc::FILE;
// REWRITES-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn __fwriting(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn __fpending(_0: *mut libc::FILE) -> usize;
// REWRITES-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn fmemopen(
// REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-NEXT:         _1: usize,
// REWRITES-NEXT:         _2: *const core::ffi::c_char,
// REWRITES-NEXT:     ) -> *mut libc::FILE;
// REWRITES-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT:     fn getdelim(
// REWRITES-NEXT:         _0: *mut *mut core::ffi::c_char,
// REWRITES-NEXT:         _1: *mut usize,
// REWRITES-NEXT:         _2: i32,
// REWRITES-NEXT:         _3: *mut libc::FILE,
// REWRITES-NEXT:     ) -> isize;
// REWRITES-NEXT:     fn getline(_0: *mut *mut core::ffi::c_char, _1: *mut usize, _2: *mut libc::FILE) -> isize;
// REWRITES-NEXT:     fn __freading(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn __fbufsize(_0: *mut libc::FILE) -> usize;
// REWRITES-NEXT:     fn fopencookie(
// REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-NEXT:         _1: *const core::ffi::c_char,
// REWRITES-NEXT:         _2: _IO_cookie_io_functions_t,
// REWRITES-NEXT:     ) -> *mut libc::FILE;
// REWRITES-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// REWRITES-NEXT:     fn memcpy(
// REWRITES-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-NEXT:         _1: *const core::ffi::c_void,
// REWRITES-NEXT:         _2: usize,
// REWRITES-NEXT:     ) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn parse_printf_format(_0: *const core::ffi::c_char, _1: usize, _2: *mut i32) -> usize;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             gnu_allocating_stdio(),
// REWRITES-NEXT:             gnu_memory_stdio(),
// REWRITES-NEXT:             gnu_cookie_stdio(),
// REWRITES-NEXT:             gnu_printf_introspection(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_allocating_stdio() -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut formatted: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut stream_data: *mut i8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let mut formatted: *mut u8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let mut stream_data: *mut u8 = std::ptr::null_mut();
// REWRITES-NEXT:     let mut stream_size: u64 = 0;
// REWRITES-NEXT:     stream_data = std::ptr::null_mut();
// REWRITES-NEXT:     stream_size = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         asprintf(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(formatted) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             c"%s:%d".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             c"gnu".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:             c"gnu".as_ptr() as *mut u8,
// REWRITES-NEXT:             23 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 6) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp(formatted as *const core::ffi::c_char, c"gnu:23".as_ptr()) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// REWRITES-NEXT:     unsafe { free(formatted as *mut core::ffi::c_void) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// REWRITES-NEXT:         open_memstream(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(stream_data) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(stream_size) as *mut usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         fprintf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// REWRITES-NEXT:             c"%s-%d".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             c"slate".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:             c"slate".as_ptr() as *mut u8,
// REWRITES-NEXT:             24 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 8) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __fwriting({{__v[0-9]+}} as *mut libc::FILE) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { __fpending({{__v[0-9]+}} as *mut libc::FILE) }) as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fflush({{__v[0-9]+}} as *mut libc::FILE) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32) + ((stream_size == 8) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         strcmp(
// REWRITES-NEXT:             stream_data as *const core::ffi::c_char,
// REWRITES-NEXT:             c"slate-24".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// REWRITES-NEXT:     unsafe { free(stream_data as *mut core::ffi::c_void) };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_memory_stdio() -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut source: [i8; 12] = [0; 12];
// REWRITES-X86_64-GNU-NEXT:     let mut line: *mut i8 = std::ptr::null_mut();
// REWRITES-AARCH64-GNU-NEXT:     let mut source: [u8; 12] = [0; 12];
// REWRITES-AARCH64-GNU-NEXT:     let mut line: *mut u8 = std::ptr::null_mut();
// REWRITES-NEXT:     let mut capacity: u64 = 0;
// REWRITES-NEXT:     source = [97, 108, 112, 104, 97, 124, 98, 101, 116, 97, 10, 0];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = source.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = source.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { strlen({{__v[0-9]+}} as *const core::ffi::c_char) }) as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE =
// REWRITES-NEXT:         unsafe { fmemopen({{__v[0-9]+}} as *mut core::ffi::c_void, {{__v[0-9]+}} as usize, c"r".as_ptr()) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0 + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = (unsafe {
// REWRITES-NEXT:         getdelim(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(line) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(capacity) as *mut usize,
// REWRITES-NEXT:             124 as i32,
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as i64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 6) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp(line as *const core::ffi::c_char, c"alpha|".as_ptr()) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = (unsafe {
// REWRITES-NEXT:         getline(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(line) as *mut *mut core::ffi::c_char,
// REWRITES-NEXT:             std::ptr::addr_of_mut!(capacity) as *mut usize,
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as i64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 5) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { strcmp(line as *const core::ffi::c_char, c"beta\n".as_ptr()) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __freading({{__v[0-9]+}} as *mut libc::FILE) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe { __fbufsize({{__v[0-9]+}} as *mut libc::FILE) }) as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} > 0) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32);
// REWRITES-NEXT:     unsafe { free(line as *mut core::ffi::c_void) };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_cookie_stdio() -> i32 {
// REWRITES-NEXT:     let mut cookie: GNUCookie = GNUCookie {
// REWRITES-NEXT:         bytes: [0; 32],
// REWRITES-NEXT:         length: 0,
// REWRITES-NEXT:         closed: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut functions: _IO_cookie_io_functions_t = _IO_cookie_io_functions_t {
// REWRITES-NEXT:         read: None,
// REWRITES-NEXT:         write: None,
// REWRITES-NEXT:         seek: None,
// REWRITES-NEXT:         close: None,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     cookie = GNUCookie {
// REWRITES-NEXT:         bytes: [0; 32],
// REWRITES-NEXT:         length: 0,
// REWRITES-NEXT:         closed: 0,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     functions = _IO_cookie_io_functions_t {
// REWRITES-NEXT:         read: None,
// REWRITES-NEXT:         write: None,
// REWRITES-NEXT:         seek: None,
// REWRITES-NEXT:         close: None,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     functions.write = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<
// REWRITES-NEXT:             *const (),
// REWRITES-X86_64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// REWRITES-AARCH64-GNU-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// REWRITES-NEXT:         >(gnu_cookie_write as *const ())
// REWRITES-NEXT:     };
// REWRITES-NEXT:     functions.close = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<
// REWRITES-NEXT:             *const (),
// REWRITES-NEXT:             Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// REWRITES-NEXT:         >(gnu_cookie_close as *const ())
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(cookie) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// REWRITES-NEXT:         fopencookie(
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:             c"w".as_ptr(),
// REWRITES-NEXT:             functions as _IO_cookie_io_functions_t,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} != std::ptr::null_mut()) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         fprintf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut libc::FILE,
// REWRITES-NEXT:             c"%s:%d".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             c"cookie".as_ptr() as *mut i8,
// REWRITES-AARCH64-GNU-NEXT:             c"cookie".as_ptr() as *mut u8,
// REWRITES-NEXT:             7 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + (({{__v[0-9]+}} == 8) as i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { fclose({{__v[0-9]+}} as *mut libc::FILE) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}
// REWRITES-NEXT:         + (({{__v[0-9]+}} == 0) as i32)
// REWRITES-NEXT:         + ((cookie.closed == 1) as i32)
// REWRITES-NEXT:         + ((cookie.length == 8) as i32);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(cookie.bytes) as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(cookie.bytes) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         memcmp(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-NEXT:             b"cookie:7\0".as_ptr() as *const core::ffi::c_void,
// REWRITES-NEXT:             (8 as u64) as usize,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{__v[0-9]+}} + (({{__v[0-9]+}} == 0) as i32)
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_printf_introspection() -> i32 {
// REWRITES-X86_64-GNU-NEXT:     let mut types: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     *types = [0; 4];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%2$d %1$s".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let mut types: [i32; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%2$d %1$s".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = types.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = (unsafe {
// REWRITES-NEXT:         parse_printf_format(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             (4 as u64) as usize,
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }) as u64;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} == 2 {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = types[0] & -65281 == 3;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = types[1] & -65281 == 0;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = false;
// REWRITES-NEXT:         {{__v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{__v[0-9]+}} as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn gnu_cookie_write(
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     {{arg[0-9]+}}: *mut u8,
// REWRITES-NEXT:     {{arg[0-9]+}}: u64,
// REWRITES-NEXT: ) -> i64 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).length };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// REWRITES-NEXT:     unsafe { std::ptr::copy_nonoverlapping({{arg[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, {{arg[0-9]+}} as usize) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{__v[0-9]+}}).length = (unsafe { (*{{__v[0-9]+}}).length }) + {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     {{arg[0-9]+}} as i64
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn gnu_cookie_close({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*({{arg[0-9]+}} as *mut GNUCookie)).closed = 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     0
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
