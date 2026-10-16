extern void abort(void);

struct A {
  int i;
};
struct B {
  struct A a;
  int      j;
};

static void foo(struct B *p) { ((struct A *)p)->i = 1; }

int main() {
  struct A a;
  a.i = 0;
  foo((struct B *)&a);
  if (a.i != 1)
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
// CHECK-NEXT: struct A {
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct B {
// CHECK-NEXT:     a: A,
// CHECK-NEXT:     j: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut p: &mut B) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*((p as *mut B) as *mut A)).i = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: A = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     a.i = 0 as i32;
// CHECK-NEXT:     foo(unsafe { &mut (*(std::ptr::addr_of_mut!(a) as *mut B)) });
// CHECK-NEXT:     if a.i != (1 as i32) {
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
