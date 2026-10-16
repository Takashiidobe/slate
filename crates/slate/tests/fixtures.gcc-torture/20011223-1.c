/* Origin: Joseph Myers <jsm28@cam.ac.uk>.  */
/* Case labels in a switch statement are converted to the promoted
   type of the controlling expression, not an unpromoted version.
   Reported as PR c/2454 by
   Andreas Krakowczyk <Andreas.Krakowczyk@fujitsu-siemens.com>.  */

extern void exit(int);
extern void abort(void);

static int i;

int main(void) {
  i = -1;
  switch ((signed char)i) {
  case 255:
    abort();
  default:
    exit(0);
  }
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Origin: Joseph Myers <jsm28@cam.ac.uk>.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Case labels in a switch statement are converted to the promoted
// CHECK-NEXT: //! type of the controlling expression, not an unpromoted version.
// CHECK-NEXT: //! Reported as PR c/2454 by
// CHECK-NEXT: //! Andreas Krakowczyk <Andreas.Krakowczyk@fujitsu-siemens.com>.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     match ((unsafe { i }) as i8) as i32 {
// CHECK-NEXT:         255 => {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:             unsafe { exit(0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             unsafe { exit(0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
