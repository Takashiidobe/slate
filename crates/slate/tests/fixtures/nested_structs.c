#include <stdio.h>

struct Inner {
  int x;
  int y;
};

struct Outer {
  struct Inner a;
  int          z;
};

int main(void) {
  struct Outer o;
  o.a.x = 3;
  o.a.y = 4;
  o.z   = 5;
  printf("%d\n", o.a.x + o.a.y + o.z);

  struct Outer init = {{1, 2}, 3};
  printf("%d\n", init.a.x + init.a.y + init.z);
  return 0;
}

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
// COMMON-LOWERING-NEXT: struct Inner {
// COMMON-LOWERING-NEXT:     x: i32,
// COMMON-LOWERING-NEXT:     y: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Outer {
// COMMON-LOWERING-NEXT:     a: Inner,
// COMMON-LOWERING-NEXT:     z: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut o: Outer = Outer {
// COMMON-LOWERING-NEXT:         a: Inner { x: 0, y: 0 },
// COMMON-LOWERING-NEXT:         z: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let mut init: Outer = Outer {
// COMMON-LOWERING-NEXT:         a: Inner { x: 0, y: 0 },
// COMMON-LOWERING-NEXT:         z: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     o.a.x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     o.a.y = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     o.z = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = o.a.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = o.a.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = o.z;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Outer = Outer {
// COMMON-LOWERING-NEXT:         a: Inner { x: 1, y: 2 },
// COMMON-LOWERING-NEXT:         z: 3,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     init = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = init.a.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = init.a.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = init.z;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
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
// COMMON-REWRITES-NEXT: struct Inner {
// COMMON-REWRITES-NEXT:     x: i32,
// COMMON-REWRITES-NEXT:     y: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Outer {
// COMMON-REWRITES-NEXT:     a: Inner,
// COMMON-REWRITES-NEXT:     z: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut o: Outer = Outer {
// COMMON-REWRITES-NEXT:         a: Inner { x: 0, y: 0 },
// COMMON-REWRITES-NEXT:         z: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut init: Outer = Outer {
// COMMON-REWRITES-NEXT:         a: Inner { x: 0, y: 0 },
// COMMON-REWRITES-NEXT:         z: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     o.a.x = 3;
// COMMON-REWRITES-NEXT:     o.a.y = 4;
// COMMON-REWRITES-NEXT:     o.z = 5;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), o.a.x + o.a.y + o.z) };
// COMMON-REWRITES-NEXT:     init = Outer {
// COMMON-REWRITES-NEXT:         a: Inner { x: 1, y: 2 },
// COMMON-REWRITES-NEXT:         z: 3,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), init.a.x + init.a.y + init.z) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
