extern void abort(void);
extern void exit(int);

struct baz {
  int a, b, c;
};

void foo(int a, int b, int c) {
  if (a != 4)
    abort();
}

void bar(struct baz x, int b, int c) { foo(x.b, b, c); }

int main() {
  struct baz x = {3, 4, 5};
  bar(x, 1, 2);
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
// CHECK-NEXT: fn foo(mut a: i32, mut b: i32, mut c: i32) {
// CHECK-NEXT:     if a != (4 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut x: baz, mut b: i32, mut c: i32) {
// CHECK-NEXT:     foo(x.b, b, c);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: baz = baz {
// CHECK-NEXT:         a: 3 as i32,
// CHECK-NEXT:         b: 4 as i32,
// CHECK-NEXT:         c: 5 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     bar(x, 1 as i32, 2 as i32);
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
