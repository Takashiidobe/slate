int            a, b, *c = &a;
unsigned short d;

int main() {
  unsigned int e = a;
  *c             = 1;
  if (!b) {
    d  = e;
    *c = d | e;
  }

  if (a != 0)
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
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut c: *mut i32 = unsafe { std::ptr::addr_of_mut!(a) };
// CHECK-NEXT: static mut d: u16 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut e: u32 = (unsafe { a }) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { c } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { b }) != (0 as i32)) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             d = e as u16;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { c } = (((((unsafe { d }) as u32) as i32) as u32) | e) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { a }) != (0 as i32) {
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
