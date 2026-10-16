int val;

int   *ptr  = &val;
float *ptr2 = (float *)&val;

__attribute__((optimize("-fno-strict-aliasing"))) void typepun(void) {
  *ptr2 = 0;
}

int main(void) {
  *ptr = 1;
  typepun();
  if (*ptr)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut val: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut ptr: *mut i32 = unsafe { std::ptr::addr_of_mut!(val) };
// CHECK-NEXT: static mut ptr2: *mut f32 = (unsafe { std::ptr::addr_of_mut!(val) }) as *mut f32;
// CHECK-NEXT: fn typepun() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ptr2 } = (0 as i32) as f32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ptr } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     typepun();
// CHECK-NEXT:     if (unsafe { *unsafe { ptr } }) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
