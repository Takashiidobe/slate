/* PR middle-end/117498 */

int          a, d, f;
char         g;
volatile int c = 1;

int foo() {
  if (c == 0)
    return -1;
  return 1;
}

void bar(int h, int i, char *k, char *m) {
  for (; d < i; d += 2)
    for (int j = 0; j < h; j++)
      m[j] = k[4 * j];
}

void baz(long h) {
  char n = 0;
  bar(h, 4, &n, &g);
}

int main() {
  f = foo();
  baz((unsigned char)f - 4);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/117498
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     if (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) }) == (0 as i32) {
// CHECK-NEXT:         return -(1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut h: i32, mut i: i32, mut k: *mut i8, mut m: *mut i8) {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { d }) < i {
// CHECK-NEXT:         let mut j: i32 = 0 as i32;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < h {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe { m.offset(j as isize) } =
// CHECK-NEXT:                     unsafe { *unsafe { k.offset(((4 as i32) * j) as isize) } };
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             j = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = unsafe { d };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (2 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = [[V_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut h: i64) {
// CHECK-NEXT:     let mut n: i8 = (0 as i32) as i8;
// CHECK-NEXT:     bar(h as i32, 4 as i32, std::ptr::addr_of_mut!(n), unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(g)
// CHECK-NEXT:     });
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = foo();
// CHECK-NEXT:     }
// CHECK-NEXT:     baz(((((((unsafe { f }) as i8) as u8) as u32) as i32) - (4 as i32)) as i64);
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
