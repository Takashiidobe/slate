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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(
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
// LOWERING-NEXT: struct hooks {
// LOWERING-NEXT:     malloc_fn: Option<unsafe extern "C" fn(u64) -> *mut core::ffi::c_void>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut h: hooks = hooks { malloc_fn: None };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     h.malloc_fn = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(u64) -> *mut core::ffi::c_void>>(
// LOWERING-NEXT:             malloc as *const (),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(u64) -> *mut core::ffi::c_void> = h.malloc_fn;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}}
// LOWERING-NEXT:         == unsafe {
// LOWERING-NEXT:             std::mem::transmute::<
// LOWERING-NEXT:                 *const (),
// LOWERING-NEXT:                 Option<unsafe extern "C" fn(u64) -> *mut core::ffi::c_void>,
// LOWERING-NEXT:             >(malloc as *const ())
// LOWERING-NEXT:         };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(
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
// REWRITES-NEXT: struct hooks {
// REWRITES-NEXT:     malloc_fn: Option<unsafe extern "C" fn(u64) -> *mut core::ffi::c_void>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut h: hooks = hooks { malloc_fn: None };
// REWRITES-NEXT:     h.malloc_fn = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C" fn(u64) -> *mut core::ffi::c_void>>(
// REWRITES-NEXT:             malloc as *const (),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = h.malloc_fn
// REWRITES-NEXT:         == unsafe {
// REWRITES-NEXT:             std::mem::transmute::<
// REWRITES-NEXT:                 *const (),
// REWRITES-NEXT:                 Option<unsafe extern "C" fn(u64) -> *mut core::ffi::c_void>,
// REWRITES-NEXT:             >(malloc as *const ())
// REWRITES-NEXT:         };
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = ({{_v[0-9]+}} as i32) != 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
