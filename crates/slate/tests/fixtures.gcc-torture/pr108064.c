/* PR tree-optimization/108064 */

static inline short foo(short value) {
  return ((value >> 8) & 0xff) | ((value & 0xff) << 8);
}

__attribute__((noipa)) void bar(short *d, const short *s) {
  for (unsigned long i = 0; i < 4; i++)
    d[i] = foo(s[i]);
}

int main() {
  short a[4] __attribute__((aligned(16))) = {0xff, 0, 0, 0};
  short b[4] __attribute__((aligned(16)));
  short c[4] __attribute__((aligned(16)));

  bar(b, a);
  bar(c, b);
  if (a[0] != c[0])
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/108064
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut value: i16) -> i16 {
// CHECK-NEXT:     ((value as i32) >> (8 as i32) & (255 as i32) | ((value as i32) & (255 as i32)) << (8 as i32))
// CHECK-NEXT:         as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut d: *mut i16, mut s: *const i16) {
// CHECK-NEXT:     let mut i: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (((4 as i32) as i64) as u64) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { d.offset(i as isize) } = foo(unsafe { *unsafe { s.offset(i as isize) } });
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u64 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u64 = [[V_0]].wrapping_add(((1 as i32) as i64) as u64);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i16; 4]> = __SlateAlign16([
// CHECK-NEXT:         (255 as i32) as i16,
// CHECK-NEXT:         (0 as i32) as i16,
// CHECK-NEXT:         (0 as i32) as i16,
// CHECK-NEXT:         (0 as i32) as i16,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut b: __SlateAlign16<[i16; 4]> = __SlateAlign16([0 as i16; 4]);
// CHECK-NEXT:     let mut c: __SlateAlign16<[i16; 4]> = __SlateAlign16([0 as i16; 4]);
// CHECK-NEXT:     bar(b.0.as_mut_ptr() as *mut i16, a.0.as_mut_ptr() as *const i16);
// CHECK-NEXT:     bar(c.0.as_mut_ptr() as *mut i16, b.0.as_mut_ptr() as *const i16);
// CHECK-NEXT:     if ((unsafe { *unsafe { (a.0.as_mut_ptr() as *mut i16).offset((0 as i32) as isize) } }) as i32)
// CHECK-NEXT:         != ((unsafe { *unsafe { (c.0.as_mut_ptr() as *mut i16).offset((0 as i32) as isize) } })
// CHECK-NEXT:             as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
