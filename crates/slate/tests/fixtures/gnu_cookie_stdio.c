#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

struct GNUCookie {
  char   bytes[32];
  size_t length;
  int    closed;
};

// @lowering-fn-begin
// @rewrite-fn-begin
static ssize_t gnu_cookie_write(void *state, const char *buffer, size_t size) {
  struct GNUCookie *cookie = state;
  memcpy(cookie->bytes + cookie->length, buffer, size);
  cookie->length += size;
  return (ssize_t)size;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int gnu_cookie_close(void *state) {
  struct GNUCookie *cookie = state;
  cookie->closed = 1;
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

static int gnu_cookie_stdio(void) {
  struct GNUCookie      cookie    = {};
  cookie_io_functions_t functions = {};
  FILE                 *stream;
  int                   total = 0;

  // @lowering-begin
  // @rewrite-begin
  functions.write = gnu_cookie_write;
  functions.close = gnu_cookie_close;
  stream          = fopencookie(&cookie, "w", functions);
  // @rewrite-end
  // @lowering-end
  total += stream != NULL;
  total += fprintf(stream, "%s:%d", "cookie", 7) == 8;
  total += fclose(stream) == 0;
  total += cookie.closed == 1;
  total += cookie.length == 8;
  total += memcmp(cookie.bytes, "cookie:7", 8) == 0;
  return total;
}

int main(void) {
  printf("%d\n", gnu_cookie_stdio());
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: extern "C-unwind" fn gnu_cookie_write(
// LOWERING-DAG:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-DAG:     {{arg[0-9]+}}: *mut i8,
// LOWERING-DAG:     {{arg[0-9]+}}: u64,
// LOWERING-DAG: ) -> i64 {
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{_v[0-9]+}}).bytes) }) as *mut i8;
// LOWERING-DAG:     let {{_v[0-9]+}}: u64 = unsafe { (*{{_v[0-9]+}}).length };
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add({{_v[0-9]+}} as usize) };
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-DAG:         memcpy(
// LOWERING-DAG:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-DAG:             {{arg[0-9]+}} as usize,
// LOWERING-DAG:         )
// LOWERING-DAG:     };
// LOWERING-DAG:     let {{_v[0-9]+}}: u64 = unsafe { (*{{_v[0-9]+}}).length };
// LOWERING-DAG:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*{{_v[0-9]+}}).length = {{_v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i64 = {{arg[0-9]+}} as i64;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: extern "C-unwind" fn gnu_cookie_close({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-DAG:     let {{_v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     unsafe {
// LOWERING-DAG:         (*{{_v[0-9]+}}).closed = {{_v[0-9]+}};
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     return {{_v[0-9]+}};
// LOWERING-DAG: }
// LOWERING-DAG: functions.write = unsafe {
// LOWERING-DAG:     std::mem::transmute::<
// LOWERING-DAG:         *const (),
// LOWERING-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// LOWERING-DAG:     >(gnu_cookie_write as *const ())
// LOWERING-DAG: };
// LOWERING-DAG: functions.close = unsafe {
// LOWERING-DAG:     std::mem::transmute::<
// LOWERING-DAG:         *const (),
// LOWERING-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// LOWERING-DAG:     >(gnu_cookie_close as *const ())
// LOWERING-DAG: };
// LOWERING-DAG: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(cookie) as *mut core::ffi::c_void;
// LOWERING-DAG: let {{_v[0-9]+}}: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-DAG: let {{_v[0-9]+}}: _IO_cookie_io_functions_t = functions;
// LOWERING-DAG: let mut byval: _IO_cookie_io_functions_t = _IO_cookie_io_functions_t {
// LOWERING-DAG:     read: None,
// LOWERING-DAG:     write: None,
// LOWERING-DAG:     seek: None,
// LOWERING-DAG:     close: None,
// LOWERING-DAG: };
// LOWERING-DAG: byval = {{_v[0-9]+}};
// LOWERING-DAG: let {{_v[0-9]+}}: *mut libc::FILE = unsafe {
// LOWERING-DAG:     fopencookie(
// LOWERING-DAG:         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-DAG:         {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-DAG:         unsafe {
// LOWERING-DAG:             std::ptr::read_unaligned(
// LOWERING-DAG:                 std::ptr::addr_of_mut!(byval) as *const _IO_cookie_io_functions_t
// LOWERING-DAG:             )
// LOWERING-DAG:         },
// LOWERING-DAG:     )
// LOWERING-DAG: };
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: extern "C-unwind" fn gnu_cookie_write(
// REWRITES-DAG:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-DAG:     {{arg[0-9]+}}: *mut i8,
// REWRITES-DAG:     {{arg[0-9]+}}: u64,
// REWRITES-DAG: ) -> i64 {
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{_v[0-9]+}}).bytes) }) as *mut i8;
// REWRITES-DAG:     let {{_v[0-9]+}}: u64 = unsafe { (*{{_v[0-9]+}}).length };
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add({{_v[0-9]+}} as usize) };
// REWRITES-DAG:     unsafe { std::ptr::copy_nonoverlapping({{arg[0-9]+}} as *const u8, {{_v[0-9]+}} as *mut u8, {{arg[0-9]+}} as usize) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*{{_v[0-9]+}}).length = (unsafe { (*{{_v[0-9]+}}).length }) + {{arg[0-9]+}};
// REWRITES-DAG:     }
// REWRITES-DAG:     {{arg[0-9]+}} as i64
// REWRITES-DAG: }
// REWRITES-DAG: extern "C-unwind" fn gnu_cookie_close({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         (*({{arg[0-9]+}} as *mut GNUCookie)).closed = 1;
// REWRITES-DAG:     }
// REWRITES-DAG:     0
// REWRITES-DAG: }
// REWRITES-DAG: functions.write = unsafe {
// REWRITES-DAG:     std::mem::transmute::<
// REWRITES-DAG:         *const (),
// REWRITES-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// REWRITES-DAG:     >(gnu_cookie_write as *const ())
// REWRITES-DAG: };
// REWRITES-DAG: functions.close = unsafe {
// REWRITES-DAG:     std::mem::transmute::<
// REWRITES-DAG:         *const (),
// REWRITES-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// REWRITES-DAG:     >(gnu_cookie_close as *const ())
// REWRITES-DAG: };
// REWRITES-DAG: let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(cookie) as *mut core::ffi::c_void;
// REWRITES-DAG: let {{_v[0-9]+}}: *mut i8 = c"w".as_ptr() as *mut i8;
// REWRITES-DAG: let {{_v[0-9]+}}: _IO_cookie_io_functions_t = functions;
// REWRITES-DAG: let mut byval: _IO_cookie_io_functions_t = {{_v[0-9]+}};
// REWRITES-DAG: let {{_v[0-9]+}}: *mut libc::FILE = unsafe {
// REWRITES-DAG:     fopencookie(
// REWRITES-DAG:         {{_v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-DAG:         {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-DAG:         unsafe {
// REWRITES-DAG:             std::ptr::read_unaligned(
// REWRITES-DAG:                 std::ptr::addr_of_mut!(byval) as *const _IO_cookie_io_functions_t
// REWRITES-DAG:             )
// REWRITES-DAG:         },
// REWRITES-DAG:     )
// REWRITES-DAG: };
// SLATE-FILECHECK-END rewrites
