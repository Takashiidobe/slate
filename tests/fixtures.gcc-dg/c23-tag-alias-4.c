/* { dg-do run }
 * { dg-options "-std=c23 -O2" }
 */

/* Here we check that structs with flexible array
 * members can alias a compatible redefinition.  */

struct bar {
  int x;
  int f[];
};

int test_bar1(struct bar *a, void *b) {
  a->x = 1;

  struct bar {
    int x;
    int f[];
  }          *p = b;
  struct bar *q = a;
  p->x          = 2;

  return a->x;
}

int main() {
  struct bar z;

  if (2 != test_bar1(&z, &z))
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
// LOWERING-NEXT: /// Here we check that structs with flexible array
// LOWERING-NEXT: /// members can alias a compatible redefinition.
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct bar {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     f: [i32; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct bar_0 {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     f: [i32; 0],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn abort() -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn test_bar1({{arg[0-9]+}}: *mut bar, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{arg[0-9]+}}).x = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bar_0 = {{arg[0-9]+}} as *mut bar_0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bar_0 = {{arg[0-9]+}} as *mut bar_0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).x = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).x };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut z: bar = bar { x: 0, f: [0; 0] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(z) as *mut core::ffi::c_void;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 =
// LOWERING-NEXT:             unsafe { test_bar1(std::ptr::addr_of_mut!(z), {{__v[0-9]+}} as *mut core::ffi::c_void) };
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
// REWRITES-NEXT: /// Here we check that structs with flexible array
// REWRITES-NEXT: /// members can alias a compatible redefinition.
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct bar {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     f: [i32; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct bar_0 {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     f: [i32; 0],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn abort() -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn test_bar1({{arg[0-9]+}}: *mut bar, {{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{arg[0-9]+}}).x = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut bar_0 = {{arg[0-9]+}} as *mut bar_0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*{{__v[0-9]+}}).x = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*{{arg[0-9]+}}).x };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut z: bar = bar { x: 0, f: [0; 0] };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(z) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { test_bar1(std::ptr::addr_of_mut!(z), {{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { std::process::abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
