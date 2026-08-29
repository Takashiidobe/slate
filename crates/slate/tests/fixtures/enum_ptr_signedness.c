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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct palette {
// LOWERING-NEXT:     start: *mut color_t,
// LOWERING-NEXT:     top: *mut color_t,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut data: [u32; 3] = [0; 3];
// LOWERING-NEXT:     let mut p: palette = palette { start: std::ptr::null_mut(), top: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut c: aligned::Aligned<aligned::A4, color_t> = aligned::Aligned(color_t::RED);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     data = [0, 1, 2];
// LOWERING-NEXT:     let _v1: *mut u32 = data.as_mut_ptr() as *mut u32;
// LOWERING-NEXT:     p.start = _v1 as *mut color_t;
// LOWERING-NEXT:     let _v2: *mut u32 = data.as_mut_ptr() as *mut u32;
// LOWERING-NEXT:     let _v3: i32 = 3;
// LOWERING-NEXT:     let _v4: *mut u32 = unsafe { _v2.add(3) };
// LOWERING-NEXT:     p.top = _v4 as *mut color_t;
// LOWERING-NEXT:     let _v5: *mut u32 = p.start as *mut u32;
// LOWERING-NEXT:     let _v6: i32 = 1;
// LOWERING-NEXT:     let _v7: *mut u32 = unsafe { _v5.add(1) };
// LOWERING-NEXT:     let _v8: u32 = unsafe { *_v7 };
// LOWERING-NEXT:     *c = unsafe { std::mem::transmute(_v8) };
// LOWERING-NEXT:     let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: u32 = *c as u32;
// LOWERING-NEXT:     let _v11: i32 = _v10 as i32;
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v9 as *const i8, _v11) };
// LOWERING-NEXT:     let _v13: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v14: *mut u32 = p.top as *mut u32;
// LOWERING-NEXT:     let _v15: *mut u32 = p.start as *mut u32;
// LOWERING-NEXT:     let _v16: i64 = unsafe { _v14.offset_from(_v15) as i64 };
// LOWERING-NEXT:     let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:     let _v18: i32 = unsafe { printf(_v13 as *const i8, _v17) };
// LOWERING-NEXT:     let _v19: i32 = 0;
// LOWERING-NEXT:     __retval = _v19;
// LOWERING-NEXT:     let _v20: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v20 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct palette {
// REWRITES-NEXT:     start: *mut color_t,
// REWRITES-NEXT:     top: *mut color_t,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut data: [u32; 3] = [0; 3];
// REWRITES-NEXT: let mut p: palette = palette { start: std::ptr::null_mut(), top: std::ptr::null_mut() };
// REWRITES-NEXT: let mut c: aligned::Aligned<aligned::A4, color_t> = aligned::Aligned(color_t::RED);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: data = [0, 1, 2];
// REWRITES-NEXT: let _v1: *mut u32 = data.as_mut_ptr() as *mut u32;
// REWRITES-NEXT: p.start = _v1 as *mut color_t;
// REWRITES-NEXT: let _v2: *mut u32 = data.as_mut_ptr() as *mut u32;
// REWRITES-NEXT: let _v3: i32 = 3;
// REWRITES-NEXT: let _v4: *mut u32 = unsafe { _v2.add(3) };
// REWRITES-NEXT: p.top = _v4 as *mut color_t;
// REWRITES-NEXT: let _v5: *mut u32 = p.start as *mut u32;
// REWRITES-NEXT: let _v6: i32 = 1;
// REWRITES-NEXT: let _v7: *mut u32 = unsafe { _v5.add(1) };
// REWRITES-NEXT: *c = unsafe { std::mem::transmute(unsafe { *_v7 }) };
// REWRITES-NEXT: let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v9 as *const i8, (*c as u32) as i32) };
// REWRITES-NEXT: let _v13: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v14: *mut u32 = p.top as *mut u32;
// REWRITES-NEXT: let _v16: i64 = unsafe { _v14.offset_from(p.start as *mut u32) as i64 };
// REWRITES-NEXT: let _v18: i32 = unsafe { printf(_v13 as *const i8, _v16 as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
