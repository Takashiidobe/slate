#include <stdlib.h>

typedef void *(*malloc_fn_t)(size_t);

struct hooks {
  malloc_fn_t malloc_fn;
};

int main(void) {
  struct hooks h;
  h.malloc_fn = malloc;
  int matches = (h.malloc_fn == malloc);
  return matches ? 0 : 1;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct hooks {
// CHECK-NEXT:     malloc_fn: Option<unsafe extern "C-unwind" fn(u64) -> *mut ()>,
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut h: hooks = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     h.malloc_fn = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             *const (),
// CHECK-NEXT:             Option<unsafe extern "C-unwind" fn(u64) -> *mut ()>,
// CHECK-NEXT:         >(malloc as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let matches: i32 = (h.malloc_fn
// CHECK-NEXT:         == unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 *const (),
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(u64) -> *mut ()>,
// CHECK-NEXT:             >(malloc as *const ())
// CHECK-NEXT:         }) as i32;
// CHECK-NEXT:     if matches != 0 { 0 } else { 1 }
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
