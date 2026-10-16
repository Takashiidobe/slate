struct Foo {
  int *p;
  int *q;
};

void __attribute__((noinline)) bar(int **x) {
  struct Foo *f = (struct Foo *)x;
  *(f->q)       = 0;
}

int foo(void) {
  struct Foo f;
  int        i = 1, j = 2;
  f.p = &i;
  f.q = &j;
  bar(&f.p);
  return j;
}

extern void abort(void);
int         main() {
  if (foo() != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Foo {
// CHECK-NEXT:     p: *mut i32,
// CHECK-NEXT:     q: *mut i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut x: &*mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { (*((x as *const *mut i32) as *mut Foo)).q } = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     let mut f: Foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = 1 as i32;
// CHECK-NEXT:     let mut j: i32 = 2 as i32;
// CHECK-NEXT:     f.p = std::ptr::addr_of_mut!(i);
// CHECK-NEXT:     f.q = std::ptr::addr_of_mut!(j);
// CHECK-NEXT:     bar(unsafe { &(*std::ptr::addr_of_mut!(f.p)) });
// CHECK-NEXT:     j
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo() != (0 as i32) {
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
