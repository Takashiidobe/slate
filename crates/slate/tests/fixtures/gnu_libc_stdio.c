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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn parse_printf_format([[ARG_0:arg[0-9]+]]: *const i8, [[ARG_1:arg[0-9]+]]: u64, [[ARG_2:arg[0-9]+]]: *mut i32) -> u64;
// CHECK-NEXT:     fn fflush([[ARG_0]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT:     fn fopencookie(
// CHECK-NEXT:         [[ARG_0]]: *mut (),
// CHECK-NEXT:         [[ARG_1]]: *const i8,
// CHECK-NEXT:         [[ARG_2]]: _IO_cookie_io_functions_t,
// CHECK-NEXT:     ) -> *mut slate_support::FILE;
// CHECK-NEXT:     fn fmemopen([[ARG_0]]: *mut (), [[ARG_1]]: u64, [[ARG_2]]: *const i8) -> *mut slate_support::FILE;
// CHECK-NEXT:     fn open_memstream([[ARG_0]]: *mut *mut i8, [[ARG_1]]: *mut u64) -> *mut slate_support::FILE;
// CHECK-NEXT:     fn asprintf([[ARG_0]]: *mut *mut i8, [[ARG_1]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn getdelim(
// CHECK-NEXT:         [[ARG_0]]: *mut *mut i8,
// CHECK-NEXT:         [[ARG_1]]: *mut u64,
// CHECK-NEXT:         [[ARG_2]]: i32,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: *mut slate_support::FILE,
// CHECK-NEXT:     ) -> i64;
// CHECK-NEXT:     fn getline(
// CHECK-NEXT:         [[ARG_0]]: *mut *mut i8,
// CHECK-NEXT:         [[ARG_1]]: *mut u64,
// CHECK-NEXT:         [[ARG_2]]: *mut slate_support::FILE,
// CHECK-NEXT:     ) -> i64;
// CHECK-NEXT:     fn __fbufsize([[ARG_0]]: *mut slate_support::FILE) -> u64;
// CHECK-NEXT:     fn __freading([[ARG_0]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT:     fn __fwriting([[ARG_0]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT:     fn __fpending([[ARG_0]]: *mut slate_support::FILE) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_cookie_io_functions_t {
// CHECK-NEXT:     read: Option<unsafe extern "C-unwind" fn(*mut (), *mut i8, u64) -> i64>,
// CHECK-NEXT:     write: Option<unsafe extern "C-unwind" fn(*mut (), *const i8, u64) -> i64>,
// CHECK-NEXT:     seek: Option<unsafe extern "C-unwind" fn(*mut (), *mut i64, i32) -> i32>,
// CHECK-NEXT:     close: Option<unsafe extern "C-unwind" fn(*mut ()) -> i32>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct GNUCookie {
// CHECK-NEXT:     bytes: [i8; 32],
// CHECK-NEXT:     length: u64,
// CHECK-NEXT:     closed: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: use slate_support::memcmp;
// CHECK-NEXT: use slate_support::strcmp;
// CHECK-NEXT: use slate_support::strlen;
// CHECK-NEXT: extern "C-unwind" fn gnu_cookie_write(
// CHECK-NEXT:     state: *mut (),
// CHECK-NEXT:     buffer: *const i8,
// CHECK-NEXT:     size: u64,
// CHECK-NEXT: ) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((buffer as *const ()) as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((((*(state as *mut GNUCookie)).bytes.as_mut_ptr() as *mut i8)
// CHECK-NEXT:                 .offset((*(state as *mut GNUCookie)).length as isize) as *mut ())
// CHECK-NEXT:                 as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (size as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u64 = unsafe { (*(state as *mut GNUCookie)).length };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(state as *mut GNUCookie)).length = [[V_0]].wrapping_add(size);
// CHECK-NEXT:     }
// CHECK-NEXT:     size as i64
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn gnu_cookie_close(state: *mut ()) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(state as *mut GNUCookie)).closed = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_allocating_stdio() -> i32 {
// CHECK-NEXT:     let mut formatted: *mut i8 = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     let mut stream_data: *mut i8 = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     let mut stream_size: u64 = 0;
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         asprintf(
// CHECK-NEXT:             std::ptr::addr_of_mut!(formatted),
// CHECK-NEXT:             c"%s:%d".as_ptr(),
// CHECK-NEXT:             c"gnu".as_ptr() as *mut i8,
// CHECK-NEXT:             23i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 6) as i32;
// CHECK-NEXT:     total += [[V_1]];
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_2]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 (formatted as *const i8) as *const core::ffi::c_char,
// CHECK-NEXT:                 c"gnu:23".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     unsafe { free((formatted as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     let stream: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         open_memstream(
// CHECK-NEXT:             std::ptr::addr_of_mut!(stream_data),
// CHECK-NEXT:             std::ptr::addr_of_mut!(stream_size),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_3]] + (!stream.is_null() as i32);
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         slate_support::fprintf(
// CHECK-NEXT:             stream,
// CHECK-NEXT:             c"%s-%d".as_ptr(),
// CHECK-NEXT:             c"slate".as_ptr() as *mut i8,
// CHECK-NEXT:             24i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 8) as i32;
// CHECK-NEXT:     total += [[V_4]];
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: i32 = ((unsafe { __fwriting(stream) }) != 0) as i32;
// CHECK-NEXT:     total += [[V_5]];
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: i32 = ((unsafe { __fpending(stream) }) > 0) as i32;
// CHECK-NEXT:     total += [[V_6]];
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: i32 = ((unsafe { fflush(stream) }) == 0) as i32;
// CHECK-NEXT:     total += [[V_7]];
// CHECK-NEXT:     total += (stream_size == 8) as i32;
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_8]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 (stream_data as *const i8) as *const core::ffi::c_char,
// CHECK-NEXT:                 c"slate-24".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: i32 = ((unsafe { slate_support::fclose(stream) }) == 0) as i32;
// CHECK-NEXT:     total += [[V_9]];
// CHECK-NEXT:     unsafe { free((stream_data as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_memory_stdio() -> i32 {
// CHECK-NEXT:     let mut source: [i8; 12] = [97, 108, 112, 104, 97, 124, 98, 101, 116, 97, 10, 0];
// CHECK-NEXT:     let mut line: *mut i8 = std::ptr::null_mut::<i8>();
// CHECK-NEXT:     let mut capacity: u64 = 0;
// CHECK-NEXT:     let stream: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         fmemopen(
// CHECK-NEXT:             source.as_mut_ptr() as *mut (),
// CHECK-NEXT:             strlen((source.as_mut_ptr() as *const i8) as *const core::ffi::c_char)
// CHECK-NEXT:                 as u64,
// CHECK-NEXT:             c"r".as_ptr(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let [[V_1]]: i32 = total;
// CHECK-NEXT:     total = [[V_1]] + (!stream.is_null() as i32);
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: i32 = ((unsafe {
// CHECK-NEXT:         getdelim(
// CHECK-NEXT:             std::ptr::addr_of_mut!(line),
// CHECK-NEXT:             std::ptr::addr_of_mut!(capacity),
// CHECK-NEXT:             124i32,
// CHECK-NEXT:             stream,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 6) as i32;
// CHECK-NEXT:     total += [[V_10]];
// CHECK-NEXT:     let [[V_3]]: i32 = total;
// CHECK-NEXT:     total = [[V_3]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 (line as *const i8) as *const core::ffi::c_char,
// CHECK-NEXT:                 c"alpha|".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let [[V_4]]: i32 = ((unsafe {
// CHECK-NEXT:         getline(std::ptr::addr_of_mut!(line), std::ptr::addr_of_mut!(capacity), stream)
// CHECK-NEXT:     }) == 5) as i32;
// CHECK-NEXT:     total += [[V_4]];
// CHECK-NEXT:     let [[V_5]]: i32 = total;
// CHECK-NEXT:     total = [[V_5]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             strcmp(
// CHECK-NEXT:                 (line as *const i8) as *const core::ffi::c_char,
// CHECK-NEXT:                 c"beta\n".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: i32 = ((unsafe { __freading(stream) }) != 0) as i32;
// CHECK-NEXT:     total += [[V_11]];
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = ((unsafe { __fbufsize(stream) }) > 0) as i32;
// CHECK-NEXT:     total += [[V_12]];
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: i32 = ((unsafe { slate_support::fclose(stream) }) == 0) as i32;
// CHECK-NEXT:     total += [[V_13]];
// CHECK-NEXT:     unsafe { free((line as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_cookie_stdio() -> i32 {
// CHECK-NEXT:     let mut cookie: GNUCookie = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: GNUCookie = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut functions: _IO_cookie_io_functions_t = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: _IO_cookie_io_functions_t = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     functions.write = Some(gnu_cookie_write);
// CHECK-NEXT:     functions.close = Some(gnu_cookie_close);
// CHECK-NEXT:     let stream: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         fopencookie(std::ptr::addr_of_mut!(cookie) as *mut (), c"w".as_ptr(), functions)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_1]]: i32 = total;
// CHECK-NEXT:     total = [[V_1]] + (!stream.is_null() as i32);
// CHECK-NEXT:     let [[V_10]]: i32 = ((unsafe {
// CHECK-NEXT:         slate_support::fprintf(
// CHECK-NEXT:             stream,
// CHECK-NEXT:             c"%s:%d".as_ptr(),
// CHECK-NEXT:             c"cookie".as_ptr() as *mut i8,
// CHECK-NEXT:             7i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) == 8) as i32;
// CHECK-NEXT:     total += [[V_10]];
// CHECK-NEXT:     let [[V_3]]: i32 = ((unsafe { slate_support::fclose(stream) }) == 0) as i32;
// CHECK-NEXT:     total += [[V_3]];
// CHECK-NEXT:     total += (cookie.closed == 1) as i32;
// CHECK-NEXT:     total += (cookie.length == 8) as i32;
// CHECK-NEXT:     let [[V_11]]: i32 = total;
// CHECK-NEXT:     total = [[V_11]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 (cookie.bytes.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:                 (b"cookie:7\0".as_ptr() as *const ()) as *const core::ffi::c_void,
// CHECK-NEXT:                 8u64 as usize,
// CHECK-NEXT:             ) as i32
// CHECK-NEXT:         }) == 0) as i32);
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn gnu_printf_introspection() -> i32 {
// CHECK-NEXT:     let mut types: __SlateAlign16<[i32; 4]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0]]: [i32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let arguments: u64 = unsafe {
// CHECK-NEXT:         parse_printf_format(
// CHECK-NEXT:             c"%2$d %1$s".as_ptr(),
// CHECK-NEXT:             4u64,
// CHECK-NEXT:             types.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     (arguments == 2 && types.0[0] & !65280 == 3 && types.0[1] & !65280 == 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %d %d\n".as_ptr(),
// CHECK-NEXT:             gnu_allocating_stdio(),
// CHECK-NEXT:             gnu_memory_stdio(),
// CHECK-NEXT:             gnu_cookie_stdio(),
// CHECK-NEXT:             gnu_printf_introspection(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
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
