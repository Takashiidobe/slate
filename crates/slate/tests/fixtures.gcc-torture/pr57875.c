/* PR rtl-optimization/57875 */

extern void abort(void);
int         a[1], b, c, d, f, i;
char        e[1];

int main() {
  for (; i < 1; i++)
    if (!d) {
      if (!c)
        f = 2;
      e[0] &= f ^= 0;
    }
  b = a[e[0] >> 1 & 1];
  if (b != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/57875
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: [i32; 1] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: [i8; 1] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { i }) < (1 as i32) {
// CHECK-NEXT:         if !((unsafe { d }) != (0 as i32)) {
// CHECK-NEXT:             if !((unsafe { c }) != (0 as i32)) {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     f = 2 as i32;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_0:__v[0-9]+]]: *mut i8 = unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(e) as *mut i8 }.offset((0 as i32) as isize)
// CHECK-NEXT:             };
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: i8 = unsafe { *[[V_0]] };
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: i32 = unsafe { f };
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] ^ (0 as i32);
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 f = [[V_3]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i8 = (([[V_1]] as i32) & [[V_3]]) as i8;
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *[[V_0]] = [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = unsafe { i };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             i = [[V_6]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(a) as *mut i32 }.offset(
// CHECK-NEXT:                     (((unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { std::ptr::addr_of_mut!(e) as *mut i8 }
// CHECK-NEXT:                                 .offset((0 as i32) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as i32)
// CHECK-NEXT:                         >> (1 as i32)
// CHECK-NEXT:                         & (1 as i32)) as isize,
// CHECK-NEXT:                 )
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { b }) != (0 as i32) {
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
