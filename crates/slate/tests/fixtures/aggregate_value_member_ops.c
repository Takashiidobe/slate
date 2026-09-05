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

// REWRITES-LABEL: {{^}}fn nested_total(
// REWRITES-DAG: n.inner.left + n.inner.right + n.tag
// REWRITES: {{^}}}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Nested {
// LOWERING-NEXT:     inner: Pair,
// LOWERING-NEXT:     tag: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Pair {
// LOWERING-NEXT:     left: i32,
// LOWERING-NEXT:     right: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct WithArray {
// LOWERING-NEXT:     data: [i32; 3],
// LOWERING-NEXT:     marker: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Pair = Pair { left: 2, right: 3 };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Pair = replace_left({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: Nested = Nested {
// LOWERING-NEXT:         inner: Pair { left: 4, right: 5 },
// LOWERING-NEXT:         tag: 6,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: WithArray = WithArray {
// LOWERING-NEXT:         data: [8, 9, 10],
// LOWERING-NEXT:         marker: 11,
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = take_pair({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = take_pair({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = nested_total({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = array_value({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn replace_left({{arg[0-9]+}}: Pair, {{arg[0-9]+}}: i32) -> Pair {
// LOWERING-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// LOWERING-NEXT:     p = {{arg[0-9]+}};
// LOWERING-NEXT:     p.left = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: Pair = p;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn take_pair({{arg[0-9]+}}: Pair) -> i32 {
// LOWERING-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// LOWERING-NEXT:     p = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.left;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.right;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn nested_total({{arg[0-9]+}}: Nested) -> i32 {
// LOWERING-NEXT:     let mut n: Nested = Nested {
// LOWERING-NEXT:         inner: Pair { left: 0, right: 0 },
// LOWERING-NEXT:         tag: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = n.inner.left;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = n.inner.right;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = n.tag;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn array_value({{arg[0-9]+}}: WithArray) -> i32 {
// LOWERING-NEXT:     let mut w: WithArray = WithArray {
// LOWERING-NEXT:         data: [0; 3],
// LOWERING-NEXT:         marker: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     w = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = w.data[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = w.marker;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Nested {
// REWRITES-NEXT:     inner: Pair,
// REWRITES-NEXT:     tag: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Pair {
// REWRITES-NEXT:     left: i32,
// REWRITES-NEXT:     right: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct WithArray {
// REWRITES-NEXT:     data: [i32; 3],
// REWRITES-NEXT:     marker: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{__v[0-9]+}}: Pair = Pair { left: 2, right: 3 };
// REWRITES-NEXT:     let {{__v[0-9]+}}: Pair = replace_left({{__v[0-9]+}}, 7);
// REWRITES-NEXT:     let {{__v[0-9]+}}: Nested = Nested {
// REWRITES-NEXT:         inner: Pair { left: 4, right: 5 },
// REWRITES-NEXT:         tag: 6,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: WithArray = WithArray {
// REWRITES-NEXT:         data: [8, 9, 10],
// REWRITES-NEXT:         marker: 11,
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), take_pair({{__v[0-9]+}})) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), take_pair({{__v[0-9]+}})) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), nested_total({{__v[0-9]+}})) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), array_value({{__v[0-9]+}})) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn replace_left(mut p: Pair, {{arg[0-9]+}}: i32) -> Pair {
// REWRITES-NEXT:     p.left = {{arg[0-9]+}};
// REWRITES-NEXT:     return p;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn take_pair(mut p: Pair) -> i32 {
// REWRITES-NEXT:     p.left * 10 + p.right
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn nested_total(mut n: Nested) -> i32 {
// REWRITES-NEXT:     n.inner.left + n.inner.right + n.tag
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn array_value(mut w: WithArray) -> i32 {
// REWRITES-NEXT:     w.data[1] + w.marker
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
