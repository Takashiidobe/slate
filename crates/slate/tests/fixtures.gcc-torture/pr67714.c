unsigned int b;
int          c;

signed char fn1() {
  signed char d;
  for (int i = 0; i < 1; i++)
    d = -15;
  return d;
}

int main(void) {
  for (c = 0; c < 1; c++)
    b = 0;
  char         e = fn1();
  signed char  f = e ^ b;
  volatile int g = (int)f;

  if (g != -15)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut b: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn fn1() -> i8 {
// CHECK-NEXT:     let mut d: i8 = 0 as i8;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (1 as i32) {
// CHECK-NEXT:         d = -(15 as i32) as i8;
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     d
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { c }) < (1 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = (0 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut e: i8 = fn1();
// CHECK-NEXT:     let mut f: i8 = ((((e as i32) as u32) ^ unsafe { b }) as u8) as i8;
// CHECK-NEXT:     let mut g: i32 = f as i32;
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(g)) }) != -(15 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
