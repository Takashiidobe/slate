#include <stdio.h>

typedef int (*fnptr)(int);

int add1(int x) { return x + 1; }
int add2(int x) { return x + 2; }
int add3(int x) { return x + 3; }
int mul5(int x) { return x * 5; }
int mul7(int x) { return x * 7; }

struct Inner {
  fnptr scanners[3];
  int   tag;
};

struct Outer {
  struct Inner  inner;
  unsigned char bytes[8];
  fnptr         trailing1;
  fnptr         trailing2;
  int           trailing3;
};

struct Outer g = {
    {{add1, add2, add3}, 42}, {1, 2, 3, 4, 5, 6, 7, 8}, mul5, mul7, 99,
};

int main(void) {
  printf("%d %d %d %d\n", g.inner.scanners[0](10), g.inner.scanners[1](10),
         g.inner.scanners[2](10), g.inner.tag);
  for (int i = 0; i < 8; i++) {
    printf("%d ", g.bytes[i]);
  }
  printf("\n");
  printf("%d %d %d\n", g.trailing1(10), g.trailing2(10), g.trailing3);
  return 0;
}

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
// LOWERING-NEXT: struct Inner {
// LOWERING-NEXT:     scanners: [Option<unsafe extern "C" fn(i32) -> i32>; 3],
// LOWERING-NEXT:     tag: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Outer {
// LOWERING-NEXT:     inner: Inner,
// LOWERING-NEXT:     bytes: [u8; 8],
// LOWERING-NEXT:     trailing1: Option<unsafe extern "C" fn(i32) -> i32>,
// LOWERING-NEXT:     trailing2: Option<unsafe extern "C" fn(i32) -> i32>,
// LOWERING-NEXT:     trailing3: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut g: Outer = Outer {
// LOWERING-NEXT:     inner: Inner {
// LOWERING-NEXT:         scanners: [
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// LOWERING-NEXT:                     add1 as *const (),
// LOWERING-NEXT:                 )
// LOWERING-NEXT:             },
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// LOWERING-NEXT:                     add2 as *const (),
// LOWERING-NEXT:                 )
// LOWERING-NEXT:             },
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// LOWERING-NEXT:                     add3 as *const (),
// LOWERING-NEXT:                 )
// LOWERING-NEXT:             },
// LOWERING-NEXT:         ],
// LOWERING-NEXT:         tag: 42,
// LOWERING-NEXT:     },
// LOWERING-NEXT:     bytes: [1, 2, 3, 4, 5, 6, 7, 8],
// LOWERING-NEXT:     trailing1: unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// LOWERING-NEXT:             mul5 as *const (),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     },
// LOWERING-NEXT:     trailing2: unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// LOWERING-NEXT:             mul7 as *const (),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     },
// LOWERING-NEXT:     trailing3: 99,
// LOWERING-NEXT: };
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add1({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add2({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add3({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn mul5({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn mul7({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { g.inner.scanners[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { g.inner.scanners[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> =
// LOWERING-NEXT:         unsafe { g.inner.scanners[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { g.inner.tag };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u8 = unsafe { g.bytes[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { g.trailing1 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32) -> i32> = unsafe { g.trailing2 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { g.trailing3 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: struct Inner {
// REWRITES-NEXT:     scanners: [Option<unsafe extern "C" fn(i32) -> i32>; 3],
// REWRITES-NEXT:     tag: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Outer {
// REWRITES-NEXT:     inner: Inner,
// REWRITES-NEXT:     bytes: [u8; 8],
// REWRITES-NEXT:     trailing1: Option<unsafe extern "C" fn(i32) -> i32>,
// REWRITES-NEXT:     trailing2: Option<unsafe extern "C" fn(i32) -> i32>,
// REWRITES-NEXT:     trailing3: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut g: Outer = Outer {
// REWRITES-NEXT:     inner: Inner {
// REWRITES-NEXT:         scanners: [
// REWRITES-NEXT:             unsafe {
// REWRITES-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// REWRITES-NEXT:                     add1 as *const (),
// REWRITES-NEXT:                 )
// REWRITES-NEXT:             },
// REWRITES-NEXT:             unsafe {
// REWRITES-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// REWRITES-NEXT:                     add2 as *const (),
// REWRITES-NEXT:                 )
// REWRITES-NEXT:             },
// REWRITES-NEXT:             unsafe {
// REWRITES-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// REWRITES-NEXT:                     add3 as *const (),
// REWRITES-NEXT:                 )
// REWRITES-NEXT:             },
// REWRITES-NEXT:         ],
// REWRITES-NEXT:         tag: 42,
// REWRITES-NEXT:     },
// REWRITES-NEXT:     bytes: [1, 2, 3, 4, 5, 6, 7, 8],
// REWRITES-NEXT:     trailing1: unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// REWRITES-NEXT:             mul5 as *const (),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     },
// REWRITES-NEXT:     trailing2: unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> i32>>(
// REWRITES-NEXT:             mul7 as *const (),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     },
// REWRITES-NEXT:     trailing3: 99,
// REWRITES-NEXT: };
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn add1({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     return {{arg[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn add2({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:     return {{arg[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn add3({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:     return {{arg[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn mul5({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT:     return {{arg[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn mul7({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// REWRITES-NEXT:     return {{arg[0-9]+}} * {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { g.inner.tag };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             unsafe { unsafe { g.inner.scanners[((0 as i64) as usize)] }.unwrap()(10 as i32) },
// REWRITES-NEXT:             unsafe { unsafe { g.inner.scanners[((1 as i64) as usize)] }.unwrap()(10 as i32) },
// REWRITES-NEXT:             unsafe { unsafe { g.inner.scanners[((2 as i64) as usize)] }.unwrap()(10 as i32) },
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let mut i: i32 = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 8;
// REWRITES-NEXT:         if !(i < {{_v[0-9]+}}) {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             printf(
// REWRITES-NEXT:                 c"%d ".as_ptr(),
// REWRITES-NEXT:                 (unsafe { g.bytes[((i as i64) as usize)] }) as i32,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:         i = i + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"\n".as_ptr()) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { g.trailing3 };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// REWRITES-NEXT:             unsafe { unsafe { g.trailing1 }.unwrap()(10 as i32) },
// REWRITES-NEXT:             unsafe { unsafe { g.trailing2 }.unwrap()(10 as i32) },
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
