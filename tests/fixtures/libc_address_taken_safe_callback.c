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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Allocator {
// LOWERING-NEXT:     realloc_fn: Option<unsafe extern "C" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void>,
// LOWERING-NEXT:     free_fn: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut alloc: Allocator = Allocator { realloc_fn: unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void>>(realloc as *const ()) }, free_fn: unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(*mut core::ffi::c_void)>>(free as *const ()) } };
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn realloc(_0: *mut core::ffi::c_void, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut p: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void> = unsafe { alloc.realloc_fn };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     p = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void)> = unsafe { alloc.free_fn };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = p;
// LOWERING-NEXT:     unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: realloc_fn: Option<unsafe extern "C" fn(*mut core::ffi::c_void, u64) -> *mut core::ffi::c_void>
// REWRITES-DAG: free_fn: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: unsafe { unsafe { alloc.realloc_fn }.unwrap()(
// REWRITES-DAG: unsafe { unsafe { alloc.free_fn }.unwrap()(p) };
// REWRITES: {{^}}}
