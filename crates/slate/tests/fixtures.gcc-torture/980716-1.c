#include <stdarg.h>

void abort(void);
void exit(int);

void stub(int num, ...) {
  va_list ap;
  char   *end;
  int     i;

  for (i = 0; i < 2; i++) {
    va_start(ap, num);
    while (1) {
      end = va_arg(ap, char *);
      if (!end)
        break;
    }
    va_end(ap);
  }
}

int main() {
  stub(1, "ab", "bc", "cx", (char *)0);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn stub(mut num: i32, mut __va_args: ...) {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut end: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         ap = __va_args.clone();
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:             end = unsafe { ap.next_arg::<*mut i8>() };
// CHECK-NEXT:             if !(end != std::ptr::null_mut::<i8>()) {
// CHECK-NEXT:                 break '[[SLATE_BREAK_1]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         stub(
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:             c"ab".as_ptr() as *mut i8,
// CHECK-NEXT:             c"bc".as_ptr() as *mut i8,
// CHECK-NEXT:             c"cx".as_ptr() as *mut i8,
// CHECK-NEXT:             std::ptr::null_mut::<i8>(),
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
