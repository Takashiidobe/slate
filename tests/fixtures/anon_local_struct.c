#include <stdio.h>

typedef struct {
  int *start;
  int *end;
  int *pointer;
} buffer_t;

int main(void) {
  struct {
    int x;
    int y;
  } point = {3, 4};

  int      storage[4];
  buffer_t buf = {0, 0, 0};
  buf.start    = storage;
  buf.pointer  = storage;
  buf.end      = storage + 4;

  *buf.pointer = point.x + point.y;
  buf.pointer++;
  *buf.pointer = point.x * point.y;
  buf.pointer++;

  printf("%d %d\n", storage[0], storage[1]);
  printf("%ld\n", (long)(buf.pointer - buf.start));
  printf("%ld\n", (long)(buf.end - buf.start));
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
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct buffer_t {
// LOWERING-NEXT:     start: *mut i32,
// LOWERING-NEXT:     end: *mut i32,
// LOWERING-NEXT:     pointer: *mut i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut point: {{anon_[0-9]+}} = {{anon_[0-9]+}} { x: 0, y: 0 };
// LOWERING-NEXT:     let mut storage: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut buf: buffer_t = buffer_t {
// LOWERING-NEXT:         start: std::ptr::null_mut(),
// LOWERING-NEXT:         end: std::ptr::null_mut(),
// LOWERING-NEXT:         pointer: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: {{anon_[0-9]+}} = {{anon_[0-9]+}} { x: 3, y: 4 };
// LOWERING-NEXT:     point = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: buffer_t = buffer_t {
// LOWERING-NEXT:         start: std::ptr::null_mut(),
// LOWERING-NEXT:         end: std::ptr::null_mut(),
// LOWERING-NEXT:         pointer: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     buf = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     buf.start = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     buf.pointer = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(4) };
// LOWERING-NEXT:     buf.end = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = point.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = point.y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.pointer;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.pointer;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     buf.pointer = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = point.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = point.y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.pointer;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.pointer;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     buf.pointer = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = storage[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = storage[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.pointer;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.start;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.end;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.start;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: struct {{anon_[0-9]+}} {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct buffer_t {
// REWRITES-NEXT:     start: *mut i32,
// REWRITES-NEXT:     end: *mut i32,
// REWRITES-NEXT:     pointer: *mut i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut point: {{anon_[0-9]+}} = {{anon_[0-9]+}} { x: 0, y: 0 };
// REWRITES-NEXT:     let mut storage: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT:     let mut buf: buffer_t = buffer_t {
// REWRITES-NEXT:         start: std::ptr::null_mut(),
// REWRITES-NEXT:         end: std::ptr::null_mut(),
// REWRITES-NEXT:         pointer: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     point = {{anon_[0-9]+}} { x: 3, y: 4 };
// REWRITES-NEXT:     buf = buffer_t {
// REWRITES-NEXT:         start: std::ptr::null_mut(),
// REWRITES-NEXT:         end: std::ptr::null_mut(),
// REWRITES-NEXT:         pointer: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     buf.start = {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     buf.pointer = {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(4) };
// REWRITES-NEXT:     buf.end = {{_v[0-9]+}};
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *buf.pointer = point.x + point.y;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.pointer;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     buf.pointer = {{_v[0-9]+}};
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *buf.pointer = point.x * point.y;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.pointer;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     buf.pointer = {{_v[0-9]+}};
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), storage[0], storage[1]) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.pointer;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.start;
// REWRITES-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 }) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.end;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = buf.start;
// REWRITES-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 }) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
