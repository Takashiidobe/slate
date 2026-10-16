short *f(short *a, int b, int *d) __attribute__((noinline, noclone));

short *f(short *a, int b, int *d) {
  short c = *a;
  a++;
  c  = b << c;
  *d = c;
  return a;
}

int main(void) {
  int   d;
  short a[2];
  a[0] = 0;
  if (f(a, 1, &d) != &a[1])
    __builtin_abort();
  if (d != 1)
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
// CHECK-NEXT: fn f(mut a: *mut i16, mut b: i32, mut d: &mut i32) -> *mut i16 {
// CHECK-NEXT:     let mut c: i16 = unsafe { *a };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut i16 = a;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut i16 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:     a = [[V_1]];
// CHECK-NEXT:     c = (b << (c as i32)) as i16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(d as *mut i32) = c as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: i32 = 0 as i32;
// CHECK-NEXT:     let mut a: [i16; 2] = [0 as i16; 2];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (a.as_mut_ptr() as *mut i16).offset((0 as i32) as isize) } = (0 as i32)
// CHECK-NEXT:             as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     if f(
// CHECK-NEXT:         a.as_mut_ptr() as *mut i16,
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(d)) },
// CHECK-NEXT:     ) != unsafe { (a.as_mut_ptr() as *mut i16).offset((1 as i32) as isize) }
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if d != (1 as i32) {
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
