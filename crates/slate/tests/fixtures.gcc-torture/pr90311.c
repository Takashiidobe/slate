/* PR rtl-optimization/90311 */

int a, b;

int main() {
  unsigned long long x;
  unsigned int       c;
  __builtin_add_overflow((unsigned char)a, b, &c);
  b -= c < (unsigned char)a;
  x  = b;
  if (x)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/90311
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u64 = 0 as u64;
// CHECK-NEXT:     let mut c: u32 = 0 as u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = (((unsafe { a }) as i8) as u8) as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = (unsafe { b }) as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(c) = [[T_2]] as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_add([[T_1]]).is_none() || (([[T_2]] as u32) as i128) != [[T_2]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { b };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 =
// CHECK-NEXT:         [[V_0]] - ((c < ((((((unsafe { a }) as i8) as u8) as u32) as i32) as u32)) as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x = ((unsafe { b }) as i64) as u64;
// CHECK-NEXT:     if x != (0 as u64) {
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
