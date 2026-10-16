/* PR 15262.  Similar to pr15262-1.c but with no obvious addresses
   being taken in function foo().  Without IPA, by only looking inside
   foo() we cannot tell for certain whether 'q' and 'b' alias each
   other.  */

void abort(void);

struct A {
  int t;
  int i;
};

struct B {
  int  *p;
  float b;
};

float X;

int foo(struct B b, struct A *q, float *h) {
  X      += *h;
  *(b.p)  = 3;
  q->t    = 2;
  return *(b.p);
}

int main(void) {
  struct A a;
  struct B b;

  b.p = &a.t;
  if (foo(b, &a, &X) == 3)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR 15262.  Similar to pr15262-1.c but with no obvious addresses
// CHECK-NEXT: //! being taken in function foo().  Without IPA, by only looking inside
// CHECK-NEXT: //! foo() we cannot tell for certain whether 'q' and 'b' alias each
// CHECK-NEXT: //! other.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct A {
// CHECK-NEXT:     t: i32,
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     p: *mut i32,
// CHECK-NEXT:     b: f32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut X: f32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut b: B, mut q: &mut A, mut h: *mut f32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: f32 = unsafe { X };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: f32 = [[V_0]] + unsafe { *h };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         X = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *b.p = 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*(q as *mut A)).t = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *b.p }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: B = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     b.p = std::ptr::addr_of_mut!(a.t);
// CHECK-NEXT:     if foo(b, unsafe { &mut (*std::ptr::addr_of_mut!(a)) }, unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!(X)
// CHECK-NEXT:     }) == (3 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
