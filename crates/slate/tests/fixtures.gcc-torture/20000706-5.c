extern void abort(void);
extern void exit(int);

struct baz {
  int a, b, c;
};

struct baz *c;

void bar(int b) {
  if (c->a != 1 || c->b != 2 || c->c != 3 || b != 4)
    abort();
}

void foo(struct baz a, int b) {
  c = &a;
  bar(b);
}

int main() {
  struct baz a;
  a.a = 1;
  a.b = 2;
  a.c = 3;
  foo(a, 4);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct baz {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut c: *mut baz = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn bar(mut b: i32) {
// CHECK-NEXT:     if (unsafe { (*unsafe { c }).a }) != (1 as i32)
// CHECK-NEXT:         || (unsafe { (*unsafe { c }).b }) != (2 as i32)
// CHECK-NEXT:         || (unsafe { (*unsafe { c }).c }) != (3 as i32) || b != (4 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut a: baz, mut b: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     }
// CHECK-NEXT:     bar(b);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: baz = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     a.a = 1 as i32;
// CHECK-NEXT:     a.b = 2 as i32;
// CHECK-NEXT:     a.c = 3 as i32;
// CHECK-NEXT:     foo(a, 4 as i32);
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
