/* Reduced from PR optimization/5076, PR optimization/2847 */

void abort(void);

static int count = 0;

static void inc(void) { count++; }

int main(void) {
  int iNbr = 1;
  int test = 0;
  while (test == 0) {
    inc();
    if (iNbr == 0)
      break;
    else {
      inc();
      iNbr--;
    }
    test = 1;
  }
  if (count != 2)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Reduced from PR optimization/5076, PR optimization/2847
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut count: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn inc() {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { count };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut iNbr: i32 = 1 as i32;
// CHECK-NEXT:     let mut test: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while test == (0 as i32) {
// CHECK-NEXT:         inc();
// CHECK-NEXT:         if iNbr == (0 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         } else {
// CHECK-NEXT:             inc();
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = iNbr;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:             iNbr = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         test = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { count }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
