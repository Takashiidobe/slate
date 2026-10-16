int b, c, d, e = 1, f, g, h, j;

static int fn1() {
  int a = c;
  if (h)
    return 9;
  g = (c || b) % e;
  if ((g || f) && b)
    return 9;
  e = d;
  for (c = 0; c > -4; c--)
    ;
  if (d)
    c--;
  j = c;
  return d;
}

int main() {
  fn1();

  if (c != -4)
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
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut e: i32 = 1 as i32;
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut h: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut j: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn fn1() -> i32 {
// CHECK-NEXT:     if (unsafe { h }) != (0 as i32) {
// CHECK-NEXT:         return 9 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g = (((unsafe { c }) != (0 as i32) || (unsafe { b }) != (0 as i32)) as i32)
// CHECK-NEXT:             % unsafe { e };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { g }) != (0 as i32) || (unsafe { f }) != (0 as i32))
// CHECK-NEXT:         && (unsafe { b }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 9 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = unsafe { d };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { c }) > -(4 as i32) {
// CHECK-NEXT:         {}
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { d }) != (0 as i32) {
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { c };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] - (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         j = unsafe { c };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { d }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fn1();
// CHECK-NEXT:     if (unsafe { c }) != -(4 as i32) {
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
