extern void abort(void);
extern void exit(int);

int c;

void baz(int *p) { c = *p; }

void bar(int b) {
  if (c != 1 || b != 2)
    abort();
}

void foo(int a, int b) {
  baz(&a);
  bar(b);
}

int main() {
  foo(1, 2);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn baz(mut p: &i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = unsafe { *(p as *const i32) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut b: i32) {
// CHECK-NEXT:     if (unsafe { c }) != (1 as i32) || b != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: i32, mut b: i32) {
// CHECK-NEXT:     baz(unsafe { &(*std::ptr::addr_of_mut!(a)) });
// CHECK-NEXT:     bar(b);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo(1 as i32, 2 as i32);
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
