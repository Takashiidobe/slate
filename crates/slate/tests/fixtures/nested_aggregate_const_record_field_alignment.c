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
// COMMON-LOWERING-NEXT:     scanners: [Option<unsafe extern "C-unwind" fn(i32) -> i32>; 3],
// COMMON-LOWERING-NEXT:     tag: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Outer {
// COMMON-LOWERING-NEXT:     inner: Inner,
// COMMON-LOWERING-NEXT:     bytes: [u8; 8],
// COMMON-LOWERING-NEXT:     trailing1: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// COMMON-LOWERING-NEXT:     trailing2: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// COMMON-LOWERING-NEXT:     trailing3: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut g: Outer = Outer {
// COMMON-LOWERING-NEXT:     inner: Inner {
// COMMON-LOWERING-NEXT:         scanners: [
// COMMON-LOWERING-NEXT:             unsafe {
// COMMON-LOWERING-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-LOWERING-NEXT:                     add1 as *const (),
// COMMON-LOWERING-NEXT:                 )
// COMMON-LOWERING-NEXT:             },
// COMMON-LOWERING-NEXT:             unsafe {
// COMMON-LOWERING-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-LOWERING-NEXT:                     add2 as *const (),
// COMMON-LOWERING-NEXT:                 )
// COMMON-LOWERING-NEXT:             },
// COMMON-LOWERING-NEXT:             unsafe {
// COMMON-LOWERING-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-LOWERING-NEXT:                     add3 as *const (),
// COMMON-LOWERING-NEXT:                 )
// COMMON-LOWERING-NEXT:             },
// COMMON-LOWERING-NEXT:         ],
// COMMON-LOWERING-NEXT:         tag: 42,
// COMMON-LOWERING-NEXT:     },
// COMMON-LOWERING-NEXT:     bytes: [1, 2, 3, 4, 5, 6, 7, 8],
// COMMON-LOWERING-NEXT:     trailing1: unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-LOWERING-NEXT:             mul5 as *const (),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     },
// COMMON-LOWERING-NEXT:     trailing2: unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-LOWERING-NEXT:             mul7 as *const (),
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     },
// COMMON-LOWERING-NEXT:     trailing3: 99,
// COMMON-LOWERING-NEXT: };
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn add1({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn add2({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn add3({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn mul5({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn mul7({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> =
// COMMON-LOWERING-NEXT:         unsafe { g.inner.scanners[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> =
// COMMON-LOWERING-NEXT:         unsafe { g.inner.scanners[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> =
// COMMON-LOWERING-NEXT:         unsafe { g.inner.scanners[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { g.inner.tag };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u8 = unsafe { g.bytes[({{__v[0-9]+}} as usize)] };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe { g.trailing1 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32) -> i32> = unsafe { g.trailing2 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { g.trailing3 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = b"%d \0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = b"%d \0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     scanners: [Option<unsafe extern "C-unwind" fn(i32) -> i32>; 3],
// COMMON-REWRITES-NEXT:     tag: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Outer {
// COMMON-REWRITES-NEXT:     inner: Inner,
// COMMON-REWRITES-NEXT:     bytes: [u8; 8],
// COMMON-REWRITES-NEXT:     trailing1: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// COMMON-REWRITES-NEXT:     trailing2: Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// COMMON-REWRITES-NEXT:     trailing3: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut g: Outer = Outer {
// COMMON-REWRITES-NEXT:     inner: Inner {
// COMMON-REWRITES-NEXT:         scanners: [
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-REWRITES-NEXT:                     add1 as *const (),
// COMMON-REWRITES-NEXT:                 )
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-REWRITES-NEXT:                     add2 as *const (),
// COMMON-REWRITES-NEXT:                 )
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-REWRITES-NEXT:                     add3 as *const (),
// COMMON-REWRITES-NEXT:                 )
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:         ],
// COMMON-REWRITES-NEXT:         tag: 42,
// COMMON-REWRITES-NEXT:     },
// COMMON-REWRITES-NEXT:     bytes: [1, 2, 3, 4, 5, 6, 7, 8],
// COMMON-REWRITES-NEXT:     trailing1: unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-REWRITES-NEXT:             mul5 as *const (),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     },
// COMMON-REWRITES-NEXT:     trailing2: unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(i32) -> i32>>(
// COMMON-REWRITES-NEXT:             mul7 as *const (),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     },
// COMMON-REWRITES-NEXT:     trailing3: 99,
// COMMON-REWRITES-NEXT: };
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn add1({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + 1
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn add2({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + 2
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn add3({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} + 3
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn mul5({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} * 5
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn mul7({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}} * 7
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { g.inner.tag };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe { unsafe { g.inner.scanners[0] }.unwrap()(10 as i32) },
// COMMON-REWRITES-NEXT:             unsafe { unsafe { g.inner.scanners[1] }.unwrap()(10 as i32) },
// COMMON-REWRITES-NEXT:             unsafe { unsafe { g.inner.scanners[2] }.unwrap()(10 as i32) },
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let mut i: i32 = 0;
// COMMON-REWRITES-NEXT:     while i < 8 {
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             printf(
// COMMON-REWRITES-NEXT:                 c"%d ".as_ptr(),
// COMMON-REWRITES-NEXT:                 (unsafe { g.bytes[((i as i64) as usize)] }) as i32,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         i += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"\n".as_ptr()) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { g.trailing3 };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe { unsafe { g.trailing1 }.unwrap()(10 as i32) },
// COMMON-REWRITES-NEXT:             unsafe { unsafe { g.trailing2 }.unwrap()(10 as i32) },
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
