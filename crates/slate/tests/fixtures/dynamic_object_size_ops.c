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
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut v: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = local.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(v), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 24;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = u64::MAX;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 37;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = runtime_alloc_size({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn runtime_alloc_size({{arg[0-9]+}}: i32) -> u64 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{arg[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = u64::MAX;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut local: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%lu %lu %lu %lu %lu %lu\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut local: [i32; 6] = [0; 6];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%lu %lu %lu %lu %lu %lu\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut v: i32 = 0;
// COMMON-REWRITES-NEXT:     local.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(v), 3 as i32) };
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v)) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = u64::MAX;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%lu %lu %lu %lu %lu %lu\n".as_ptr(),
// COMMON-REWRITES-NEXT:             24 as u64,
// COMMON-REWRITES-NEXT:             16 as u64,
// COMMON-REWRITES-NEXT:             16 as u64,
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             0 as u64,
// COMMON-REWRITES-NEXT:             runtime_alloc_size(37),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn runtime_alloc_size({{arg[0-9]+}}: i32) -> u64 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = u64::MAX;
// COMMON-REWRITES-NEXT:     unsafe { free((unsafe { malloc(({{arg[0-9]+}} as u64) as usize) }) as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut local: aligned::Aligned<aligned::A16, [i32; 6]> = aligned::Aligned([0; 6]);
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut local: [i32; 6] = [0; 6];
// SLATE-FILECHECK-END rewrites-aarch64-gnu
