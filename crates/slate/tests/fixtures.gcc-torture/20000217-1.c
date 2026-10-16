void abort(void);
void exit(int);

unsigned short int showbug(unsigned short int *a, unsigned short int *b) {
  *a += *b - 8;
  return (*a >= 8);
}

int main() {
  unsigned short int x = 0;
  unsigned short int y = 10;

  if (showbug(&x, &y) != 0)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn showbug(mut a: &mut u16, mut b: &u16) -> u16 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u16 = unsafe { *a };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u16 = (((([[V_1]] as u32) as i32)
// CHECK-NEXT:         + ((((unsafe { *(b as *const u16) }) as u32) as i32) - (8 as i32))) as i16)
// CHECK-NEXT:         as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *a = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     ((((unsafe { *(a as *mut u16) }) as u32) as i32) >= (8 as i32)) as u16
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u16 = ((0 as i32) as i16) as u16;
// CHECK-NEXT:     let mut y: u16 = ((10 as i32) as i16) as u16;
// CHECK-NEXT:     if ((showbug(
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(x)) },
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(y)) },
// CHECK-NEXT:     ) as u32) as i32) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
