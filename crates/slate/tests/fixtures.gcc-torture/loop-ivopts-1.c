/* From PR 18977.  */
void foo(float *x);

int main() {
  float x[4];
  foo(x);
  return 0;
}

void foo(float *x) {
  int          i, j, k;
  float        temp;
  static float t16[16] = {1., 2.,  3.,  4.,  5.,  6.,  7.,  8.,
                          9., 10., 11., 12., 13., 14., 15., 16.};
  static float tmp[4]  = {0., 0., 0., 0.};

  for (i = 0; i < 4; i++) {
    k    = 3 - i;
    temp = t16[5 * k];
    for (j = k + 1; j < 4; j++) {
      tmp[k] = t16[k + j * 4] * temp;
    }
  }
  x[0] = tmp[0];
  x[1] = tmp[1];
  x[2] = tmp[2];
  x[3] = tmp[3];
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: __SlateAlign16<[f32; 4]> = __SlateAlign16([0 as f32; 4]);
// CHECK-NEXT:     foo(x.0.as_mut_ptr() as *mut f32);
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// From PR 18977.
// CHECK-NEXT: fn foo(mut x: *mut f32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut k: i32 = 0 as i32;
// CHECK-NEXT:     let mut temp: f32 = 0 as f32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (4 as i32) {
// CHECK-NEXT:         k = (3 as i32) - i;
// CHECK-NEXT:         temp = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(t16.0) as *mut f32 }
// CHECK-NEXT:                     .offset(((5 as i32) * k) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         j = k + (1 as i32);
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while j < (4 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut f32 }.offset(k as isize)
// CHECK-NEXT:                 } = (unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { std::ptr::addr_of_mut!(t16.0) as *mut f32 }
// CHECK-NEXT:                             .offset((k + j * (4 as i32)) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }) * temp;
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:             j = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((0 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut f32 }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((1 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((2 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut f32 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x.offset((3 as i32) as isize) } = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(tmp.0) as *mut f32 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut t16: __SlateAlign16<[f32; 16]> = __SlateAlign16([
// CHECK-NEXT:     1.0f64 as f32,
// CHECK-NEXT:     2.0f64 as f32,
// CHECK-NEXT:     3.0f64 as f32,
// CHECK-NEXT:     4.0f64 as f32,
// CHECK-NEXT:     5.0f64 as f32,
// CHECK-NEXT:     6.0f64 as f32,
// CHECK-NEXT:     7.0f64 as f32,
// CHECK-NEXT:     8.0f64 as f32,
// CHECK-NEXT:     9.0f64 as f32,
// CHECK-NEXT:     10.0f64 as f32,
// CHECK-NEXT:     11.0f64 as f32,
// CHECK-NEXT:     12.0f64 as f32,
// CHECK-NEXT:     13.0f64 as f32,
// CHECK-NEXT:     14.0f64 as f32,
// CHECK-NEXT:     15.0f64 as f32,
// CHECK-NEXT:     16.0f64 as f32,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut tmp: __SlateAlign16<[f32; 4]> =
// CHECK-NEXT:     __SlateAlign16([0.0f64 as f32, 0.0f64 as f32, 0.0f64 as f32, 0.0f64 as f32]);
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
