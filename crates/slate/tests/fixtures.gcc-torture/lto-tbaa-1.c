/* { dg-additional-options "-fno-early-inlining -fno-ipa-cp" }  */
struct a {
  float *b;
} *a;
struct b {
  int *b;
} b;
struct c {
  float *b;
}                              *c;
int                             d;
void                            use_a(struct a *a) {}
void                            set_b(int **a) { *a = &d; }
void                            use_c(struct c *a) {}
__attribute__((noinline)) int **retme(int **val) { return val; }
int                             e;
struct b                        b = {&e};
struct b                        b2;
struct b                        b3;
int                           **ptr = &b2.b;
int                             main(void) {
  a   = (void *)0;
  b.b = &e;
  ptr = retme(&b.b);
  set_b(ptr);
  b3 = b;
  if (b3.b != &d)
    __builtin_abort();
  c = (void *)0;
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// { dg-additional-options "-fno-early-inlining -fno-ipa-cp" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct a {
// CHECK-NEXT:     b: *mut f32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: *mut a = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct b {
// CHECK-NEXT:     b: *mut i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct c {
// CHECK-NEXT:     b: *mut f32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut c = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn use_a(mut [[A_0:a_[0-9]+]]: &a) {}
// CHECK-EMPTY:
// CHECK-NEXT: fn set_b(mut [[A_1:a_[0-9]+]]: *mut *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[A_1]] = unsafe { std::ptr::addr_of_mut!(d) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn use_c(mut [[A_2:a_[0-9]+]]: &c) {}
// CHECK-EMPTY:
// CHECK-NEXT: fn retme(mut val: *mut *mut i32) -> *mut *mut i32 {
// CHECK-NEXT:     val
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: b = b {
// CHECK-NEXT:     b: unsafe { std::ptr::addr_of_mut!(e) },
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b2: b = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b3: b = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut ptr: *mut *mut i32 = unsafe { std::ptr::addr_of_mut!(b2.b) };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = std::ptr::null_mut::<a>();
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b.b = unsafe { std::ptr::addr_of_mut!(e) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ptr = retme(unsafe { std::ptr::addr_of_mut!(b.b) });
// CHECK-NEXT:     }
// CHECK-NEXT:     set_b(unsafe { ptr });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b3 = unsafe { b };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { b3.b }) != unsafe { std::ptr::addr_of_mut!(d) } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = std::ptr::null_mut::<c>();
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
