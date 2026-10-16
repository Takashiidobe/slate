void abort(void);
void exit(int);

unsigned char  x = 50;
volatile short y = -5;

int main() {
  x /= y;
  if (x != (unsigned char)-10)
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
// CHECK-NEXT: static mut x: u8 = ((50 as i32) as i8) as u8;
// CHECK-NEXT: static mut y: i16 = -(5 as i32) as i16;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u8 = unsafe { x };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u8 = (((([[V_1]] as u32) as i32)
// CHECK-NEXT:         / ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }) as i32)) as i8)
// CHECK-NEXT:         as u8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((unsafe { x }) as u32) as i32) != ((((-(10 as i32) as i8) as u8) as u32) as i32)
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
