struct mouse_button_str {
  unsigned char left   : 1;
  unsigned char right  : 1;
  unsigned char middle : 1;
} button;

static char fct(struct mouse_button_str newbutton) __attribute__((__noipa__));
static char fct(struct mouse_button_str newbutton) {
  char l = newbutton.left;
  char r = newbutton.right;
  char m = newbutton.middle;
  return l && r && m;
}

int main(void) {
  struct mouse_button_str newbutton1;
  newbutton1.left   = 1;
  newbutton1.middle = 1;
  newbutton1.right  = 1;
  if (!fct(newbutton1))
    __builtin_abort();

  newbutton1.left   = 0;
  newbutton1.middle = 1;
  newbutton1.right  = 1;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 1;
  newbutton1.middle = 0;
  newbutton1.right  = 1;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 1;
  newbutton1.middle = 1;
  newbutton1.right  = 0;
  if (fct(newbutton1))
    __builtin_abort();

  newbutton1.left   = 1;
  newbutton1.middle = 0;
  newbutton1.right  = 0;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 0;
  newbutton1.middle = 1;
  newbutton1.right  = 0;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 0;
  newbutton1.middle = 0;
  newbutton1.right  = 1;
  if (fct(newbutton1))
    __builtin_abort();
  newbutton1.left   = 0;
  newbutton1.middle = 0;
  newbutton1.right  = 0;
  if (fct(newbutton1))
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct mouse_button_str {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub left: u8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub right: u8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub middle: u8,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_3: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut button: mouse_button_str = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn fct(mut newbutton: mouse_button_str) -> i8 {
// CHECK-NEXT:     let mut l: i8 = ((newbutton.__slate_bits_0.__get_left() as u32) as i32) as i8;
// CHECK-NEXT:     let mut r: i8 = ((newbutton.__slate_bits_0.__get_right() as u32) as i32) as i8;
// CHECK-NEXT:     let mut m: i8 = ((newbutton.__slate_bits_0.__get_middle() as u32) as i32) as i8;
// CHECK-NEXT:     (l != (0 as i8) && r != (0 as i8) && m != (0 as i8)) as i8
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut newbutton1: mouse_button_str = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_left(((1 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_middle(((1 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_right(((1 as i32) as i8) as u8);
// CHECK-NEXT:     if !(fct(newbutton1) != (0 as i8)) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_left(((0 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_middle(((1 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_right(((1 as i32) as i8) as u8);
// CHECK-NEXT:     if fct(newbutton1) != (0 as i8) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_left(((1 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_middle(((0 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_right(((1 as i32) as i8) as u8);
// CHECK-NEXT:     if fct(newbutton1) != (0 as i8) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_left(((1 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_middle(((1 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_right(((0 as i32) as i8) as u8);
// CHECK-NEXT:     if fct(newbutton1) != (0 as i8) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_left(((1 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_middle(((0 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_right(((0 as i32) as i8) as u8);
// CHECK-NEXT:     if fct(newbutton1) != (0 as i8) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_left(((0 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_middle(((1 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_right(((0 as i32) as i8) as u8);
// CHECK-NEXT:     if fct(newbutton1) != (0 as i8) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_left(((0 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_middle(((0 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_right(((1 as i32) as i8) as u8);
// CHECK-NEXT:     if fct(newbutton1) != (0 as i8) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_left(((0 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_middle(((0 as i32) as i8) as u8);
// CHECK-NEXT:     newbutton1.__slate_bits_0.__set_right(((0 as i32) as i8) as u8);
// CHECK-NEXT:     if fct(newbutton1) != (0 as i8) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
