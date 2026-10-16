/* { dg-do run } */
void abort(void);
void exit(int);

int main(void) {
  union {
    double         d;
    unsigned short i[sizeof(double) / sizeof(short)];
  } u;
  int a = 0;
  int b = -5;
  int j;

  u.d = (double)a / b;

  /* Look for the right pattern, but be sloppy since
     we don't know the byte order.  */
  for (j = 0; j < sizeof(double) / sizeof(short); j++) {
    if (u.i[j] == 0x8000)
      exit(0);
  }
  abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     d: f64,
// CHECK-NEXT:     i: [u16; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-do run }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     let mut b: i32 = -(5 as i32);
// CHECK-NEXT:     // Look for the right pattern, but be sloppy since
// CHECK-NEXT:     // we don't know the byte order.
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.d = (a as f64) / (b as f64);
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((j as i64) as u64) < (8 as u64) / (2 as u64) {
// CHECK-NEXT:         if (((unsafe { *unsafe { unsafe { u.i.as_mut_ptr() as *mut u16 }.offset(j as isize) } })
// CHECK-NEXT:             as u32) as i32)
// CHECK-NEXT:             == (32768 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { exit(0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         j = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { abort() };
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
