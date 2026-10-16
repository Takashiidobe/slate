int main(int argc, char **argv) {
  int           c;
  unsigned char out[][1] = {{71}, {71}, {71}};

  for (int i = 0; i < 3; i++)
    if (!out[i][0])
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut out: {{\[\[}}u8; 1]; 3] = [
// CHECK-NEXT:         [((71 as i32) as i8) as u8],
// CHECK-NEXT:         [((71 as i32) as i8) as u8],
// CHECK-NEXT:         [((71 as i32) as i8) as u8],
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (3 as i32) {
// CHECK-NEXT:         if !((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe { (out.as_mut_ptr() as *mut [u8; 1]).offset(i as isize) })
// CHECK-NEXT:                         .as_mut_ptr() as *mut u8
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (0 as u8))
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
