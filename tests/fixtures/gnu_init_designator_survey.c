#include <stdio.h>

union FlexUnion {
  int  value;
  char data[];
};

struct OnlyFlex {
  char data[];
};

union Castable {
  int   i;
  float f;
};

enum Forward;
enum Forward { FORWARD_A, FORWARD_B };

struct Point {
  int x;
  int y;
};

struct Sized {
  int n;
  int data[];
};

static struct Sized sized = {3, {10, 20, 30}};

struct NestedOuter {
  struct Sized inner;
};

int main(void) {
  int            range_values[10] = {[2 ... 5] = 9};
  int            old_index[3]     = {[1] 11};
  struct Point   p                = {x : 1, y : 2};
  int            five             = 5;
  union Castable c                = (union Castable)five;
  enum Forward   f                = FORWARD_B;

  printf("%d %d %d %d\n", range_values[3], old_index[1], p.x + p.y, c.i);
  printf("%d\n", (int)f);
  printf("%zu %zu\n", sizeof(union FlexUnion), sizeof(struct OnlyFlex));
  printf("%d %d %d\n", sized.data[0], sized.data[1], sized.data[2]);
  printf("%zu\n", sizeof(struct NestedOuter));
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
// COMMON-LOWERING-NEXT: union Castable {
// COMMON-LOWERING-NEXT:     i: i32,
// COMMON-LOWERING-NEXT:     f: f32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union FlexUnion {
// COMMON-LOWERING-NEXT:     value: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct NestedOuter {
// COMMON-LOWERING-NEXT:     inner: Sized,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct OnlyFlex {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Point {
// COMMON-LOWERING-NEXT:     x: i32,
// COMMON-LOWERING-NEXT:     y: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Sized {
// COMMON-LOWERING-NEXT:     n: i32,
// COMMON-LOWERING-NEXT:     data: [i32; 3],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut sized: Sized = Sized {
// COMMON-LOWERING-NEXT:     n: 3,
// COMMON-LOWERING-NEXT:     data: [10, 20, 30],
// COMMON-LOWERING-NEXT: };
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut old_index: [i32; 3] = [0; 3];
// COMMON-LOWERING-NEXT:     let mut p: Point = Point { x: 0, y: 0 };
// COMMON-LOWERING-NEXT:     let mut c: Castable = unsafe { std::mem::zeroed::<Castable>() };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 10] = [0, 0, 9, 9, 9, 9, 0, 0, 0, 0];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 3] = [0, 11, 0];
// COMMON-LOWERING-NEXT:     old_index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Point = Point { x: 1, y: 2 };
// COMMON-LOWERING-NEXT:     p = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(c) as *mut i32;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = Forward::FORWARD_B as u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = range_values[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = old_index[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = p.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c.i };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<FlexUnion>() as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<OnlyFlex>() as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         *std::ptr::addr_of_mut!(sized.data)
// COMMON-LOWERING-NEXT:             .cast::<i32>()
// COMMON-LOWERING-NEXT:             .add({{__v[0-9]+}} as usize)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         *std::ptr::addr_of_mut!(sized.data)
// COMMON-LOWERING-NEXT:             .cast::<i32>()
// COMMON-LOWERING-NEXT:             .add({{__v[0-9]+}} as usize)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         *std::ptr::addr_of_mut!(sized.data)
// COMMON-LOWERING-NEXT:             .cast::<i32>()
// COMMON-LOWERING-NEXT:             .add({{__v[0-9]+}} as usize)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     data: [i8; 0],
// LOWERING-X86_64-GNU-NEXT:     data: [i8; 0],
// LOWERING-X86_64-GNU-NEXT:     let mut range_values: aligned::Aligned<aligned::A16, [i32; 10]> = aligned::Aligned([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     *range_values = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     data: [u8; 0],
// LOWERING-AARCH64-GNU-NEXT:     data: [u8; 0],
// LOWERING-AARCH64-GNU-NEXT:     let mut range_values: [i32; 10] = [0; 10];
// LOWERING-AARCH64-GNU-NEXT:     range_values = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: union Castable {
// COMMON-REWRITES-NEXT:     i: i32,
// COMMON-REWRITES-NEXT:     f: f32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union FlexUnion {
// COMMON-REWRITES-NEXT:     value: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct NestedOuter {
// COMMON-REWRITES-NEXT:     inner: Sized,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct OnlyFlex {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Point {
// COMMON-REWRITES-NEXT:     x: i32,
// COMMON-REWRITES-NEXT:     y: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Sized {
// COMMON-REWRITES-NEXT:     n: i32,
// COMMON-REWRITES-NEXT:     data: [i32; 3],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut sized: Sized = Sized {
// COMMON-REWRITES-NEXT:     n: 3,
// COMMON-REWRITES-NEXT:     data: [10, 20, 30],
// COMMON-REWRITES-NEXT: };
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut c: Castable = unsafe { std::mem::zeroed::<Castable>() };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = std::ptr::addr_of_mut!(c) as *mut i32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u32 = Forward::FORWARD_B as u32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { c.i };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             range_values[3],
// COMMON-REWRITES-NEXT:             old_index[1],
// COMMON-REWRITES-NEXT:             p.x + p.y,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}} as i32) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%zu %zu\n".as_ptr(),
// COMMON-REWRITES-NEXT:             std::mem::size_of::<FlexUnion>() as u64,
// COMMON-REWRITES-NEXT:             std::mem::size_of::<OnlyFlex>() as u64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 *std::ptr::addr_of_mut!(sized.data)
// COMMON-REWRITES-NEXT:                     .cast::<i32>()
// COMMON-REWRITES-NEXT:                     .add((0 as i64) as usize)
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 *std::ptr::addr_of_mut!(sized.data)
// COMMON-REWRITES-NEXT:                     .cast::<i32>()
// COMMON-REWRITES-NEXT:                     .add((1 as i64) as usize)
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:             unsafe {
// COMMON-REWRITES-NEXT:                 *std::ptr::addr_of_mut!(sized.data)
// COMMON-REWRITES-NEXT:                     .cast::<i32>()
// COMMON-REWRITES-NEXT:                     .add((2 as i64) as usize)
// COMMON-REWRITES-NEXT:             },
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%zu\n".as_ptr(), 4 as u64) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     data: [i8; 0],
// REWRITES-X86_64-GNU-NEXT:     data: [i8; 0],
// REWRITES-X86_64-GNU-NEXT:     let mut range_values: aligned::Aligned<aligned::A16, [i32; 10]> = aligned::Aligned([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut old_index: [i32; 3] = [0; 3];
// REWRITES-X86_64-GNU-NEXT:     let mut p: Point = Point { x: 0, y: 0 };
// REWRITES-X86_64-GNU-NEXT:     *range_values = [0, 0, 9, 9, 9, 9, 0, 0, 0, 0];
// REWRITES-X86_64-GNU-NEXT:     old_index = [0, 11, 0];
// REWRITES-X86_64-GNU-NEXT:     p = Point { x: 1, y: 2 };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     data: [u8; 0],
// REWRITES-AARCH64-GNU-NEXT:     data: [u8; 0],
// REWRITES-AARCH64-GNU-NEXT:     let mut range_values: [i32; 10] = [0, 0, 9, 9, 9, 9, 0, 0, 0, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut old_index: [i32; 3] = [0, 11, 0];
// REWRITES-AARCH64-GNU-NEXT:     let mut p: Point = Point { x: 1, y: 2 };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
