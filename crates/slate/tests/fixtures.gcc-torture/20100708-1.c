/* PR middle-end/44843 */
/* Verify that we don't use the alignment of struct S for inner accesses.  */

struct S {
  double for_alignment;
  struct {
    int x, y, z;
  } a[16];
};

void f(struct S *s) __attribute__((noinline));

void f(struct S *s) {
  unsigned int i;

  for (i = 0; i < 16; ++i) {
    s->a[i].x = 0;
    s->a[i].y = 0;
    s->a[i].z = 0;
  }
}

int main(void) {
  struct S s;
  f(&s);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/44843
// CHECK-NEXT: //!
// CHECK-NEXT: //! Verify that we don't use the alignment of struct S for inner accesses.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     for_alignment: f64,
// CHECK-NEXT:     a: [[[SLATERECORD_0:__SlateRecord[0-9]+]]; 16],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT:     z: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut s: &mut S) {
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < ((16 as i32) as u32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*(s as *mut S)).a.as_mut_ptr() as *mut [[SLATERECORD_0]] }.offset(i as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .x = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*(s as *mut S)).a.as_mut_ptr() as *mut [[SLATERECORD_0]] }.offset(i as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .y = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { (*(s as *mut S)).a.as_mut_ptr() as *mut [[SLATERECORD_0]] }.offset(i as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .z = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     f(unsafe { &mut (*std::ptr::addr_of_mut!(s)) });
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
