void abort(void);
void exit(int);

struct foo {
  int a, b, c;
};

void brother(int a, int b, int c) {
  if (a)
    abort();
}

void sister(struct foo f, int b, int c) { brother((f.b == b), b, c); }

int main() {
  struct foo f = {7, 8, 9};
  sister(f, 1, 2);
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
// CHECK-NEXT: struct foo {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn brother(mut a: i32, mut b: i32, mut c: i32) {
// CHECK-NEXT:     if a != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn sister(mut f: foo, mut b: i32, mut c: i32) {
// CHECK-NEXT:     brother((f.b == b) as i32, b, c);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f: foo = foo {
// CHECK-NEXT:         a: 7 as i32,
// CHECK-NEXT:         b: 8 as i32,
// CHECK-NEXT:         c: 9 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     sister(f, 1 as i32, 2 as i32);
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
