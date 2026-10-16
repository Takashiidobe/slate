extern void abort(void);
static void fixnum_neg(signed char x, signed char *py, int *pv) {
  unsigned char ux, uy;

  ux  = (unsigned char)x;
  uy  = -ux;
  *py = (uy <= 127) ? (signed char)uy : (-(signed char)(255 - uy) - 1);
  *pv = (x == -128) ? 1 : 0;
}

void __attribute__((noinline)) foo(int x, int y, int v) {
  if (y < -128 || y > 127)
    abort();
}

int test_neg(void) {
  signed char x, y;
  int         v, err;

  err = 0;
  x   = -128;
  for (;;) {
    fixnum_neg(x, &y, &v);
    foo((int)x, (int)y, v);
    if ((v && x != -128) || (!v && x == -128))
      ++err;
    if (x == 127)
      break;
    ++x;
  }
  return err;
}

int main(void) {
  if (sizeof(char) != 1)
    return 0;
  if (test_neg() != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn fixnum_neg(mut x: i8, mut py: &mut i8, mut pv: &mut i32) {
// CHECK-NEXT:     let mut ux: u8 = x as u8;
// CHECK-NEXT:     let mut uy: u8 = (-((ux as u32) as i32) as i8) as u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(py as *mut i8) = (if ((uy as u32) as i32) <= (127 as i32) {
// CHECK-NEXT:             (uy as i8) as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             -((((255 as i32) - ((uy as u32) as i32)) as i8) as i32) - (1 as i32)
// CHECK-NEXT:         }) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(pv as *mut i32) = if (x as i32) == -(128 as i32) {
// CHECK-NEXT:             1 as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0 as i32
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32, mut v: i32) {
// CHECK-NEXT:     if y < -(128 as i32) || y > (127 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn test_neg() -> i32 {
// CHECK-NEXT:     let mut x: i8 = -(128 as i32) as i8;
// CHECK-NEXT:     let mut y: i8 = 0 as i8;
// CHECK-NEXT:     let mut v: i32 = 0 as i32;
// CHECK-NEXT:     let mut err: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         fixnum_neg(
// CHECK-NEXT:             x,
// CHECK-NEXT:             unsafe { &mut (*std::ptr::addr_of_mut!(y)) },
// CHECK-NEXT:             unsafe { &mut (*std::ptr::addr_of_mut!(v)) },
// CHECK-NEXT:         );
// CHECK-NEXT:         foo(x as i32, y as i32, v);
// CHECK-NEXT:         if v != (0 as i32) && (x as i32) != -(128 as i32)
// CHECK-NEXT:             || !(v != (0 as i32)) && (x as i32) == -(128 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = err;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             err = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if (x as i32) == (127 as i32) {
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i8 = x;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i8 = (([[V_2]] as i32) + (1 as i32)) as i8;
// CHECK-NEXT:         x = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     err
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (1 as u64) != (((1 as i32) as i64) as u64) {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if test_neg() != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
