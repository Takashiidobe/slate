/* { dg-do run }
 * { dg-options "-std=c23 -O2" }
 */

/* These tests check that redefinitions of tagged
   types can alias the original definitions.  */

struct foo {
  int x;
};

int test_foo(struct foo *a, void *b) {
  a->x = 1;

  struct foo {
    int x;
  } *p = b;
  p->x = 2;

  return a->x;
}

enum bar { A = 1, B = 3 };

int test_bar(enum bar *a, void *b) {
  *a = A;

  enum bar { A = 1, B = 3 } *p = b;
  *p                           = B;

  return *a;
}

int main() {
  struct foo y;

  if (2 != test_foo(&y, &y))
    __builtin_abort();

  enum bar z;

  if (B != test_bar(&z, &z))
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
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
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum bar {
// LOWERING-NEXT:     A = 1,
// LOWERING-NEXT:     B = 3,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: /// These tests check that redefinitions of tagged
// LOWERING-NEXT: /// types can alias the original definitions.
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct foo {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct foo_0 {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn abort() -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn test_foo({{arg[0-9]+}}: *mut foo, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{arg[0-9]+}}).x = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut foo_0 = {{arg[0-9]+}} as *mut foo_0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).x = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).x };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn test_bar({{arg[0-9]+}}: *mut u32, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = bar::A as u32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = {{arg[0-9]+}} as *mut u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = bar::B as u32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut y: foo = foo { x: 0 };
// LOWERING-NEXT:     let mut z: aligned::Aligned<aligned::A4, bar> = aligned::Aligned(bar::A);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(y) as *mut core::ffi::c_void;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 =
// LOWERING-NEXT:             unsafe { test_foo(std::ptr::addr_of_mut!(y), {{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = bar::B as i32;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void =
// LOWERING-NEXT:             (std::ptr::addr_of_mut!(*z) as *mut u32) as *mut core::ffi::c_void;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:             test_bar(
// LOWERING-NEXT:                 (std::ptr::addr_of_mut!(*z) as *mut u32) as *mut u32,
// LOWERING-NEXT:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
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
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum bar {
// REWRITES-NEXT:     A = 1,
// REWRITES-NEXT:     B = 3,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: /// These tests check that redefinitions of tagged
// REWRITES-NEXT: /// types can alias the original definitions.
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct foo {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct foo_0 {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn abort() -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn test_foo({{arg[0-9]+}}: *mut foo, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{arg[0-9]+}}).x = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut foo_0 = {{arg[0-9]+}} as *mut foo_0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{__v[0-9]+}}).x = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).x };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn test_bar({{arg[0-9]+}}: *mut u32, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = bar::A as u32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = {{arg[0-9]+}} as *mut u32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = bar::B as u32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { *{{arg[0-9]+}} };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut y: foo = foo { x: 0 };
// REWRITES-NEXT:     let mut z: aligned::Aligned<aligned::A4, bar> = aligned::Aligned(bar::A);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(y) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { test_foo(std::ptr::addr_of_mut!(y), {{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { std::process::abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = bar::B as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:         (std::ptr::addr_of_mut!(*z) as *mut u32) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         test_bar(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(*z) as *mut u32,
// REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { std::process::abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
