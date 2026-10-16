#include <stdarg.h>

void abort(void);
void exit(int);

typedef unsigned long L;
void f(L p0, L p1, L p2, L p3, L p4, L p5, L p6, L p7, L p8, ...) {
  va_list select;

  va_start(select, p8);

  if (va_arg(select, L) != 10)
    abort();
  if (va_arg(select, L) != 11)
    abort();
  if (va_arg(select, L) != 0)
    abort();

  va_end(select);
}

int main(void) {
  f(1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L, 9L, 10L, 11L, 0L);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn f(
// CHECK-NEXT:     mut p0: u64,
// CHECK-NEXT:     mut p1: u64,
// CHECK-NEXT:     mut p2: u64,
// CHECK-NEXT:     mut p3: u64,
// CHECK-NEXT:     mut p4: u64,
// CHECK-NEXT:     mut p5: u64,
// CHECK-NEXT:     mut p6: u64,
// CHECK-NEXT:     mut p7: u64,
// CHECK-NEXT:     mut p8: u64,
// CHECK-NEXT:     mut __va_args: ...
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut select: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     select = __va_args.clone();
// CHECK-NEXT:     if (unsafe { select.next_arg::<u64>() }) != (((10 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { select.next_arg::<u64>() }) != (((11 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { select.next_arg::<u64>() }) != (((0 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f(
// CHECK-NEXT:             (1 as i64) as u64,
// CHECK-NEXT:             (2 as i64) as u64,
// CHECK-NEXT:             (3 as i64) as u64,
// CHECK-NEXT:             (4 as i64) as u64,
// CHECK-NEXT:             (5 as i64) as u64,
// CHECK-NEXT:             (6 as i64) as u64,
// CHECK-NEXT:             (7 as i64) as u64,
// CHECK-NEXT:             (8 as i64) as u64,
// CHECK-NEXT:             (9 as i64) as u64,
// CHECK-NEXT:             10 as i64,
// CHECK-NEXT:             11 as i64,
// CHECK-NEXT:             0 as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
