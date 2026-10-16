char         a, h;
int          b, d, e, g, j, k;
volatile int c;
short        i;

int main() {
  int m;

  m = i ^= 1;
  for (b = 0; b < 1; b++) {
    char o = m;
    g      = k;
    j      = j || c;
    if (a != o)
      for (; d < 1; d++)
        ;
    else {
      char *p = &h;
      *p      = 1;
      for (; e; e++)
        ;
    }
  }

  if (h != 0)
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
// CHECK-NEXT: static mut a: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut h: i8 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut j: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut k: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut i: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut m: i32 = 0 as i32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i16 = unsafe { i };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i16 = (([[V_0]] as i32) ^ (1 as i32)) as i16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     m = [[V_1]] as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { b }) < (1 as i32) {
// CHECK-NEXT:         let mut o: i8 = m as i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             g = unsafe { k };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             j = ((unsafe { j }) != (0 as i32)
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) })
// CHECK-NEXT:                     != (0 as i32)) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         if ((unsafe { a }) as i32) != (o as i32) {
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { d }) < (1 as i32) {
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 let mut [[V_2:__v[0-9]+]]: i32 = unsafe { d };
// CHECK-NEXT:                 let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     d = [[V_3]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             let mut p: *mut i8 = unsafe { std::ptr::addr_of_mut!(h) };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *p = (1 as i32) as i8;
// CHECK-NEXT:             }
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while (unsafe { e }) != (0 as i32) {
// CHECK-NEXT:                 {}
// CHECK-NEXT:                 let mut [[V_4:__v[0-9]+]]: i32 = unsafe { e };
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: i32 = [[V_4]] + (1 as i32);
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     e = [[V_5]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = unsafe { b };
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { h }) as i32) != (0 as i32) {
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
