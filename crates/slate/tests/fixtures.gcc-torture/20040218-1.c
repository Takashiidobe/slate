/* PR target/14209.  Bug in cris.md, shrinking access size of
   postincrement.
   Origin: <hp@axis.com>.  */

void abort(void);
void exit(int);

long int  xb(long int *y) __attribute__((__noinline__));
long int  xw(long int *y) __attribute__((__noinline__));
short int yb(short int *y) __attribute__((__noinline__));

long int xb(long int *y) {
  long int xx = *y & 255;
  return xx + y[1];
}

long int xw(long int *y) {
  long int xx = *y & 65535;
  return xx + y[1];
}

short int yb(short int *y) {
  short int xx = *y & 255;
  return xx + y[1];
}

int main(void) {
  long int  y[]  = {-1, 16000};
  short int yw[] = {-1, 16000};

  if (xb(y) != 16255 || xw(y) != 81535 || yb(yw) != 16255)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/14209.  Bug in cris.md, shrinking access size of
// CHECK-NEXT: //! postincrement.
// CHECK-NEXT: //! Origin: <hp@axis.com>.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn xb(mut y: *mut i64) -> i64 {
// CHECK-NEXT:     let mut xx: i64 = (unsafe { *y }) & ((255 as i32) as i64);
// CHECK-NEXT:     xx + unsafe { *unsafe { y.offset((1 as i32) as isize) } }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn xw(mut y: *mut i64) -> i64 {
// CHECK-NEXT:     let mut xx: i64 = (unsafe { *y }) & ((65535 as i32) as i64);
// CHECK-NEXT:     xx + unsafe { *unsafe { y.offset((1 as i32) as isize) } }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn yb(mut y: *mut i16) -> i16 {
// CHECK-NEXT:     let mut xx: i16 = (((unsafe { *y }) as i32) & (255 as i32)) as i16;
// CHECK-NEXT:     ((xx as i32) + ((unsafe { *unsafe { y.offset((1 as i32) as isize) } }) as i32)) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut y: __SlateAlign16<[i64; 2]> =
// CHECK-NEXT:         __SlateAlign16([-(1 as i32) as i64, (16000 as i32) as i64]);
// CHECK-NEXT:     let mut yw: [i16; 2] = [-(1 as i32) as i16, (16000 as i32) as i16];
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if xb(y.0.as_mut_ptr() as *mut i64) != ((16255 as i32) as i64) {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = xw(y.0.as_mut_ptr() as *mut i64) != ((81535 as i32) as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = (yb(yw.as_mut_ptr() as *mut i16) as i32) != (16255 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_2]] {
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
