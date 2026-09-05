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
  cookie->closed           = 1;
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
  functions.write  = gnu_cookie_write;
  functions.close  = gnu_cookie_close;
  stream           = fopencookie(&cookie, "w", functions);
  // @rewrite-end
  // @lowering-end
  total           += stream != NULL;
  total           += fprintf(stream, "%s:%d", "cookie", 7) == 8;
  total           += fclose(stream) == 0;
  total           += cookie.closed == 1;
  total           += cookie.length == 8;
  total           += memcmp(cookie.bytes, "cookie:7", 8) == 0;
  return total;
}

int main(void) {
  printf("%d\n", gnu_cookie_stdio());
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: extern "C-unwind" fn gnu_cookie_write(
// COMMON-LOWERING-DAG:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-DAG:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-DAG: ) -> i64 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).length };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// COMMON-LOWERING-DAG:         memcpy(
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{__v[0-9]+}} as *const core::ffi::c_void,
// COMMON-LOWERING-DAG:             {{arg[0-9]+}} as usize,
// COMMON-LOWERING-DAG:         )
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).length };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-DAG:     unsafe {
// COMMON-LOWERING-DAG:         (*{{__v[0-9]+}}).length = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = {{arg[0-9]+}} as i64;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: extern "C-unwind" fn gnu_cookie_close({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     unsafe {
// COMMON-LOWERING-DAG:         (*{{__v[0-9]+}}).closed = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: functions.write = unsafe {
// COMMON-LOWERING-DAG:     std::mem::transmute::<
// COMMON-LOWERING-DAG:         *const (),
// COMMON-LOWERING-DAG:     >(gnu_cookie_write as *const ())
// COMMON-LOWERING-DAG: };
// COMMON-LOWERING-DAG: functions.close = unsafe {
// COMMON-LOWERING-DAG:     std::mem::transmute::<
// COMMON-LOWERING-DAG:         *const (),
// COMMON-LOWERING-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// COMMON-LOWERING-DAG:     >(gnu_cookie_close as *const ())
// COMMON-LOWERING-DAG: };
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(cookie) as *mut core::ffi::c_void;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: _IO_cookie_io_functions_t = functions;
// COMMON-LOWERING-DAG: let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// COMMON-LOWERING-DAG:     fopencookie(
// COMMON-LOWERING-DAG:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-DAG:         {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-DAG:         {{__v[0-9]+}} as _IO_cookie_io_functions_t,
// COMMON-LOWERING-DAG:     )
// COMMON-LOWERING-DAG: };
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     {{arg[0-9]+}}: *mut i8,
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut i8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-X86_64-GNU-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: *mut i8 = b"w\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     {{arg[0-9]+}}: *mut u8,
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut u8;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// LOWERING-AARCH64-GNU-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: *mut u8 = b"w\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: extern "C-unwind" fn gnu_cookie_write(
// COMMON-REWRITES-DAG:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-DAG:     {{arg[0-9]+}}: u64,
// COMMON-REWRITES-DAG: ) -> i64 {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: *mut GNUCookie = {{arg[0-9]+}} as *mut GNUCookie;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u64 = unsafe { (*{{__v[0-9]+}}).length };
// COMMON-REWRITES-DAG:     unsafe { std::ptr::copy_nonoverlapping({{arg[0-9]+}} as *const u8, {{__v[0-9]+}} as *mut u8, {{arg[0-9]+}} as usize) };
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         (*{{__v[0-9]+}}).length = (unsafe { (*{{__v[0-9]+}}).length }) + {{arg[0-9]+}};
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     {{arg[0-9]+}} as i64
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: extern "C-unwind" fn gnu_cookie_close({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-REWRITES-DAG:     unsafe {
// COMMON-REWRITES-DAG:         (*({{arg[0-9]+}} as *mut GNUCookie)).closed = 1;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     0
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: functions.write = unsafe {
// COMMON-REWRITES-DAG:     std::mem::transmute::<
// COMMON-REWRITES-DAG:         *const (),
// COMMON-REWRITES-DAG:     >(gnu_cookie_write as *const ())
// COMMON-REWRITES-DAG: };
// COMMON-REWRITES-DAG: functions.close = unsafe {
// COMMON-REWRITES-DAG:     std::mem::transmute::<
// COMMON-REWRITES-DAG:         *const (),
// COMMON-REWRITES-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// COMMON-REWRITES-DAG:     >(gnu_cookie_close as *const ())
// COMMON-REWRITES-DAG: };
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(cookie) as *mut core::ffi::c_void;
// COMMON-REWRITES-DAG: let {{__v[0-9]+}}: *mut libc::FILE = unsafe {
// COMMON-REWRITES-DAG:     fopencookie(
// COMMON-REWRITES-DAG:         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-DAG:         c"w".as_ptr(),
// COMMON-REWRITES-DAG:         functions as _IO_cookie_io_functions_t,
// COMMON-REWRITES-DAG:     )
// COMMON-REWRITES-DAG: };
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     {{arg[0-9]+}}: *mut i8,
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut i8;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: *mut i8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// REWRITES-X86_64-GNU-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut i8, u64) -> i64>,
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     {{arg[0-9]+}}: *mut u8,
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = (unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).bytes) }) as *mut u8;
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// REWRITES-AARCH64-GNU-DAG:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64) -> i64>,
// SLATE-FILECHECK-END rewrites-aarch64-gnu
