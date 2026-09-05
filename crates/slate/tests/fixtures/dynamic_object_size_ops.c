#include <stdio.h>
#include <stdlib.h>

static int global_array[4];

static unsigned long runtime_alloc_size(int n) {
  void         *p    = malloc(n);
  unsigned long size = __builtin_dynamic_object_size(p, 0);
  free(p);
  return size;
}

int main(void) {
  int          local[6];
  int         *p = local;
  volatile int v = 3;
  (void)v;

  unsigned long local_whole     = __builtin_dynamic_object_size(local, 0);
  unsigned long local_remaining = __builtin_dynamic_object_size(&local[2], 1);
  unsigned long global_whole  = __builtin_dynamic_object_size(global_array, 0);
  unsigned long unknown       = __builtin_dynamic_object_size(p, 0);
  unsigned long unknown_upper = __builtin_dynamic_object_size(p, 2);
  unsigned long runtime_alloc = runtime_alloc_size(37);

  printf("%lu %lu %lu %lu %lu %lu\n", local_whole, local_remaining,
         global_whole, unknown, unknown_upper, runtime_alloc);
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
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut local: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// LOWERING-AARCH64-GNU-NEXT:     let mut local: [i32; 6] = [0; 6];
// LOWERING-NEXT:     let mut v: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = local.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(v), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 24;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = u64::MAX;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 37;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = runtime_alloc_size({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%lu %lu %lu %lu %lu %lu\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%lu %lu %lu %lu %lu %lu\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn runtime_alloc_size({{arg[0-9]+}}: i32) -> u64 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = u64::MAX;
// LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
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
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let mut local: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// REWRITES-AARCH64-GNU-NEXT:     let mut local: [i32; 6] = [0; 6];
// REWRITES-NEXT:     let mut v: i32 = 0;
// REWRITES-NEXT:     local.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(v), 3 as i32) };
// REWRITES-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = u64::MAX;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%lu %lu %lu %lu %lu %lu\n".as_ptr(),
// REWRITES-NEXT:             24 as u64,
// REWRITES-NEXT:             16 as u64,
// REWRITES-NEXT:             16 as u64,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             0 as u64,
// REWRITES-NEXT:             runtime_alloc_size(37),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn runtime_alloc_size({{arg[0-9]+}}: i32) -> u64 {
// REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = u64::MAX;
// REWRITES-NEXT:     unsafe { free((unsafe { malloc(({{arg[0-9]+}} as u64) as usize) }) as *mut core::ffi::c_void) };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
