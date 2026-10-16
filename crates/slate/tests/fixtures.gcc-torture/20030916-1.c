/* "i" overflows in f().  Check that x[i] is not treated as a giv.  */
#include <limits.h>

void abort(void);
void exit(int);

#if CHAR_BIT == 8

void f(unsigned int *x) {
  unsigned char i;
  int           j;

  i = 0x10;
  for (j = 0; j < 0x10; j++) {
    i    += 0xe8;
    x[i]  = 0;
    i    -= 0xe7;
  }
}

int main() {
  unsigned int x[256];
  int          i;

  for (i = 0; i < 256; i++)
    x[i] = 1;
  f(x);
  for (i = 0; i < 256; i++)
    if (x[i] != (i >= 0x08 && i < 0xf8))
      abort();
  exit(0);
}
#else
int main() { exit(0); }
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! "i" overflows in f().  Check that x[i] is not treated as a giv.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut x: *mut u32) {
// CHECK-NEXT:     let mut i: u8 = ((16 as i32) as i8) as u8;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < (16 as i32) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u8 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u8 = (((([[V_1]] as u32) as i32) + (232 as i32)) as i8) as u8;
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { x.offset(((i as u32) as i32) as isize) } = (0 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u8 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u8 = (((([[V_3]] as u32) as i32) - (231 as i32)) as i8) as u8;
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         j = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: __SlateAlign16<[u32; 256]> = __SlateAlign16([0 as u32; 256]);
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < (256 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { (x.0.as_mut_ptr() as *mut u32).offset(i as isize) } = (1 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i32 = [[V_7]] + (1 as i32);
// CHECK-NEXT:         i = [[V_8]];
// CHECK-NEXT:     }
// CHECK-NEXT:     f(x.0.as_mut_ptr() as *mut u32);
// CHECK-NEXT:     i = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while i < (256 as i32) {
// CHECK-NEXT:         if (unsafe { *unsafe { (x.0.as_mut_ptr() as *mut u32).offset(i as isize) } })
// CHECK-NEXT:             != (((i >= (8 as i32) && i < (248 as i32)) as i32) as u32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = [[V_9]] + (1 as i32);
// CHECK-NEXT:         i = [[V_10]];
// CHECK-NEXT:     }
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
