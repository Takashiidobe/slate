/* PR middle-end/51323 */

extern void abort(void);
struct S {
  int a, b, c;
};
int v;

__attribute__((noinline, noclone)) void foo(int x, int y, int z) {
  if (x != v || y != 0 || z != 9)
    abort();
}

static inline int baz(const struct S *p) { return p->b; }

__attribute__((noinline, noclone)) void bar(int x, struct S y) {
  foo(baz(&y), 0, x);
}

int main() {
  struct S s;
  v   = 3;
  s.a = v - 1;
  s.b = v;
  s.c = v + 1;
  bar(9, s);
  v   = 17;
  s.a = v - 1;
  s.b = v;
  s.c = v + 1;
  bar(9, s);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/51323
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
// CHECK-NEXT: static mut v: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32, mut z: i32) {
// CHECK-NEXT:     if x != unsafe { v } || y != (0 as i32) || z != (9 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut p: &S) -> i32 {
// CHECK-NEXT:     unsafe { (*(p as *const S)).b }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut x: i32, mut y: S) {
// CHECK-NEXT:     foo(
// CHECK-NEXT:         baz(unsafe { &(*(std::ptr::addr_of_mut!(y) as *const S)) }),
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         x,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: S = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     s.a = (unsafe { v }) - (1 as i32);
// CHECK-NEXT:     s.b = unsafe { v };
// CHECK-NEXT:     s.c = (unsafe { v }) + (1 as i32);
// CHECK-NEXT:     bar(9 as i32, s);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v = 17 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     s.a = (unsafe { v }) - (1 as i32);
// CHECK-NEXT:     s.b = unsafe { v };
// CHECK-NEXT:     s.c = (unsafe { v }) + (1 as i32);
// CHECK-NEXT:     bar(9 as i32, s);
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
