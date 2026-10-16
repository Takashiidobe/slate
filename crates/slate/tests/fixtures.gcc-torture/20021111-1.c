/* Origin: PR c/8467 */

extern void abort(void);
extern void exit(int);

int aim_callhandler(int sess, int conn, unsigned short family,
                    unsigned short type);

int aim_callhandler(int sess, int conn, unsigned short family,
                    unsigned short type) {
  static int i = 0;

  if (!conn)
    return 0;

  if (type == 0xffff) {
    return 0;
  }

  if (i >= 1)
    abort();

  i++;
  return aim_callhandler(sess, conn, family, (unsigned short)0xffff);
}

int main(void) {
  aim_callhandler(0, 1, 0, 0);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Origin: PR c/8467
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn aim_callhandler(mut sess: i32, mut conn: i32, mut family: u16, mut r#type: u16) -> i32 {
// CHECK-NEXT:     if !(conn != (0 as i32)) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((r#type as u32) as i32) == (65535 as i32) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { i }) >= (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { i };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     aim_callhandler(sess, conn, family, ((65535 as i32) as i16) as u16)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     aim_callhandler(
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         ((0 as i32) as i16) as u16,
// CHECK-NEXT:         ((0 as i32) as i16) as u16,
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
