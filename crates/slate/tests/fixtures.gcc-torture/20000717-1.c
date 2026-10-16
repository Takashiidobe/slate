void abort(void);
void exit(int);

typedef struct trio {
  int a, b, c;
} trio;

int bar(int i, trio t) {
  if (t.a == t.b || t.a == t.c)
    abort();
}

int foo(trio t, int i) { return bar(i, t); }

int main(void) {
  trio t = {1, 2, 3};

  foo(t, 4);
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
// CHECK-NEXT: struct trio {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn bar(mut i: i32, mut t: trio) -> i32 {
// CHECK-NEXT:     if t.a == t.b || t.a == t.c {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut t: trio, mut i: i32) -> i32 {
// CHECK-NEXT:     bar(i, t)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: trio = trio {
// CHECK-NEXT:         a: 1 as i32,
// CHECK-NEXT:         b: 2 as i32,
// CHECK-NEXT:         c: 3 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(t, 4 as i32);
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
