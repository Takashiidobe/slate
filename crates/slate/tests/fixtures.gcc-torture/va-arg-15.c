#include <stdarg.h>

void abort(void);
void exit(int);

void vafunction(char *dummy, ...) {
  double  darg;
  int     iarg;
  int     flag = 0;
  int     i;
  va_list ap;

  va_start(ap, dummy);
  for (i = 1; i <= 18; i++, flag++) {
    if (flag & 1) {
      darg = va_arg(ap, double);
      if (darg != (double)i)
        abort();
    } else {
      iarg = va_arg(ap, int);
      if (iarg != i)
        abort();
    }
  }
  va_end(ap);
}

int main(void) {
  vafunction("", 1, 2., 3, 4., 5, 6., 7, 8., 9, 10., 11, 12., 13, 14., 15, 16.,
             17, 18.);
  exit(0);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn vafunction(mut dummy: *mut i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut darg: f64 = 0 as f64;
// CHECK-NEXT:     let mut iarg: i32 = 0 as i32;
// CHECK-NEXT:     let mut flag: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = __va_args.clone();
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i <= (18 as i32) {
// CHECK-NEXT:         if flag & (1 as i32) != (0 as i32) {
// CHECK-NEXT:             darg = unsafe { ap.next_arg::<f64>() };
// CHECK-NEXT:             if darg != (i as f64) {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             iarg = unsafe { ap.next_arg::<i32>() };
// CHECK-NEXT:             if iarg != i {
// CHECK-NEXT:                 unsafe { abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = flag;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:         flag = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         vafunction(
// CHECK-NEXT:             c"".as_ptr() as *mut i8,
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             2.0f64,
// CHECK-NEXT:             3 as i32,
// CHECK-NEXT:             4.0f64,
// CHECK-NEXT:             5 as i32,
// CHECK-NEXT:             6.0f64,
// CHECK-NEXT:             7 as i32,
// CHECK-NEXT:             8.0f64,
// CHECK-NEXT:             9 as i32,
// CHECK-NEXT:             10.0f64,
// CHECK-NEXT:             11 as i32,
// CHECK-NEXT:             12.0f64,
// CHECK-NEXT:             13 as i32,
// CHECK-NEXT:             14.0f64,
// CHECK-NEXT:             15 as i32,
// CHECK-NEXT:             16.0f64,
// CHECK-NEXT:             17 as i32,
// CHECK-NEXT:             18.0f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0 as i32
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
