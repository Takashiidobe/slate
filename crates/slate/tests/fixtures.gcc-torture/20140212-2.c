/* This used to fail as we would convert f into just return (unsigned
   int)usVlanID which is wrong. */

int f(unsigned short usVlanID) __attribute__((noinline, noclone));
int f(unsigned short usVlanID) {
  unsigned int uiVlanID = 0xffffffff;
  int          i;
  if ((unsigned short)0xffff != usVlanID)
    uiVlanID = (unsigned int)usVlanID;
  return uiVlanID;
}

int main(void) {
  if (f(1) != 1)
    __builtin_abort();
  if (f(0xffff) != -1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This used to fail as we would convert f into just return (unsigned
// CHECK-NEXT: //! int)usVlanID which is wrong.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut usVlanID: u16) -> i32 {
// CHECK-NEXT:     let mut uiVlanID: u32 = 4294967295 as u32;
// CHECK-NEXT:     if (((((65535 as i32) as i16) as u16) as u32) as i32) != ((usVlanID as u32) as i32) {
// CHECK-NEXT:         uiVlanID = usVlanID as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     uiVlanID as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(((1 as i32) as i16) as u16) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(((65535 as i32) as i16) as u16) != -(1 as i32) {
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
