extern void    abort(void);
unsigned short c = 0x8000;
int            main() {
  if ((c - 0x8000) < 0 || (c - 0x8000) > 0x7fff)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut c: u16 = ((32768 as i32) as i16) as u16;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (((unsafe { c }) as u32) as i32) - (32768 as i32) < (0 as i32)
// CHECK-NEXT:         || (((unsafe { c }) as u32) as i32) - (32768 as i32) > (32767 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
