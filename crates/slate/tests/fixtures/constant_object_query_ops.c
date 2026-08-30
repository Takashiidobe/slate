#include <stdio.h>

static int global_array[4];

int main(void) {
  int          local[6];
  int         *p = local;
  volatile int v = 3;

  int c_literal  = __builtin_constant_p(42);
  int c_expr     = __builtin_constant_p(7 + 5);
  int c_volatile = __builtin_constant_p(v);

  unsigned long local_whole     = __builtin_object_size(local, 0);
  unsigned long local_remaining = __builtin_object_size(&local[2], 1);
  unsigned long global_whole    = __builtin_object_size(global_array, 0);
  unsigned long unknown         = __builtin_object_size(p, 0);
  unsigned long unknown_upper   = __builtin_object_size(p, 2);

  printf("%d %d %d %lu %lu %lu %lu %lu\n", c_literal, c_expr, c_volatile,
         local_whole, local_remaining, global_whole, unknown, unknown_upper);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut local: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut v: i32 = 0;
// LOWERING-NEXT:     let mut c_literal: i32 = 0;
// LOWERING-NEXT:     let mut c_expr: i32 = 0;
// LOWERING-NEXT:     let mut c_volatile: i32 = 0;
// LOWERING-NEXT:     let mut local_whole: u64 = 0;
// LOWERING-NEXT:     let mut local_remaining: u64 = 0;
// LOWERING-NEXT:     let mut global_whole: u64 = 0;
// LOWERING-NEXT:     let mut unknown: u64 = 0;
// LOWERING-NEXT:     let mut unknown_upper: u64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = local.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     p = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(v), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     c_literal = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     c_expr = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     c_volatile = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 24;
// LOWERING-NEXT:     local_whole = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     local_remaining = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     global_whole = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = u64::MAX;
// LOWERING-NEXT:     unknown = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = p;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     unknown_upper = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %lu %lu %lu %lu %lu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c_literal;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c_expr;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = c_volatile;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = local_whole;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = local_remaining;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = global_whole;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unknown;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unknown_upper;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut local: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// REWRITES-NEXT: let mut p: *mut i32 = std::ptr::null_mut();
// REWRITES-NEXT: let mut v: i32 = 0;
// REWRITES-NEXT: let mut c_literal: i32 = 0;
// REWRITES-NEXT: let mut c_expr: i32 = 0;
// REWRITES-NEXT: let mut c_volatile: i32 = 0;
// REWRITES-NEXT: let mut local_whole: u64 = 0;
// REWRITES-NEXT: let mut local_remaining: u64 = 0;
// REWRITES-NEXT: let mut global_whole: u64 = 0;
// REWRITES-NEXT: let mut unknown: u64 = 0;
// REWRITES-NEXT: let mut unknown_upper: u64 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: p = local.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(v), {{_v[0-9]+}}) };
// REWRITES-NEXT: c_literal = 1;
// REWRITES-NEXT: c_expr = 1;
// REWRITES-NEXT: c_volatile = 0;
// REWRITES-NEXT: local_whole = 24;
// REWRITES-NEXT: local_remaining = 16;
// REWRITES-NEXT: global_whole = 16;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = p as *mut core::ffi::c_void;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = u64::MAX;
// REWRITES-NEXT: unknown = {{_v[0-9]+}};
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = p as *mut core::ffi::c_void;
// REWRITES-NEXT: unknown_upper = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %lu %lu %lu %lu %lu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, c_literal, c_expr, c_volatile, local_whole, local_remaining, global_whole, unknown, unknown_upper) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
