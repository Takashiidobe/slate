int a;

int main(void) {
  char e[2] = {0, 0}, f = 0;
  if (a == 131072)
    f = e[a];
  return f;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: [i8; 2] = [(0 as i32) as i8, (0 as i32) as i8];
// CHECK-NEXT:     let mut f: i8 = (0 as i32) as i8;
// CHECK-NEXT:     if (unsafe { a }) == (131072 as i32) {
// CHECK-NEXT:         f = unsafe {
// CHECK-NEXT:             *unsafe { (e.as_mut_ptr() as *mut i8).offset((unsafe { a }) as isize) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     f as i32
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
