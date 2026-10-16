/* PR middle-end/14470 */
/* Origin: Lodewijk Voge <lvoge@cs.vu.nl> */

extern void abort(void);

int main() {
  int t[1025] = {1024}, d;

  d = 0;
  d = t[d]++;
  if (t[0] != 1025)
    abort();
  if (d != 1024)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/14470
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Lodewijk Voge <lvoge@cs.vu.nl>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: __SlateAlign16<[i32; 1025]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [i32; 1025] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 1024 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut d: i32 = 0 as i32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut i32 = unsafe { (t.0.as_mut_ptr() as *mut i32).offset(d as isize) };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { *[[V_0]] };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     d = [[V_1]];
// CHECK-NEXT:     if (unsafe { *unsafe { (t.0.as_mut_ptr() as *mut i32).offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (1025 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if d != (1024 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
