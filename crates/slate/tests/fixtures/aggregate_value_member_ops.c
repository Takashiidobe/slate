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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Nested {
// COMMON-LOWERING-NEXT:     inner: Pair,
// COMMON-LOWERING-NEXT:     tag: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Pair {
// COMMON-LOWERING-NEXT:     left: i32,
// COMMON-LOWERING-NEXT:     right: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct WithArray {
// COMMON-LOWERING-NEXT:     data: [i32; 3],
// COMMON-LOWERING-NEXT:     marker: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Pair = Pair { left: 2, right: 3 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Pair = replace_left({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Nested = Nested {
// COMMON-LOWERING-NEXT:         inner: Pair { left: 4, right: 5 },
// COMMON-LOWERING-NEXT:         tag: 6,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: WithArray = WithArray {
// COMMON-LOWERING-NEXT:         data: [8, 9, 10],
// COMMON-LOWERING-NEXT:         marker: 11,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = take_pair({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = take_pair({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = nested_total({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = array_value({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn replace_left({{arg[0-9]+}}: Pair, {{arg[0-9]+}}: i32) -> Pair {
// COMMON-LOWERING-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// COMMON-LOWERING-NEXT:     p = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     p.left = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Pair = p;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn take_pair({{arg[0-9]+}}: Pair) -> i32 {
// COMMON-LOWERING-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// COMMON-LOWERING-NEXT:     p = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.left;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.right;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn nested_total({{arg[0-9]+}}: Nested) -> i32 {
// COMMON-LOWERING-NEXT:     let mut n: Nested = Nested {
// COMMON-LOWERING-NEXT:         inner: Pair { left: 0, right: 0 },
// COMMON-LOWERING-NEXT:         tag: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     n = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = n.inner.left;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = n.inner.right;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = n.tag;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn array_value({{arg[0-9]+}}: WithArray) -> i32 {
// COMMON-LOWERING-NEXT:     let mut w: WithArray = WithArray {
// COMMON-LOWERING-NEXT:         data: [0; 3],
// COMMON-LOWERING-NEXT:         marker: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     w = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = w.data[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = w.marker;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Nested {
// COMMON-REWRITES-NEXT:     inner: Pair,
// COMMON-REWRITES-NEXT:     tag: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Pair {
// COMMON-REWRITES-NEXT:     left: i32,
// COMMON-REWRITES-NEXT:     right: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct WithArray {
// COMMON-REWRITES-NEXT:     data: [i32; 3],
// COMMON-REWRITES-NEXT:     marker: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Pair = Pair { left: 2, right: 3 };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Pair = replace_left({{__v[0-9]+}}, 7);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Nested = Nested {
// COMMON-REWRITES-NEXT:         inner: Pair { left: 4, right: 5 },
// COMMON-REWRITES-NEXT:         tag: 6,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: WithArray = WithArray {
// COMMON-REWRITES-NEXT:         data: [8, 9, 10],
// COMMON-REWRITES-NEXT:         marker: 11,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), take_pair({{__v[0-9]+}})) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), take_pair({{__v[0-9]+}})) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), nested_total({{__v[0-9]+}})) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), array_value({{__v[0-9]+}})) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn replace_left(mut p: Pair, {{arg[0-9]+}}: i32) -> Pair {
// COMMON-REWRITES-NEXT:     p.left = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     return p;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn take_pair(mut p: Pair) -> i32 {
// COMMON-REWRITES-NEXT:     p.left * 10 + p.right
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn nested_total(mut n: Nested) -> i32 {
// COMMON-REWRITES-NEXT:     n.inner.left + n.inner.right + n.tag
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn array_value(mut w: WithArray) -> i32 {
// COMMON-REWRITES-NEXT:     w.data[1] + w.marker
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
