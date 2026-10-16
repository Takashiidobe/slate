/* PR opt/10011 */
/* This is link test for builtin_constant_p simplification + DCE.  */

extern void link_error(void);
static void usb_hub_port_wait_reset(unsigned int delay) {
  int delay_time;
  for (delay_time = 0; delay_time < 500; delay_time += delay) {
    if (__builtin_constant_p(delay))
      link_error();
  }
}

int main() { return 0; }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR opt/10011
// CHECK-NEXT: //!
// CHECK-NEXT: //! This is link test for builtin_constant_p simplification + DCE.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn link_error();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn usb_hub_port_wait_reset(mut delay: u32) {
// CHECK-NEXT:     let mut delay_time: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while delay_time < (500 as i32) {
// CHECK-NEXT:         if (0 as i32) != (0 as i32) {
// CHECK-NEXT:             unsafe { link_error() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = delay_time;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = ([[V_0]] as u32).wrapping_add(delay) as i32;
// CHECK-NEXT:         delay_time = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
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
