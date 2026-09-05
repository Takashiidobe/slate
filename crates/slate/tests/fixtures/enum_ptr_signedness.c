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
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum color_t {
// LOWERING-NEXT:     RED = 0,
// LOWERING-NEXT:     GREEN = 1,
// LOWERING-NEXT:     BLUE = 2,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct palette {
// LOWERING-NEXT:     start: *mut color_t,
// LOWERING-NEXT:     top: *mut color_t,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut data: [u32; 3] = [0; 3];
// LOWERING-NEXT:     let mut p: palette = palette {
// LOWERING-NEXT:         start: std::ptr::null_mut(),
// LOWERING-NEXT:         top: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u32; 3] = [0, 1, 2];
// LOWERING-NEXT:     data = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = data.as_mut_ptr() as *mut u32;
// LOWERING-NEXT:     p.start = {{__v[0-9]+}} as *mut color_t;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = data.as_mut_ptr() as *mut u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-NEXT:     p.top = {{__v[0-9]+}} as *mut color_t;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.start as *mut u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { *{{__v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.top as *mut u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.start as *mut u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum color_t {
// REWRITES-NEXT:     RED = 0,
// REWRITES-NEXT:     GREEN = 1,
// REWRITES-NEXT:     BLUE = 2,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct palette {
// REWRITES-NEXT:     start: *mut color_t,
// REWRITES-NEXT:     top: *mut color_t,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut data: [u32; 3] = [0; 3];
// REWRITES-NEXT:     let mut p: palette = palette {
// REWRITES-NEXT:         start: std::ptr::null_mut(),
// REWRITES-NEXT:         top: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     data = [0, 1, 2];
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = data.as_mut_ptr() as *mut u32;
// REWRITES-NEXT:     p.start = {{__v[0-9]+}} as *mut color_t;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = data.as_mut_ptr() as *mut u32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-NEXT:     p.top = {{__v[0-9]+}} as *mut color_t;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.start as *mut u32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), (unsafe { *{{__v[0-9]+}} }) as i32) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u32 = p.top as *mut u32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d\n".as_ptr(),
// REWRITES-NEXT:             (unsafe { {{__v[0-9]+}}.offset_from(p.start as *mut u32) as i64 }) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
