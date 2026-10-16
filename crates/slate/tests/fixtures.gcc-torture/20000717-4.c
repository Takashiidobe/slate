/* Extracted from gas.  Incorrectly generated non-pic code at -O0 for
   IA-64, which produces linker errors on some operating systems.  */

struct {
  int offset;
  struct slot {
    int field[6];
  } slot[4];
} s;

int x() {
  int toggle = 0;
  int r      = s.slot[0].field[!toggle];
  return r;
}

int main() { return 0; }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Extracted from gas.  Incorrectly generated non-pic code at -O0 for
// CHECK-NEXT: //! IA-64, which produces linker errors on some operating systems.
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     offset: i32,
// CHECK-NEXT:     slot: [slot; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct slot {
// CHECK-NEXT:     field: [i32; 6],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn x() -> i32 {
// CHECK-NEXT:     let mut toggle: i32 = 0 as i32;
// CHECK-NEXT:     let mut r: i32 = unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { std::ptr::addr_of_mut!(s.slot) as *mut slot }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                 .field
// CHECK-NEXT:                 .as_mut_ptr() as *mut i32
// CHECK-NEXT:             }
// CHECK-NEXT:             .offset((!(toggle != (0 as i32)) as i32) as isize)
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
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
