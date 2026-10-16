/* PR middle-end/90348 */

void __attribute__((noipa)) set_one(unsigned char *ptr) { *ptr = 1; }

void __attribute__((noipa)) check_zero(unsigned char const *in,
                                       unsigned int         len) {
  for (unsigned int i = 0; i < len; ++i)
    if (in[i] != 0)
      __builtin_abort();
}

static void set_one_on_stack(void) {
  unsigned char buf[1];
  set_one(buf);
}

int main() {
  for (int i = 0; i <= 4; ++i) {
    unsigned char in[4];
    for (int j = 0; j < i; ++j) {
      in[j] = 0;
      set_one_on_stack();
    }
    check_zero(in, i);
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/90348
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn set_one(mut ptr: *mut u8) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *ptr = ((1 as i32) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_zero(mut r#in: *const u8, mut len: u32) {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < len {
// CHECK-NEXT:         if (((unsafe { *unsafe { r#in.offset(i as isize) } }) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn set_one_on_stack() {
// CHECK-NEXT:     let mut buf: [u8; 1] = [0 as u8; 1];
// CHECK-NEXT:     set_one(buf.as_mut_ptr() as *mut u8);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i <= (4 as i32) {
// CHECK-NEXT:         let mut r#in: [u8; 4] = [0 as u8; 4];
// CHECK-NEXT:         let mut j: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while j < i {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { (r#in.as_mut_ptr() as *mut u8).offset(j as isize) } =
// CHECK-NEXT:                     ((0 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             set_one_on_stack();
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:             j = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         check_zero(r#in.as_mut_ptr() as *const u8, i as u32);
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:         i = [[V_5]];
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
