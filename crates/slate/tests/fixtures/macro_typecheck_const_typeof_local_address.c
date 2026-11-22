#include <stdio.h>

#define typecheck(type, x)                                                     \
  ({                                                                           \
    type __dummy;                                                              \
    typeof(x) __dummy2;                                                        \
    (void)(&__dummy == &__dummy2);                                             \
    1;                                                                         \
  })

#define time_after(a, b)                                                       \
  (typecheck(unsigned long, a) && typecheck(unsigned long, b) &&               \
   ((long)((b) - (a)) < 0))
#define time_before(a, b) time_after(b, a)

struct request {
  unsigned long timeout;
  unsigned long deadline;
};

static unsigned long next_timeout(const struct request *req,
                                  unsigned long now) {
  unsigned long timeout = now + req->timeout;
  if (time_before(timeout, req->deadline))
    return timeout;
  return req->deadline;
}

int main(void) {
  struct request req = {10, 40};
  printf("%lu %lu\n", next_timeout(&req, 5), next_timeout(&req, 50));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct request {
// CHECK-NEXT:     timeout: u64,
// CHECK-NEXT:     deadline: u64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn next_timeout(req: &request, now: u64) -> u64 {
// CHECK-NEXT:     let timeout: u64 = now.wrapping_add((*req).timeout);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __dummy: u64 = 0;
// CHECK-NEXT:         let mut __dummy2: u64 = 0;
// CHECK-NEXT:         core::ptr::addr_of_mut!(__dummy) == (core::ptr::addr_of!(__dummy2) as *mut u64);
// CHECK-NEXT:         [[V_0]] = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bool = if [[V_0]] != 0 {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32;
// CHECK-NEXT:         let mut __dummy: u64 = 0;
// CHECK-NEXT:         let mut __dummy2: u64 = 0;
// CHECK-NEXT:         core::ptr::addr_of_mut!(__dummy) == core::ptr::addr_of_mut!(__dummy2);
// CHECK-NEXT:         [[V_2]] = 1;
// CHECK-NEXT:         [[V_2]] != 0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         false
// CHECK-NEXT:     };
// CHECK-NEXT:     if [[V_1]] && (timeout.wrapping_sub((*req).deadline) as i64) < 0 {
// CHECK-NEXT:         return timeout;
// CHECK-NEXT:     }
// CHECK-NEXT:     (*req).deadline
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let req: request = request {
// CHECK-NEXT:         timeout: 10,
// CHECK-NEXT:         deadline: 40,
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lu %lu\n".as_ptr(),
// CHECK-NEXT:             next_timeout(&req, 5u64),
// CHECK-NEXT:             next_timeout(&req, 50u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
