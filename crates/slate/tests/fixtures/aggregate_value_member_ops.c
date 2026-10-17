#include <stdio.h>

struct Pair {
  int left;
  int right;
};

struct Nested {
  struct Pair inner;
  int         tag;
};

struct WithArray {
  int data[3];
  int marker;
};

static int take_pair(struct Pair p) { return p.left * 10 + p.right; }

static struct Pair replace_left(struct Pair p, int v) {
  p.left = v;
  return p;
}

static int nested_total(struct Nested n) {
  return n.inner.left + n.inner.right + n.tag;
}

static int array_value(struct WithArray w) { return w.data[1] + w.marker; }

int main(void) {
  struct Pair      p = {2, 3};
  struct Pair      q = replace_left(p, 7);
  struct Nested    n = {{4, 5}, 6};
  struct WithArray w = {{8, 9, 10}, 11};
  printf("%d\n", take_pair(p));
  printf("%d\n", take_pair(q));
  printf("%d\n", nested_total(n));
  printf("%d\n", array_value(w));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Nested {
// CHECK-NEXT:     inner: Pair,
// CHECK-NEXT:     tag: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct WithArray {
// CHECK-NEXT:     data: [i32; 3],
// CHECK-NEXT:     marker: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn take_pair(p: Pair) -> i32 {
// CHECK-NEXT:     p.left * 10 + p.right
// CHECK-NEXT: }
// CHECK-NEXT: fn replace_left(mut p: Pair, v: i32) -> Pair {
// CHECK-NEXT:     p.left = v;
// CHECK-NEXT:     return p;
// CHECK-NEXT: }
// CHECK-NEXT: fn nested_total(n: Nested) -> i32 {
// CHECK-NEXT:     n.inner.left + n.inner.right + n.tag
// CHECK-NEXT: }
// CHECK-NEXT: fn array_value(w: WithArray) -> i32 {
// CHECK-NEXT:     w.data[1] + w.marker
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: Pair = Pair { left: 2, right: 3 };
// CHECK-NEXT:     let q: Pair = replace_left(p, 7 as i32);
// CHECK-NEXT:     let n: Nested = Nested {
// CHECK-NEXT:         inner: Pair { left: 4, right: 5 },
// CHECK-NEXT:         tag: 6,
// CHECK-NEXT:     };
// CHECK-NEXT:     let w: WithArray = WithArray {
// CHECK-NEXT:         data: [8, 9, 10],
// CHECK-NEXT:         marker: 11,
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", take_pair(p));
// CHECK-NEXT:     println!("{}", take_pair(q));
// CHECK-NEXT:     println!("{}", nested_total(n));
// CHECK-NEXT:     println!("{}", array_value(w));
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
