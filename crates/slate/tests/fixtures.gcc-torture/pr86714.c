/* PR tree-optimization/86714 - tree-ssa-forwprop.c confused by too
   long initializer

   The excessively long initializer for a[0] is undefined but this
   test verifies that the excess elements are not considered a part
   of the value of the array as a matter of QoI.  */

const char a[2][3] = {"1234", "xyz"};
char       b[6];

void *pb = b;

int main() {
  __builtin_memcpy(b, a, 4);
  __builtin_memset(b + 4, 'a', 2);

  if (b[0] != '1' || b[1] != '2' || b[2] != '3' || b[3] != 'x' || b[4] != 'a' ||
      b[5] != 'a')
    __builtin_abort();

  if (__builtin_memcmp(pb, "123xaa", 6))
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/86714 - tree-ssa-forwprop.c confused by too
// CHECK-NEXT: //! long initializer
// CHECK-NEXT: //!
// CHECK-NEXT: //! The excessively long initializer for a[0] is undefined but this
// CHECK-NEXT: //! test verifies that the excess elements are not considered a part
// CHECK-NEXT: //! of the value of the array as a matter of QoI.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcpy"]
// CHECK-NEXT:     fn __builtin_memcpy([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "memset"]
// CHECK-NEXT:     fn __builtin_memset([[V_3:__v[0-9]+]]: *mut (), [[V_4:__v[0-9]+]]: i32, [[V_5:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_6:__v[0-9]+]]: *const (), [[V_7:__v[0-9]+]]: *const (), [[V_8:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: {{\[\[}}i8; 3]; 2] = [
// CHECK-NEXT:     [49 as i8, 50 as i8, 51 as i8],
// CHECK-NEXT:     [120 as i8, 121 as i8, 122 as i8],
// CHECK-NEXT: ];
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: [i8; 6] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut pb: *mut () = (unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }) as *mut ();
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memcpy(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }) as *mut (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of!(a) as *const [i8; 3] }) as *const (),
// CHECK-NEXT:             ((4 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __builtin_memset(
// CHECK-NEXT:             (unsafe { unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }.offset((4 as i32) as isize) })
// CHECK-NEXT:                 as *mut (),
// CHECK-NEXT:             97 as i32,
// CHECK-NEXT:             ((2 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) as i32)
// CHECK-NEXT:         != (49 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (50 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }.offset((2 as i32) as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (51 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }.offset((3 as i32) as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (120 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }.offset((4 as i32) as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (97 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { std::ptr::addr_of_mut!(b) as *mut i8 }.offset((5 as i32) as isize) }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (97 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { pb }) as *const (),
// CHECK-NEXT:             b"123xaa\0".as_ptr() as *const (),
// CHECK-NEXT:             ((6 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
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
