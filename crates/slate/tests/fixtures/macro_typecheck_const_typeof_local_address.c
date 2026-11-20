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
// CHECK-NEXT: macro_rules! time_after {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         {
// CHECK-NEXT:             let __v: bool = typecheck!(u64) != 0 && typecheck__2!(u64) != 0;
// CHECK-NEXT:             __v && ($b.wrapping_sub($a) as i64) < 0
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! time_before {
// CHECK-NEXT:     ($a:expr, $b:expr) => {
// CHECK-NEXT:         time_after!($b, $a)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! typecheck {
// CHECK-NEXT:     ($type:ty) => {
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut __v: i32;
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: $type = 0;
// CHECK-NEXT:             let [[V_1:__v[0-9]+]]: $type = 0;
// CHECK-NEXT:             core::ptr::addr_of_mut!([[V_0]]) == (core::ptr::addr_of!([[V_1]]) as *mut $type);
// CHECK-NEXT:             __v = 1;
// CHECK-NEXT:             __v
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! typecheck__2 {
// CHECK-NEXT:     ($type:ty) => {
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut __v: i32;
// CHECK-NEXT:             let mut [[V_0]]: $type = 0;
// CHECK-NEXT:             let mut [[V_1]]: $type = 0;
// CHECK-NEXT:             core::ptr::addr_of_mut!([[V_0]]) == core::ptr::addr_of_mut!([[V_1]]);
// CHECK-NEXT:             __v = 1;
// CHECK-NEXT:             __v
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
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
// CHECK-NEXT: fn next_timeout(req: *const request, now: u64) -> u64 {
// CHECK-NEXT:     let timeout: u64 = now.wrapping_add(unsafe { (*req).timeout });
// CHECK-NEXT:     if time_before!(timeout, unsafe { (*req).deadline }) {
// CHECK-NEXT:         return timeout;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { (*req).deadline }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut req: request = request {
// CHECK-NEXT:         timeout: 10,
// CHECK-NEXT:         deadline: 40,
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lu %lu\n".as_ptr(),
// CHECK-NEXT:             next_timeout(core::ptr::addr_of_mut!(req) as *const request, 5u64),
// CHECK-NEXT:             next_timeout(core::ptr::addr_of_mut!(req) as *const request, 50u64),
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
