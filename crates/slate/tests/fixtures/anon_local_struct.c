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
// COMMON-LOWERING-NEXT: struct {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT:     x: i32,
// COMMON-LOWERING-NEXT:     y: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct buffer_t {
// COMMON-LOWERING-NEXT:     start: *mut i32,
// COMMON-LOWERING-NEXT:     end: *mut i32,
// COMMON-LOWERING-NEXT:     pointer: *mut i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut point: {{anon_[0-9]+}} = {{anon_[0-9]+}} { x: 0, y: 0 };
// COMMON-LOWERING-NEXT:     let mut buf: buffer_t = buffer_t {
// COMMON-LOWERING-NEXT:         start: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         end: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         pointer: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: {{anon_[0-9]+}} = {{anon_[0-9]+}} { x: 3, y: 4 };
// COMMON-LOWERING-NEXT:     point = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: buffer_t = buffer_t {
// COMMON-LOWERING-NEXT:         start: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         end: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         pointer: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     buf = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     buf.start = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     buf.pointer = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(4) };
// COMMON-LOWERING-NEXT:     buf.end = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = point.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = point.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.pointer;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.pointer;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:     buf.pointer = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = point.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = point.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.pointer;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.pointer;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:     buf.pointer = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = storage[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = storage[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.pointer;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.start;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.end;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.start;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64 };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut storage: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%ld\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut storage: [i32; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%ld\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%ld\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: struct {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT:     x: i32,
// COMMON-REWRITES-NEXT:     y: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct buffer_t {
// COMMON-REWRITES-NEXT:     start: *mut i32,
// COMMON-REWRITES-NEXT:     end: *mut i32,
// COMMON-REWRITES-NEXT:     pointer: *mut i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut point: {{anon_[0-9]+}} = {{anon_[0-9]+}} { x: 0, y: 0 };
// COMMON-REWRITES-NEXT:     let mut buf: buffer_t = buffer_t {
// COMMON-REWRITES-NEXT:         start: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         end: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         pointer: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     point = {{anon_[0-9]+}} { x: 3, y: 4 };
// COMMON-REWRITES-NEXT:     buf = buffer_t {
// COMMON-REWRITES-NEXT:         start: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         end: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         pointer: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     buf.start = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     buf.pointer = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = storage.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(4) };
// COMMON-REWRITES-NEXT:     buf.end = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *buf.pointer = point.x + point.y;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.pointer;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     buf.pointer = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *buf.pointer = point.x * point.y;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.pointer;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     buf.pointer = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), storage[0], storage[1]) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.pointer;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.start;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(c"%ld\n".as_ptr(), unsafe {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.end;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = buf.start;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(c"%ld\n".as_ptr(), unsafe {
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}}.offset_from({{__v[0-9]+}}) as i64
// COMMON-REWRITES-NEXT:         })
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut storage: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut storage: [i32; 4] = [0; 4];
// SLATE-FILECHECK-END rewrites-aarch64-gnu
