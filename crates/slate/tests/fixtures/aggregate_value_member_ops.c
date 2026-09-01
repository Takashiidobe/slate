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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:     __slate_anon_0: u64,
// LOWERING-NEXT:     __slate_anon_1: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:     __slate_anon_0: u64,
// LOWERING-NEXT:     __slate_anon_1: u64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn replace_left({{arg[0-9]+}}: u64, {{arg[0-9]+}}: i32) -> u64 {
// LOWERING-NEXT:     let mut coerce: aligned::Aligned<aligned::A8, Pair> =
// LOWERING-NEXT:         aligned::Aligned(Pair { left: 0, right: 0 });
// LOWERING-NEXT:     let mut coerce2: u64 = 0;
// LOWERING-NEXT:     coerce2 = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Pair = std::ptr::addr_of_mut!(coerce2) as *mut Pair;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Pair = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// LOWERING-NEXT:     p = {{_v[0-9]+}};
// LOWERING-NEXT:     p.left = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Pair = p;
// LOWERING-NEXT:     *coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u64 = std::ptr::addr_of_mut!(*coerce) as *mut u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn take_pair({{arg[0-9]+}}: u64) -> i32 {
// LOWERING-NEXT:     let mut coerce: u64 = 0;
// LOWERING-NEXT:     coerce = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Pair = std::ptr::addr_of_mut!(coerce) as *mut Pair;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Pair = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut p: Pair = Pair { left: 0, right: 0 };
// LOWERING-NEXT:     p = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = p.left;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = p.right;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn nested_total({{arg[0-9]+}}: u64, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: 0,
// LOWERING-NEXT:         __slate_anon_1: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce2: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: 0,
// LOWERING-NEXT:         __slate_anon_1: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce2.__slate_anon_0 = {{arg[0-9]+}};
// LOWERING-NEXT:     coerce2.__slate_anon_1 = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = coerce2;
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Nested = std::ptr::addr_of_mut!(coerce) as *mut Nested;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Nested = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut n: Nested = Nested {
// LOWERING-NEXT:         inner: Pair { left: 0, right: 0 },
// LOWERING-NEXT:         tag: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     n = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = n.inner.left;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = n.inner.right;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = n.tag;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn array_value({{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64) -> i32 {
// LOWERING-NEXT:     let mut coerce: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: 0,
// LOWERING-NEXT:         __slate_anon_1: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce2: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: 0,
// LOWERING-NEXT:         __slate_anon_1: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     coerce2.__slate_anon_0 = {{arg[0-9]+}};
// LOWERING-NEXT:     coerce2.__slate_anon_1 = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_struct[0-9A-Za-z_]*}} = coerce2;
// LOWERING-NEXT:     coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut WithArray = std::ptr::addr_of_mut!(coerce) as *mut WithArray;
// LOWERING-NEXT:     let {{_v[0-9]+}}: WithArray = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let mut w: WithArray = WithArray {
// LOWERING-NEXT:         data: [0; 3],
// LOWERING-NEXT:         marker: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     w = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = w.data[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = w.marker;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut coerce: aligned::Aligned<aligned::A8, WithArray> = aligned::Aligned(WithArray {
// LOWERING-NEXT:         data: [0; 3],
// LOWERING-NEXT:         marker: 0,
// LOWERING-NEXT:     });
// LOWERING-NEXT:     let mut coerce2: {{anon_struct[0-9A-Za-z_]*}} = {{anon_struct[0-9A-Za-z_]*}} {
// LOWERING-NEXT:         __slate_anon_0: 0,
// LOWERING-NEXT:         __slate_anon_1: 0,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let mut coerce3: aligned::Aligned<aligned::A8, Pair> =
// LOWERING-NEXT:         aligned::Aligned(Pair { left: 0, right: 0 });
// LOWERING-NEXT:     let mut coerce4: aligned::Aligned<aligned::A8, Pair> =
// LOWERING-NEXT:         aligned::Aligned(Pair { left: 0, right: 0 });
// LOWERING-NEXT:     let mut coerce5: u64 = 0;
// LOWERING-NEXT:     let mut coerce6: aligned::Aligned<aligned::A8, Pair> =
// LOWERING-NEXT:         aligned::Aligned(Pair { left: 0, right: 0 });
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Pair = Pair { left: 2, right: 3 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     *coerce6 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u64 = std::ptr::addr_of_mut!(*coerce6) as *mut u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = replace_left({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     coerce5 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Pair = std::ptr::addr_of_mut!(coerce5) as *mut Pair;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Pair = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Nested = Nested {
// LOWERING-NEXT:         inner: Pair { left: 4, right: 5 },
// LOWERING-NEXT:         tag: 6,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: WithArray = WithArray {
// LOWERING-NEXT:         data: [8, 9, 10],
// LOWERING-NEXT:         marker: 11,
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     *coerce4 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u64 = std::ptr::addr_of_mut!(*coerce4) as *mut u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = take_pair({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     *coerce3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u64 = std::ptr::addr_of_mut!(*coerce3) as *mut u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = take_pair({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut Nested = std::ptr::addr_of_mut!(coerce2) as *mut Nested;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = coerce2.__slate_anon_0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = coerce2.__slate_anon_1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = nested_total({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     *coerce = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut {{anon_struct[0-9A-Za-z_]*}} =
// LOWERING-NEXT:         std::ptr::addr_of_mut!(*coerce) as *mut {{anon_struct[0-9A-Za-z_]*}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { (*{{_v[0-9]+}}).__slate_anon_0 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { (*{{_v[0-9]+}}).__slate_anon_1 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = array_value({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering
