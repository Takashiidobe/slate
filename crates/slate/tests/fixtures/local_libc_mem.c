#include <stdio.h>
#include <string.h>

int main(void) {
  char dst[16];
  char src[8] = "hello";
  memcpy(dst, src, 6);
  memset(dst + 5, 'A', 3);
  dst[8] = 0;
  char moved[16];
  memmove(moved, dst, 9);
  printf("%s\n", moved);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn memmove(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut dst: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut src: [i8; 8] = [0; 8];
// LOWERING-NEXT:     let mut moved: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     src = [104, 101, 108, 108, 111, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = dst.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = src.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memcpy({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = dst.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(5) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 65;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memset({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as i32, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 8;
// LOWERING-NEXT:     dst[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = moved.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = dst.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 9;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { memmove({{_v[0-9]+}} as *mut core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = moved.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn memcpy(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn memmove(_0: *mut core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut dst: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut src: [i8; 8] = [104, 101, 108, 108, 111, 0, 0, 0];
// REWRITES-NEXT: let mut moved: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = dst.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = src.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 6;
// REWRITES-NEXT: unsafe { std::ptr::copy_nonoverlapping((({{_v[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void) as *const u8, ({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut u8, ({{_v[0-9]+}} as usize) as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = dst.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { {{_v[0-9]+}}.add(5) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 65;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 3;
// REWRITES-NEXT: unsafe { std::ptr::write_bytes((({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut core::ffi::c_void) as *mut u8, ({{_v[0-9]+}} as i32) as u8, ({{_v[0-9]+}} as usize) as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 8;
// REWRITES-NEXT: dst[({{_v[0-9]+}} as usize)] = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = moved.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = dst.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 9;
// REWRITES-NEXT: unsafe { std::ptr::copy((({{_v[0-9]+}} as *mut core::ffi::c_void) as *const core::ffi::c_void) as *const u8, ({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut u8, ({{_v[0-9]+}} as usize) as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = moved.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
