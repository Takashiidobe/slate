int __attribute__((noinline)) foo(int *__restrict p, int i) {
  int *__restrict q;
  int *__restrict r;
  int v, w;
  q  = p + 1;
  r  = q - i;
  v  = *r;
  *p = 1;
  w  = *r;
  return v + w;
}
extern void abort(void);
int         main() {
  int i = 0;
  if (foo(&i, 1) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: *mut i32, mut i: i32) -> i32 {
// CHECK-NEXT:     let mut q: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut r: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut v: i32 = 0 as i32;
// CHECK-NEXT:     let mut w: i32 = 0 as i32;
// CHECK-NEXT:     q = unsafe { p.offset((1 as i32) as isize) };
// CHECK-NEXT:     r = unsafe { q.offset(-(i as isize)) };
// CHECK-NEXT:     v = unsafe { *r };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     w = unsafe { *r };
// CHECK-NEXT:     v + w
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     if foo(std::ptr::addr_of_mut!(i), 1 as i32) != (1 as i32) {
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
