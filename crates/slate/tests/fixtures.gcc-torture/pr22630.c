void abort(void);

int j;

void bla(int *r) {
  int *p, *q;

  p = q = r;
  if (!p)
    p = &j;

  if (p != q)
    j = 1;
}

int main(void) {
  bla(0);
  if (!j)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut j: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn bla(mut [[V_0:__v[0-9]+]]: *mut i32) {
// CHECK-NEXT:     let mut p: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut q: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     q = [[V_0]];
// CHECK-NEXT:     p = [[V_0]];
// CHECK-NEXT:     if !(p != std::ptr::null_mut::<i32>()) {
// CHECK-NEXT:         p = unsafe { std::ptr::addr_of_mut!(j) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if p != q {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             j = 1 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     bla(std::ptr::null_mut::<i32>());
// CHECK-NEXT:     if !((unsafe { j }) != (0 as i32)) {
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
