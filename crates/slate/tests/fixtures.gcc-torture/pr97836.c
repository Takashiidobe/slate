int a;

int b(int c) { return 0; }

static int *d(int *e) {
  if (a) {
    a = a && b(*e);
  }
  return e;
}

int main() {
  int f;
  if (d(&f) != &f)
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
// CHECK-NEXT: fn b(mut c: i32) -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn d(mut e: &i32) -> *mut i32 {
// CHECK-NEXT:     if (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:         if (unsafe { a }) != (0 as i32) {
// CHECK-NEXT:             [[V_0]] = b(unsafe { *(e as *const i32) }) != (0 as i32);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             [[V_0]] = false as bool;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = [[V_0]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     (e as *const i32) as *mut i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: i32 = 0 as i32;
// CHECK-NEXT:     if d(unsafe { &(*std::ptr::addr_of_mut!(f)) }) != std::ptr::addr_of_mut!(f) {
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
