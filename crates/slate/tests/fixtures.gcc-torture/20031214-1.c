/* PR optimization/10312 */
/* Originator: Peter van Hoof <p dot van-hoof at qub dot ac dot uk> */

/* Verify that the strength reduction pass doesn't find
   illegitimate givs.  */

struct {
  double a;
  int    n[2];
} g = {0., {1, 2}};

int k = 0;

void b(int *j) {}

int main() {
  int j;

  for (j = 0; j < 2; j++)
    k = (k > g.n[j]) ? k : g.n[j];

  k++;
  b(&j);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR optimization/10312
// CHECK-NEXT: //!
// CHECK-NEXT: //! Originator: Peter van Hoof <p dot van-hoof at qub dot ac dot uk>
// CHECK-NEXT: // Verify that the strength reduction pass doesn't find
// CHECK-NEXT: // illegitimate givs.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: f64,
// CHECK-NEXT:     n: [i32; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut g: [[SLATERECORD_0]] = [[SLATERECORD_0]] {
// CHECK-NEXT:     a: 0.0f64,
// CHECK-NEXT:     n: [1 as i32, 2 as i32],
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut k: i32 = 0 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn b(mut j: &i32) {}
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < (2 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             k = if (unsafe { k })
// CHECK-NEXT:                 > unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(g.n) as *mut i32 }.offset(j as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 } {
// CHECK-NEXT:                 unsafe { k }
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(g.n) as *mut i32 }.offset(j as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         j = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = unsafe { k };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         k = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     b(unsafe { &(*std::ptr::addr_of_mut!(j)) });
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
