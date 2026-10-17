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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn realloc(__ptr: *mut (), __size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Allocator {
// CHECK-NEXT:     realloc_fn: Option<unsafe extern "C-unwind" fn(*mut (), u64) -> *mut ()>,
// CHECK-NEXT:     free_fn: Option<unsafe extern "C-unwind" fn(*mut ())>,
// CHECK-NEXT: }
// CHECK-NEXT: static mut alloc: Allocator = Allocator {
// CHECK-NEXT:     realloc_fn: unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(*mut (), u64) -> *mut ()>,
// CHECK-NEXT:         >(realloc as *const ())
// CHECK-NEXT:     },
// CHECK-NEXT:     free_fn: unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(*mut ())>,
// CHECK-NEXT:         >(free as *const ())
// CHECK-NEXT:     },
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: *mut () = unsafe {
// CHECK-NEXT:         unsafe { alloc.realloc_fn }.unwrap()(std::ptr::null_mut::<()>(), 8 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { unsafe { alloc.free_fn }.unwrap()(p) };
// CHECK-NEXT:     println!("ok");
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
