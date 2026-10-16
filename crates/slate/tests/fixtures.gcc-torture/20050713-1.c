/* Test that sibling call is not used if there is an argument overlap.  */

extern void abort(void);

struct S {
  int a, b, c;
};

int foo2(struct S x, struct S y) {
  if (x.a != 3 || x.b != 4 || x.c != 5)
    abort();
  if (y.a != 6 || y.b != 7 || y.c != 8)
    abort();
  return 0;
}

int foo3(struct S x, struct S y, struct S z) {
  foo2(x, y);
  if (z.a != 9 || z.b != 10 || z.c != 11)
    abort();
  return 0;
}

int bar2(struct S x, struct S y) { return foo2(y, x); }

int bar3(struct S x, struct S y, struct S z) { return foo3(y, x, z); }

int baz3(struct S x, struct S y, struct S z) { return foo3(y, z, x); }

int main(void) {
  struct S a = {3, 4, 5}, b = {6, 7, 8}, c = {9, 10, 11};

  bar2(b, a);
  bar3(b, a, c);
  baz3(c, a, b);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Test that sibling call is not used if there is an argument overlap.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo2(mut x: S, mut y: S) -> i32 {
// CHECK-NEXT:     if x.a != (3 as i32) || x.b != (4 as i32) || x.c != (5 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if y.a != (6 as i32) || y.b != (7 as i32) || y.c != (8 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo3(mut x: S, mut y: S, mut z: S) -> i32 {
// CHECK-NEXT:     foo2(x, y);
// CHECK-NEXT:     if z.a != (9 as i32) || z.b != (10 as i32) || z.c != (11 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar2(mut x: S, mut y: S) -> i32 {
// CHECK-NEXT:     foo2(y, x)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar3(mut x: S, mut y: S, mut z: S) -> i32 {
// CHECK-NEXT:     foo3(y, x, z)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz3(mut x: S, mut y: S, mut z: S) -> i32 {
// CHECK-NEXT:     foo3(y, z, x)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: S = S {
// CHECK-NEXT:         a: 3 as i32,
// CHECK-NEXT:         b: 4 as i32,
// CHECK-NEXT:         c: 5 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: S = S {
// CHECK-NEXT:         a: 6 as i32,
// CHECK-NEXT:         b: 7 as i32,
// CHECK-NEXT:         c: 8 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut c: S = S {
// CHECK-NEXT:         a: 9 as i32,
// CHECK-NEXT:         b: 10 as i32,
// CHECK-NEXT:         c: 11 as i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     bar2(b, a);
// CHECK-NEXT:     bar3(b, a, c);
// CHECK-NEXT:     baz3(c, a, b);
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
