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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![allow(
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
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct hooks {
// COMMON-LOWERING-NEXT:     malloc_fn: Option<unsafe extern "C-unwind" fn(u64) -> *mut core::ffi::c_void>,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut h: hooks = hooks { malloc_fn: None };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     h.malloc_fn = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<
// COMMON-LOWERING-NEXT:             *const (),
// COMMON-LOWERING-NEXT:             Option<unsafe extern "C-unwind" fn(u64) -> *mut core::ffi::c_void>,
// COMMON-LOWERING-NEXT:         >(malloc as *const ())
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(u64) -> *mut core::ffi::c_void> = h.malloc_fn;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:         == unsafe {
// COMMON-LOWERING-NEXT:             std::mem::transmute::<
// COMMON-LOWERING-NEXT:                 *const (),
// COMMON-LOWERING-NEXT:                 Option<unsafe extern "C-unwind" fn(u64) -> *mut core::ffi::c_void>,
// COMMON-LOWERING-NEXT:             >(malloc as *const ())
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![allow(
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
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct hooks {
// COMMON-REWRITES-NEXT:     malloc_fn: Option<unsafe extern "C-unwind" fn(u64) -> *mut core::ffi::c_void>,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut h: hooks = hooks { malloc_fn: None };
// COMMON-REWRITES-NEXT:     h.malloc_fn = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<
// COMMON-REWRITES-NEXT:             *const (),
// COMMON-REWRITES-NEXT:             Option<unsafe extern "C-unwind" fn(u64) -> *mut core::ffi::c_void>,
// COMMON-REWRITES-NEXT:         >(malloc as *const ())
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = h.malloc_fn
// COMMON-REWRITES-NEXT:         == unsafe {
// COMMON-REWRITES-NEXT:             std::mem::transmute::<
// COMMON-REWRITES-NEXT:                 *const (),
// COMMON-REWRITES-NEXT:                 Option<unsafe extern "C-unwind" fn(u64) -> *mut core::ffi::c_void>,
// COMMON-REWRITES-NEXT:             >(malloc as *const ())
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if ({{__v[0-9]+}} as i32) != 0 { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
