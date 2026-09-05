#include <stdio.h>
#include <stdlib.h>

typedef void *(*ReallocFn)(void *, size_t);
typedef void  (*FreeFn)(void *);

struct Allocator {
  ReallocFn realloc_fn;
  FreeFn    free_fn;
};

struct Allocator alloc = {realloc, free};

int main(void) {
  void *p = alloc.realloc_fn(NULL, 8);
  alloc.free_fn(p);
  printf("ok\n");
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
// LOWERING-NEXT: struct Allocator {
// LOWERING-NEXT:     realloc_fn:
// LOWERING-NEXT:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void>,
// LOWERING-NEXT:     free_fn: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut alloc: Allocator = Allocator {
// LOWERING-NEXT:     realloc_fn: unsafe {
// LOWERING-NEXT:         std::mem::transmute::<
// LOWERING-NEXT:             *const (),
// LOWERING-NEXT:             Option<
// LOWERING-NEXT:                 unsafe extern "C-unwind" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void,
// LOWERING-NEXT:             >,
// LOWERING-NEXT:         >(realloc as *const ())
// LOWERING-NEXT:     },
// LOWERING-NEXT:     free_fn: unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>>(
// LOWERING-NEXT:             free as *const (),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     },
// LOWERING-NEXT: };
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: Option<
// LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void,
// LOWERING-NEXT:     > = unsafe { alloc.realloc_fn };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)> =
// LOWERING-NEXT:         unsafe { alloc.free_fn };
// LOWERING-NEXT:     unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ok\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
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
// REWRITES-NEXT: struct Allocator {
// REWRITES-NEXT:     realloc_fn:
// REWRITES-NEXT:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void>,
// REWRITES-NEXT:     free_fn: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut alloc: Allocator = Allocator {
// REWRITES-NEXT:     realloc_fn: unsafe {
// REWRITES-NEXT:         std::mem::transmute::<
// REWRITES-NEXT:             *const (),
// REWRITES-NEXT:             Option<
// REWRITES-NEXT:                 unsafe extern "C-unwind" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void,
// REWRITES-NEXT:             >,
// REWRITES-NEXT:         >(realloc as *const ())
// REWRITES-NEXT:     },
// REWRITES-NEXT:     free_fn: unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void)>>(
// REWRITES-NEXT:             free as *const (),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     },
// REWRITES-NEXT: };
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void =
// REWRITES-NEXT:         unsafe { unsafe { alloc.realloc_fn }.unwrap()({{__v[0-9]+}}, 8 as u64) };
// REWRITES-NEXT:     unsafe { unsafe { alloc.free_fn }.unwrap()({{__v[0-9]+}}) };
// REWRITES-NEXT:     unsafe { printf(c"ok\n".as_ptr()) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
