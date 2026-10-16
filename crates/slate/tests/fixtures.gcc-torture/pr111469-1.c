/* PR tree-optimization/111469 */

long                           f;
char                          *g;
__attribute__((noinline)) char o() {
  char l;
  while (f)
    ;
  l = *g;
  return l;
}

/* factor_out_conditional_conversion is able to remove the casts
   from the 2 bbs (correctly)
   but then minmax_replacement should not optimize this to a MIN_EXPR
   as o has side effects. */

__attribute__((noinline)) unsigned short gg(unsigned short a,
                                            unsigned short b) {
  short d;
  if (a > b) {
    d = b;
  } else {
    o();
    d = a;
  }
  return d;
}

int main(void) { gg(3, 2); }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/111469
// CHECK-NEXT: static mut f: i64 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn o() -> i8 {
// CHECK-NEXT:     let mut l: i8 = 0 as i8;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { f }) != (0 as i64) {
// CHECK-NEXT:         {}
// CHECK-NEXT:     }
// CHECK-NEXT:     l = unsafe { *unsafe { g } };
// CHECK-NEXT:     l
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // factor_out_conditional_conversion is able to remove the casts
// CHECK-NEXT: // from the 2 bbs (correctly)
// CHECK-NEXT: // but then minmax_replacement should not optimize this to a MIN_EXPR
// CHECK-NEXT: // as o has side effects.
// CHECK-NEXT: fn gg(mut a: u16, mut b: u16) -> u16 {
// CHECK-NEXT:     let mut d: i16 = 0 as i16;
// CHECK-NEXT:     if ((a as u32) as i32) > ((b as u32) as i32) {
// CHECK-NEXT:         d = b as i16;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         o();
// CHECK-NEXT:         d = a as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     d as u16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     gg(((3 as i32) as i16) as u16, ((2 as i32) as i16) as u16);
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
