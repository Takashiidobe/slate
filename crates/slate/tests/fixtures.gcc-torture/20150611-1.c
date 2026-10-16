int   a, c, d;
short b;

int main() {
  int e[1];
  for (; b < 2; b++) {
    a = 0;
    if (b == 28378)
      a = e[b];
    if (!(d || b))
      for (; c;)
        ;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: [i32; 1] = [0 as i32; 1];
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while ((unsafe { b }) as i32) < (2 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         if ((unsafe { b }) as i32) == (28378 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 a = unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         (e.as_mut_ptr() as *mut i32)
// CHECK-NEXT:                             .offset(((unsafe { b }) as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if !((unsafe { d }) != (0 as i32) || (unsafe { b }) != (0 as i16)) {
// CHECK-NEXT:             '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while (unsafe { c }) != (0 as i32) {
// CHECK-NEXT:                 {}
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i16 = unsafe { b };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i16 = (([[V_0]] as i32) + (1 as i32)) as i16;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = [[V_1]];
// CHECK-NEXT:         }
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
