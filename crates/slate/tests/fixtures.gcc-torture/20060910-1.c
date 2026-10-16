/* PR rtl-optimization/28636 */
/* Origin: Andreas Schwab <schwab@suse.de> */

extern void abort(void);

struct input_ty {
  unsigned char *buffer_position;
  unsigned char *buffer_end;
};

int input_getc_complicated(struct input_ty *x) { return 0; }

int check_header(struct input_ty *deeper) {
  unsigned len;
  for (len = 0; len < 6; len++)
    if (((deeper)->buffer_position < (deeper)->buffer_end
             ? *((deeper)->buffer_position)++
             : input_getc_complicated((deeper))) < 0)
      return 0;
  return 1;
}

struct input_ty s;
unsigned char   b[6];

int main(void) {
  s.buffer_position = b;
  s.buffer_end      = b + sizeof b;
  if (!check_header(&s))
    abort();
  if (s.buffer_position != s.buffer_end)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/28636
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Andreas Schwab <schwab@suse.de>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct input_ty {
// CHECK-NEXT:     buffer_position: *mut u8,
// CHECK-NEXT:     buffer_end: *mut u8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn input_getc_complicated(mut x: *mut input_ty) -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_header(mut deeper: *mut input_ty) -> i32 {
// CHECK-NEXT:     let mut len: u32 = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while len < ((6 as i32) as u32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:         if (unsafe { (*deeper).buffer_position }) < unsafe { (*deeper).buffer_end } {
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: *mut u8 = unsafe { (*deeper).buffer_position };
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: *mut u8 = unsafe { [[V_1]].offset((1 as i32) as isize) };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*deeper).buffer_position = [[V_2]];
// CHECK-NEXT:             }
// CHECK-NEXT:             [[V_0]] = ((unsafe { *[[V_1]] }) as u32) as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_0]] = input_getc_complicated(deeper);
// CHECK-NEXT:         }
// CHECK-NEXT:         if [[V_0]] < (0 as i32) {
// CHECK-NEXT:             return 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = len;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 = [[V_3]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         len = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: input_ty = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: [u8; 6] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.buffer_position = unsafe { std::ptr::addr_of_mut!(b) as *mut u8 };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.buffer_end =
// CHECK-NEXT:             unsafe { unsafe { std::ptr::addr_of_mut!(b) as *mut u8 }.offset((6 as u64) as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !(check_header(unsafe { std::ptr::addr_of_mut!(s) }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { s.buffer_position }) != unsafe { s.buffer_end } {
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
