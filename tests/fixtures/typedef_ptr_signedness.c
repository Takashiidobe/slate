#include <stdio.h>

typedef unsigned char my_char_t;

struct buffer {
  my_char_t *start;
  my_char_t *end;
};

int main(void) {
  my_char_t     data[4] = {65, 66, 67, 0};
  struct buffer b;
  b.start = data;
  b.end   = data + 3;
  printf("%c%c%c\n", b.start[0], b.start[1], b.start[2]);
  printf("%d\n", (int)(b.end - b.start));
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
// LOWERING-NEXT: struct buffer {
// LOWERING-NEXT:     start: *mut u8,
// LOWERING-NEXT:     end: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut data: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut b: buffer = buffer {
// LOWERING-NEXT:         start: std::ptr::null_mut(),
// LOWERING-NEXT:         end: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [65, 66, 67, 0];
// LOWERING-NEXT:     data = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = data.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     b.start = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = data.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// LOWERING-NEXT:     b.end = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%c%c%c\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%c%c%c\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.start;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.start;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.start;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.end;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.start;
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
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct buffer {
// REWRITES-NEXT:     start: *mut u8,
// REWRITES-NEXT:     end: *mut u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut data: [u8; 4] = [0; 4];
// REWRITES-NEXT:     let mut b: buffer = buffer {
// REWRITES-NEXT:         start: std::ptr::null_mut(),
// REWRITES-NEXT:         end: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     data = [65, 66, 67, 0];
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = data.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     b.start = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = data.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(3) };
// REWRITES-NEXT:     b.end = {{__v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%c%c%c\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%c%c%c\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.start;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(0) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.start;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(1) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *{{__v[0-9]+}} }) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.start;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(2) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             (unsafe { *{{__v[0-9]+}} }) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.end;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = b.start;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d\n".as_ptr(),
// REWRITES-NEXT:             (unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 }) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
