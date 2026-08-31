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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct buffer {
// LOWERING-NEXT:     start: *mut u8,
// LOWERING-NEXT:     end: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut data: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut b: buffer = buffer { start: std::ptr::null_mut(), end: std::ptr::null_mut() };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     data = [65, 66, 67, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = data.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     b.start = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = data.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     b.end = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%c%c%c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = b.start;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = b.start;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = b.start;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(2) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = b.end;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = b.start;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct buffer {
// REWRITES-NEXT:     start: *mut u8,
// REWRITES-NEXT:     end: *mut u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut data: [u8; 4] = [65, 66, 67, 0];
// REWRITES-NEXT: let mut b: buffer = buffer { start: std::ptr::null_mut(), end: std::ptr::null_mut() };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = data.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: b.start = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = data.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(3) };
// REWRITES-NEXT: b.end = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%c%c%c\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = b.start;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = (unsafe { *{{_v[0-9]+}} }) as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = b.start;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = (unsafe { *{{_v[0-9]+}} }) as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = b.start;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(2) };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, (unsafe { *{{_v[0-9]+}} }) as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = b.end;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u8 = b.start;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = unsafe { {{_v[0-9]+}}.offset_from({{_v[0-9]+}}) as i64 };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
