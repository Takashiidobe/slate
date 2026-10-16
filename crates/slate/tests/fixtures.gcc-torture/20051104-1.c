/* PR rtl-optimization/23567 */

struct {
  int   len;
  char *name;
} s;

int main(void) {
  s.len  = 0;
  s.name = "";
  if (s.name[s.len] != 0)
    s.name[s.len] = 0;
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/23567
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     len: i32,
// CHECK-NEXT:     name: *mut i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.len = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.name = c"".as_ptr() as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *unsafe { unsafe { s.name }.offset((unsafe { s.len }) as isize) } }) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { unsafe { s.name }.offset((unsafe { s.len }) as isize) } = (0 as i32) as i8;
// CHECK-NEXT:         }
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
