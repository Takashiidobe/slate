/* Test whether division by constant works properly.  */

extern void abort(void);
extern void exit(int);

unsigned char      cx = 7;
unsigned short     sx = 14;
unsigned int       ix = 21;
unsigned long      lx = 28;
unsigned long long Lx = 35;

int main() {
  unsigned char      cy;
  unsigned short     sy;
  unsigned int       iy;
  unsigned long      ly;
  unsigned long long Ly;

  cy = cx / 6;
  if (cy != 1)
    abort();
  cy = cx % 6;
  if (cy != 1)
    abort();

  sy = sx / 6;
  if (sy != 2)
    abort();
  sy = sx % 6;
  if (sy != 2)
    abort();

  iy = ix / 6;
  if (iy != 3)
    abort();
  iy = ix % 6;
  if (iy != 3)
    abort();

  ly = lx / 6;
  if (ly != 4)
    abort();
  ly = lx % 6;
  if (ly != 4)
    abort();

  Ly = Lx / 6;
  if (Ly != 5)
    abort();
  Ly = Lx % 6;
  if (Ly != 5)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test whether division by constant works properly.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut cx: u8 = ((7 as i32) as i8) as u8;
// CHECK-EMPTY:
// CHECK-NEXT: static mut sx: u16 = ((14 as i32) as i16) as u16;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ix: u32 = (21 as i32) as u32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut lx: u64 = ((28 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut Lx: u64 = ((35 as i32) as i64) as u64;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut cy: u8 = (((((unsafe { cx }) as u32) as i32) / (6 as i32)) as i8) as u8;
// CHECK-NEXT:     let mut sy: u16 = 0 as u16;
// CHECK-NEXT:     let mut iy: u32 = 0 as u32;
// CHECK-NEXT:     let mut ly: u64 = 0 as u64;
// CHECK-NEXT:     let mut Ly: u64 = 0 as u64;
// CHECK-NEXT:     if ((cy as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     cy = (((((unsafe { cx }) as u32) as i32) % (6 as i32)) as i8) as u8;
// CHECK-NEXT:     if ((cy as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     sy = (((((unsafe { sx }) as u32) as i32) / (6 as i32)) as i16) as u16;
// CHECK-NEXT:     if ((sy as u32) as i32) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     sy = (((((unsafe { sx }) as u32) as i32) % (6 as i32)) as i16) as u16;
// CHECK-NEXT:     if ((sy as u32) as i32) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     iy = (unsafe { ix }) / ((6 as i32) as u32);
// CHECK-NEXT:     if iy != ((3 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     iy = (unsafe { ix }) % ((6 as i32) as u32);
// CHECK-NEXT:     if iy != ((3 as i32) as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     ly = (unsafe { lx }) / (((6 as i32) as i64) as u64);
// CHECK-NEXT:     if ly != (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     ly = (unsafe { lx }) % (((6 as i32) as i64) as u64);
// CHECK-NEXT:     if ly != (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     Ly = (unsafe { Lx }) / (((6 as i32) as i64) as u64);
// CHECK-NEXT:     if Ly != (((5 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     Ly = (unsafe { Lx }) % (((6 as i32) as i64) as u64);
// CHECK-NEXT:     if Ly != (((5 as i32) as i64) as u64) {
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
