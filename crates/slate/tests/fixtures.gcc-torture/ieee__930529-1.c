/* { dg-do run } */
void abort(void);
void exit(int);

int main(void) {
  union {
    double        d;
    unsigned char c[8];
  } d;

  d.d = 1.0 / 7.0;

  if (sizeof(char) * 8 == sizeof(double)) {
    if (d.c[0] == 0x92 && d.c[1] == 0x24 && d.c[2] == 0x49 && d.c[3] == 0x92 &&
        d.c[4] == 0x24 && d.c[5] == 0x49 && d.c[6] == 0xc2 && d.c[7] == 0x3f)
      exit(0);
    if (d.c[7] == 0x92 && d.c[6] == 0x24 && d.c[5] == 0x49 && d.c[4] == 0x92 &&
        d.c[3] == 0x24 && d.c[2] == 0x49 && d.c[1] == 0xc2 && d.c[0] == 0x3f)
      exit(0);
#if defined __arm__ || defined __thumb__
    if (d.c[4] == 0x92 && d.c[5] == 0x24 && d.c[6] == 0x49 && d.c[7] == 0x92 &&
        d.c[0] == 0x24 && d.c[1] == 0x49 && d.c[2] == 0xc2 && d.c[3] == 0x3f)
      exit(0);
#endif
    abort();
  }

  exit(0);
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
// CHECK-NEXT:     c: [u8; 8],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-do run }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d.d = 1.0f64 / 7.0f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (1 as u64).wrapping_mul(((8 as i32) as i64) as u64) == (8 as u64) {
// CHECK-NEXT:         if (((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((0 as i32) as isize) }
// CHECK-NEXT:         }) as u32) as i32)
// CHECK-NEXT:             == (146 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((1 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (36 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((2 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (73 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((3 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (146 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((4 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (36 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((5 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (73 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((6 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (194 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((7 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (63 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { exit(0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:         if (((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((7 as i32) as isize) }
// CHECK-NEXT:         }) as u32) as i32)
// CHECK-NEXT:             == (146 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((6 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (36 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((5 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (73 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((4 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (146 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((3 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (36 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((2 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (73 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((1 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (194 as i32)
// CHECK-NEXT:             && (((unsafe {
// CHECK-NEXT:                 *unsafe { unsafe { d.c.as_mut_ptr() as *mut u8 }.offset((0 as i32) as isize) }
// CHECK-NEXT:             }) as u32) as i32)
// CHECK-NEXT:                 == (63 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { exit(0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { abort() };
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
