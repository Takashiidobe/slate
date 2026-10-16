/* PR ipa/97404 */
/* { dg-additional-options "-fno-inline" } */

char         a, b;
long         c;
short        d, e;
long        *f = &c;
int          g;
char         h(signed char i) { return 0; }
static short j(short i, int k) { return i < 0 ? 0 : i >> k; }
void         l(void);
void         m(void) {
  e  = j(d | 9766, 11);
  *f = e;
}
void l(void) {
  a = 5 | g;
  b = h(a);
}
int main() {
  m();
  if (c != 4)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR ipa/97404
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-additional-options "-fno-inline" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: *mut i64 = unsafe { std::ptr::addr_of_mut!(c) };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn h(mut i: i8) -> i8 {
// CHECK-NEXT:     (0 as i32) as i8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn j(mut i: i16, mut k: i32) -> i16 {
// CHECK-NEXT:     (if (i as i32) < (0 as i32) {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         (i as i32) >> k
// CHECK-NEXT:     }) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn m() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = j((((unsafe { d }) as i32) | (9766 as i32)) as i16, 11 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { f } = (unsafe { e }) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn l() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = ((5 as i32) | unsafe { g }) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = h(unsafe { a });
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     m();
// CHECK-NEXT:     if (unsafe { c }) != ((4 as i32) as i64) {
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
