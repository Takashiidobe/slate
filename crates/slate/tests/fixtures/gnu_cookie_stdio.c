#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

struct GNUCookie {
  char   bytes[32];
  size_t length;
  int    closed;
};

// @check-fn-begin
static ssize_t gnu_cookie_write(void *state, const char *buffer, size_t size) {
  struct GNUCookie *cookie = state;
  memcpy(cookie->bytes + cookie->length, buffer, size);
  cookie->length += size;
  return (ssize_t)size;
}
// @check-fn-end

// @check-fn-begin
static int gnu_cookie_close(void *state) {
  struct GNUCookie *cookie = state;
  cookie->closed           = 1;
  return 0;
}
// @check-fn-end

// @check-fn-begin
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
// @check-fn-end

int main(void) {
  printf("%d\n", gnu_cookie_stdio());
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK-LABEL: {{^}}extern "C-unwind" fn gnu_cookie_write(
// CHECK-NEXT:     state: *mut (),
// CHECK-NEXT:     buffer: *const i8,
// CHECK-NEXT:     size: u64,
// CHECK-NEXT: ) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             buffer as *const u8,
// CHECK-NEXT:             ((unsafe {
// CHECK-NEXT:                 unsafe { (*(state as *mut GNUCookie)).bytes.as_mut_ptr() as *mut i8 }
// CHECK-NEXT:                     .offset((unsafe { (*(state as *mut GNUCookie)).length }) as isize)
// CHECK-NEXT:             }) as *mut ()) as *mut u8,
// CHECK-NEXT:             size as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u64 = unsafe { (*(state as *mut GNUCookie)).length };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(state as *mut GNUCookie)).length = [[V_0]].wrapping_add(size);
// CHECK-NEXT:     }
// CHECK-NEXT:     size as i64
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}extern "C-unwind" fn gnu_cookie_close(state: *mut ()) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(state as *mut GNUCookie)).closed = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn gnu_cookie_stdio() -> i32 {
// CHECK-NEXT:     let mut cookie: GNUCookie = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: GNUCookie = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut functions: _IO_cookie_io_functions_t = {
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: _IO_cookie_io_functions_t = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_1]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut stream: *mut _IO_FILE = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut total: i32 = 0 as i32;
// CHECK-NEXT:     functions.write = Some(gnu_cookie_write);
// CHECK-NEXT:     functions.close = Some(gnu_cookie_close);
// CHECK-NEXT:     stream = unsafe {
// CHECK-NEXT:         fopencookie(
// CHECK-NEXT:             std::ptr::addr_of_mut!(cookie) as *mut (),
// CHECK-NEXT:             c"w".as_ptr() as *const i8,
// CHECK-NEXT:             functions,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_0]] + ((stream != std::ptr::null_mut::<_IO_FILE>()) as i32);
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_1]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             fprintf(
// CHECK-NEXT:                 stream,
// CHECK-NEXT:                 c"%s:%d".as_ptr() as *const i8,
// CHECK-NEXT:                 c"cookie".as_ptr() as *mut i8,
// CHECK-NEXT:                 7 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == (8 as i32)) as i32);
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_2]] + (((unsafe { fclose(stream) }) == (0 as i32)) as i32);
// CHECK-NEXT:     total += (cookie.closed == (1 as i32)) as i32;
// CHECK-NEXT:     total += (cookie.length == (((8 as i32) as i64) as u64)) as i32;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: i32 = total;
// CHECK-NEXT:     total = [[V_3]]
// CHECK-NEXT:         + (((unsafe {
// CHECK-NEXT:             memcmp(
// CHECK-NEXT:                 cookie.bytes.as_mut_ptr() as *const (),
// CHECK-NEXT:                 b"cookie:7\0".as_ptr() as *const (),
// CHECK-NEXT:                 ((8 as i32) as i64) as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         }) == (0 as i32)) as i32);
// CHECK-NEXT:     total
// CHECK: {{^}}}
// SLATE-FILECHECK-END
