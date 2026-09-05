#include <stdio.h>

typedef enum color_e { RED, GREEN, BLUE } color_t;

struct palette {
  color_t *start;
  color_t *top;
};

int main(void) {
  color_t        data[3] = {RED, GREEN, BLUE};
  struct palette p;
  p.start   = data;
  p.top     = data + 3;
  color_t c = *(p.start + 1);
  printf("%d\n", (int)c);
  printf("%d\n", (int)(p.top - p.start));
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
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum color_t {
// COMMON-LOWERING-NEXT:     RED = 0,
// COMMON-LOWERING-NEXT:     GREEN = 1,
// COMMON-LOWERING-NEXT:     BLUE = 2,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct palette {
// COMMON-LOWERING-NEXT:     start: *mut color_t,
// COMMON-LOWERING-NEXT:     top: *mut color_t,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut data: [u32; 3] = [0; 3];
// COMMON-LOWERING-NEXT:     let mut p: palette = palette {
// COMMON-LOWERING-NEXT:         start: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         top: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [u32; 3] = [0, 1, 2];
// COMMON-LOWERING-NEXT:     data = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = data.as_mut_ptr() as *mut u32;
// COMMON-LOWERING-NEXT:     p.start = {{__v[0-9]+}} as *mut color_t;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = data.as_mut_ptr() as *mut u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(3) };
// COMMON-LOWERING-NEXT:     p.top = {{__v[0-9]+}} as *mut color_t;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.start as *mut u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.top as *mut u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.start as *mut u32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
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
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum color_t {
// COMMON-REWRITES-NEXT:     RED = 0,
// COMMON-REWRITES-NEXT:     GREEN = 1,
// COMMON-REWRITES-NEXT:     BLUE = 2,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct palette {
// COMMON-REWRITES-NEXT:     start: *mut color_t,
// COMMON-REWRITES-NEXT:     top: *mut color_t,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut data: [u32; 3] = [0; 3];
// COMMON-REWRITES-NEXT:     let mut p: palette = palette {
// COMMON-REWRITES-NEXT:         start: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         top: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     data = [0, 1, 2];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = data.as_mut_ptr() as *mut u32;
// COMMON-REWRITES-NEXT:     p.start = {{__v[0-9]+}} as *mut color_t;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = data.as_mut_ptr() as *mut u32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(3) };
// COMMON-REWRITES-NEXT:     p.top = {{__v[0-9]+}} as *mut color_t;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.start as *mut u32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), (unsafe { *{{__v[0-9]+}} }) as i32) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.top as *mut u32;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             (unsafe { {{__v[0-9]+}}.offset_from(p.start as *mut u32) as i64 }) as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
