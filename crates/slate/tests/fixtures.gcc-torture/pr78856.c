extern void exit(int);

int a, b, c, d, e, f[3];

int main() {
  while (d)
    while (1)
      ;
  int g = 0, h, i = 0;
  for (; g < 21; g += 9) {
    int j = 1;
    for (h = 0; h < 3; h++)
      f[h] = 1;
    for (; j < 10; j++) {
      d = i && (b ? 0 : c);
      i = 1;
      if (g)
        a = e;
    }
  }
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut f: [i32; 3] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { d }) != (0 as i32) {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:             {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut g: i32 = 0 as i32;
// CHECK-NEXT:     let mut h: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: while g < (21 as i32) {
// CHECK-NEXT:         let mut j: i32 = 1 as i32;
// CHECK-NEXT:         h = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while h < (3 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(f) as *mut i32 }.offset(h as isize)
// CHECK-NEXT:                 } = 1 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = h;
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:             h = [[V_2]];
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: while j < (10 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 d = (i != (0 as i32)
// CHECK-NEXT:                     && (if (unsafe { b }) != (0 as i32) {
// CHECK-NEXT:                         0 as i32
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         unsafe { c }
// CHECK-NEXT:                     }) != (0 as i32)) as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:             i = 1 as i32;
// CHECK-NEXT:             if g != (0 as i32) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     a = unsafe { e };
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:             j = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = g;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (9 as i32);
// CHECK-NEXT:         g = [[V_6]];
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
