int *ptr1 = 0, **ptr2 = &ptr1;

int *identity(int *p) { return p; }

void store_to_c(int *p) { *ptr2 = identity(p); }

int main() {
  int f;
  store_to_c(&f);
  if (ptr1 != &f)
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
// CHECK-NEXT: static mut ptr1: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT: static mut ptr2: *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(ptr1) };
// CHECK-NEXT: fn identity(mut p: &i32) -> *mut i32 {
// CHECK-NEXT:     (p as *const i32) as *mut i32
// CHECK-NEXT: }
// CHECK-NEXT: fn store_to_c(mut p: &i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { ptr2 } = identity(p);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: i32 = 0 as i32;
// CHECK-NEXT:     store_to_c(unsafe { &(*std::ptr::addr_of_mut!(f)) });
// CHECK-NEXT:     if (unsafe { ptr1 }) != std::ptr::addr_of_mut!(f) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
