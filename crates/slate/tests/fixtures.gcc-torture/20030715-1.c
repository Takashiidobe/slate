/* PR optimization/11320 */
/* Origin: Andreas Schwab <schwab@suse.de> */

/* Verify that the scheduler correctly computes the dependencies
   in the presence of conditional instructions.  */

int strcmp(const char *, const char *);
int ap_standalone;

const char *ap_check_cmd_context(void *a, int b) { return 0; }

const char *server_type(void *a, void *b, char *arg) {
  const char *err = ap_check_cmd_context(a, 0x01 | 0x02 | 0x04 | 0x08 | 0x10);
  if (err)
    return err;

  if (!strcmp(arg, "inetd"))
    ap_standalone = 0;
  else if (!strcmp(arg, "standalone"))
    ap_standalone = 1;
  else
    return "ServerType must be either 'inetd' or 'standalone'";

  return 0;
}

int main() {
  server_type(0, 0, "standalone");
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR optimization/11320
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Andreas Schwab <schwab@suse.de>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn strcmp([[V_0:__v[0-9]+]]: *const i8, [[V_1:__v[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Verify that the scheduler correctly computes the dependencies
// CHECK-NEXT: // in the presence of conditional instructions.
// CHECK-NEXT: static mut ap_standalone: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn ap_check_cmd_context(mut a: *mut (), mut b: i32) -> *const i8 {
// CHECK-NEXT:     std::ptr::null::<i8>()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn server_type(mut a: *mut (), mut b: *mut (), mut arg: *mut i8) -> *const i8 {
// CHECK-NEXT:     let mut err: *const i8 = ap_check_cmd_context(
// CHECK-NEXT:         a,
// CHECK-NEXT:         (1 as i32) | (2 as i32) | (4 as i32) | (8 as i32) | (16 as i32),
// CHECK-NEXT:     );
// CHECK-NEXT:     if err != std::ptr::null::<i8>() {
// CHECK-NEXT:         return err;
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { strcmp(arg as *const i8, c"inetd".as_ptr() as *const i8) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ap_standalone = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if !((unsafe { strcmp(arg as *const i8, c"standalone".as_ptr() as *const i8) })
// CHECK-NEXT:             != (0 as i32))
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 ap_standalone = 1 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             return c"ServerType must be either 'inetd' or 'standalone'".as_ptr() as *const i8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null::<i8>()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     server_type(
// CHECK-NEXT:         std::ptr::null_mut::<()>(),
// CHECK-NEXT:         std::ptr::null_mut::<()>(),
// CHECK-NEXT:         c"standalone".as_ptr() as *mut i8,
// CHECK-NEXT:     );
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
