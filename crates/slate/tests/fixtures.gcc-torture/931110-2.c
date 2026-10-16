void abort(void);
void exit(int);

int main(void) {
  static int    a[] = {3, 4};
  register int *b;
  int           c;

  b = a;
  c = *b++ % 8;
  if (c != 3)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut c: i32 = 0 as i32;
// CHECK-NEXT:     b = unsafe { std::ptr::addr_of_mut!(a) as *mut i32 };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i32 = b;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i32 = unsafe { [[V_1]].offset((1 as i32) as isize) };
// CHECK-NEXT:     b = [[V_2]];
// CHECK-NEXT:     c = (unsafe { *[[V_1]] }) % (8 as i32);
// CHECK-NEXT:     if c != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut a: [i32; 2] = [3 as i32, 4 as i32];
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
