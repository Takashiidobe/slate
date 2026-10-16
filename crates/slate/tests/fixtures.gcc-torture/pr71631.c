/* PR tree-optimization/71631 */

volatile char v;
int           a = 1, b = 1, c = 1;

void foo(const char *s) {
  while (*s++)
    v = *s;
}

int main() {
  volatile int d = 1;
  volatile int e = 1;
  int          f = 1 / a;
  int          g = 1U < f;
  int          h = 2 + g;
  int          i = 3 % h;
  int          j = e && b;
  int          k = 1 == c;
  int          l = d != 0;
  short        m = (short)(-1 * i * l);
  short        x = j * (k * m);
  if (i == 1)
    foo("AB");
  if (x != -1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/71631
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut s: *const i8) {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *const i8 = s;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *const i8 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:         s = [[V_1]];
// CHECK-NEXT:         if !((unsafe { *[[V_0]] }) != (0 as i8)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(v), unsafe { *s }) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: i32 = 1 as i32;
// CHECK-NEXT:     let mut e: i32 = 1 as i32;
// CHECK-NEXT:     let mut f: i32 = (1 as i32) / unsafe { a };
// CHECK-NEXT:     let mut g: i32 = ((1 as u32) < (f as u32)) as i32;
// CHECK-NEXT:     let mut h: i32 = (2 as i32) + g;
// CHECK-NEXT:     let mut i: i32 = (3 as i32) % h;
// CHECK-NEXT:     let mut j: i32 = ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(e)) }) != (0 as i32)
// CHECK-NEXT:         && (unsafe { b }) != (0 as i32)) as i32;
// CHECK-NEXT:     let mut k: i32 = ((1 as i32) == unsafe { c }) as i32;
// CHECK-NEXT:     let mut l: i32 =
// CHECK-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) }) != (0 as i32)) as i32;
// CHECK-NEXT:     let mut m: i16 = (-(1 as i32) * i * l) as i16;
// CHECK-NEXT:     let mut x: i16 = (j * (k * (m as i32))) as i16;
// CHECK-NEXT:     if i == (1 as i32) {
// CHECK-NEXT:         foo(c"AB".as_ptr() as *const i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (x as i32) != -(1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
