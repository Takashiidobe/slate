typedef __UINT8_TYPE__  u8;
typedef __UINT32_TYPE__ u32;

u32 a, b, d, e;
u8  c;

static u32 __attribute__((noinline, noclone)) foo(u32 p) {
  do {
    e /= 0xfff;
    if (p > c)
      d = 0;
    e -= 3;
    e *= b <= a;
  } while (e >= 88030);
  return e;
}

int main(void) {
  u32 x = foo(1164);
  if (x != 0xfd)
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
// CHECK-NEXT: static mut a: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut e: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: u8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut p: u32) -> u32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = unsafe { e };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]] / ((4095 as i32) as u32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if p > ((((unsafe { c }) as u32) as i32) as u32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 d = (0 as i32) as u32;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = unsafe { e };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = [[V_2]].wrapping_sub((3 as i32) as u32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u32 = unsafe { e };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: u32 = [[V_4]]
// CHECK-NEXT:             .wrapping_mul((((unsafe { b }) <= unsafe { a }) as i32) as u32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             e = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((unsafe { e }) >= ((88030 as i32) as u32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { e }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u32 = foo((1164 as i32) as u32);
// CHECK-NEXT:     if x != ((253 as i32) as u32) {
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
