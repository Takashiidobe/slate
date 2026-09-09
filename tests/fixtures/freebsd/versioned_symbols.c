#include <stdlib.h>

int old_compare(void *context, const void *lhs, const void *rhs) {
  return context != lhs && lhs != rhs;
}

int new_compare(const void *lhs, const void *rhs, void *context) {
  return context != lhs && lhs != rhs;
}

void sort_old(void *base, size_t count, size_t size, void *context) {
  qsort_r(base, count, size, context, old_compare);
}

void sort_new(void *base, size_t count, size_t size, void *context) {
  qsort_r(base, count, size, new_compare, context);
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-FREEBSD: #![allow(
// LOWERING-FREEBSD-NEXT:     dead_code,
// LOWERING-FREEBSD-NEXT:     unused,
// LOWERING-FREEBSD-NEXT:     non_camel_case_types,
// LOWERING-FREEBSD-NEXT:     non_snake_case,
// LOWERING-FREEBSD-NEXT:     non_upper_case_globals,
// LOWERING-FREEBSD-NEXT:     arithmetic_overflow,
// LOWERING-FREEBSD-NEXT:     unconditional_panic,
// LOWERING-FREEBSD-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-FREEBSD-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-FREEBSD-NEXT:     unused_comparisons
// LOWERING-FREEBSD-NEXT: )]
// LOWERING-FREEBSD-EMPTY:
// LOWERING-FREEBSD-NEXT: #[cfg(target_arch = "x86_64")]
// LOWERING-FREEBSD-NEXT: core::arch::global_asm!(
// LOWERING-FREEBSD-NEXT:     ".symver __qsort_r_compat, qsort_r@FBSD_1.0",
// LOWERING-FREEBSD-NEXT:     options(att_syntax, raw)
// LOWERING-FREEBSD-NEXT: );
// LOWERING-FREEBSD-EMPTY:
// LOWERING-FREEBSD-NEXT: unsafe extern "C" {
// LOWERING-FREEBSD-NEXT:     fn __qsort_r_compat(
// LOWERING-FREEBSD-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:         _1: usize,
// LOWERING-FREEBSD-NEXT:         _2: usize,
// LOWERING-FREEBSD-NEXT:         _3: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:         _4: Option<
// LOWERING-FREEBSD-NEXT:             unsafe extern "C-unwind" fn(
// LOWERING-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:             ) -> i32,
// LOWERING-FREEBSD-NEXT:         >,
// LOWERING-FREEBSD-NEXT:     );
// LOWERING-FREEBSD-NEXT:     fn qsort_r(
// LOWERING-FREEBSD-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:         _1: usize,
// LOWERING-FREEBSD-NEXT:         _2: usize,
// LOWERING-FREEBSD-NEXT:         _3: Option<
// LOWERING-FREEBSD-NEXT:             unsafe extern "C-unwind" fn(
// LOWERING-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:             ) -> i32,
// LOWERING-FREEBSD-NEXT:         >,
// LOWERING-FREEBSD-NEXT:         _4: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:     );
// LOWERING-FREEBSD-NEXT: }
// LOWERING-FREEBSD-EMPTY:
// LOWERING-FREEBSD-NEXT: extern "C-unwind" fn old_compare(
// LOWERING-FREEBSD-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT: ) -> i32 {
// LOWERING-FREEBSD-NEXT:     let mut lhs: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-FREEBSD-NEXT:     let mut rhs: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-FREEBSD-NEXT:     lhs = {{arg[0-9]+}};
// LOWERING-FREEBSD-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-FREEBSD-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = lhs;
// LOWERING-FREEBSD-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != {{__v[0-9]+}};
// LOWERING-FREEBSD-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-FREEBSD-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = lhs;
// LOWERING-FREEBSD-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = rhs;
// LOWERING-FREEBSD-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-FREEBSD-NEXT:         {{__v[0-9]+}}
// LOWERING-FREEBSD-NEXT:     } else {
// LOWERING-FREEBSD-NEXT:         let {{__v[0-9]+}}: bool = false;
// LOWERING-FREEBSD-NEXT:         {{__v[0-9]+}}
// LOWERING-FREEBSD-NEXT:     };
// LOWERING-FREEBSD-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-FREEBSD-NEXT:     return {{__v[0-9]+}};
// LOWERING-FREEBSD-NEXT: }
// LOWERING-FREEBSD-EMPTY:
// LOWERING-FREEBSD-NEXT: extern "C-unwind" fn new_compare(
// LOWERING-FREEBSD-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT: ) -> i32 {
// LOWERING-FREEBSD-NEXT:     let mut lhs: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-FREEBSD-NEXT:     let mut rhs: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-FREEBSD-NEXT:     lhs = {{arg[0-9]+}};
// LOWERING-FREEBSD-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-FREEBSD-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = lhs;
// LOWERING-FREEBSD-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != {{__v[0-9]+}};
// LOWERING-FREEBSD-NEXT:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// LOWERING-FREEBSD-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = lhs;
// LOWERING-FREEBSD-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = rhs;
// LOWERING-FREEBSD-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-FREEBSD-NEXT:         {{__v[0-9]+}}
// LOWERING-FREEBSD-NEXT:     } else {
// LOWERING-FREEBSD-NEXT:         let {{__v[0-9]+}}: bool = false;
// LOWERING-FREEBSD-NEXT:         {{__v[0-9]+}}
// LOWERING-FREEBSD-NEXT:     };
// LOWERING-FREEBSD-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-FREEBSD-NEXT:     return {{__v[0-9]+}};
// LOWERING-FREEBSD-NEXT: }
// LOWERING-FREEBSD-EMPTY:
// LOWERING-FREEBSD-NEXT: fn sort_old({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// LOWERING-FREEBSD-NEXT:     unsafe {
// LOWERING-FREEBSD-NEXT:         __qsort_r_compat(
// LOWERING-FREEBSD-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-FREEBSD-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-FREEBSD-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:             Some(old_compare),
// LOWERING-FREEBSD-NEXT:         )
// LOWERING-FREEBSD-NEXT:     };
// LOWERING-FREEBSD-NEXT:     return;
// LOWERING-FREEBSD-NEXT: }
// LOWERING-FREEBSD-EMPTY:
// LOWERING-FREEBSD-NEXT: fn sort_new({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// LOWERING-FREEBSD-NEXT:     unsafe {
// LOWERING-FREEBSD-NEXT:         qsort_r(
// LOWERING-FREEBSD-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-FREEBSD-NEXT:             {{arg[0-9]+}} as usize,
// LOWERING-FREEBSD-NEXT:             Some(new_compare),
// LOWERING-FREEBSD-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-FREEBSD-NEXT:         )
// LOWERING-FREEBSD-NEXT:     };
// LOWERING-FREEBSD-NEXT:     return;
// LOWERING-FREEBSD-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-FREEBSD: #![allow(
// REWRITES-FREEBSD-NEXT:     dead_code,
// REWRITES-FREEBSD-NEXT:     unused,
// REWRITES-FREEBSD-NEXT:     non_camel_case_types,
// REWRITES-FREEBSD-NEXT:     non_snake_case,
// REWRITES-FREEBSD-NEXT:     non_upper_case_globals,
// REWRITES-FREEBSD-NEXT:     arithmetic_overflow,
// REWRITES-FREEBSD-NEXT:     unconditional_panic,
// REWRITES-FREEBSD-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-FREEBSD-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-FREEBSD-NEXT:     unused_comparisons
// REWRITES-FREEBSD-NEXT: )]
// REWRITES-FREEBSD-EMPTY:
// REWRITES-FREEBSD-NEXT: #[cfg(target_arch = "x86_64")]
// REWRITES-FREEBSD-NEXT: core::arch::global_asm!(
// REWRITES-FREEBSD-NEXT:     ".symver __qsort_r_compat, qsort_r@FBSD_1.0",
// REWRITES-FREEBSD-NEXT:     options(att_syntax, raw)
// REWRITES-FREEBSD-NEXT: );
// REWRITES-FREEBSD-EMPTY:
// REWRITES-FREEBSD-NEXT: unsafe extern "C" {
// REWRITES-FREEBSD-NEXT:     fn __qsort_r_compat(
// REWRITES-FREEBSD-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:         _1: usize,
// REWRITES-FREEBSD-NEXT:         _2: usize,
// REWRITES-FREEBSD-NEXT:         _3: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:         _4: Option<
// REWRITES-FREEBSD-NEXT:             unsafe extern "C-unwind" fn(
// REWRITES-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:             ) -> i32,
// REWRITES-FREEBSD-NEXT:         >,
// REWRITES-FREEBSD-NEXT:     );
// REWRITES-FREEBSD-NEXT:     fn qsort_r(
// REWRITES-FREEBSD-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:         _1: usize,
// REWRITES-FREEBSD-NEXT:         _2: usize,
// REWRITES-FREEBSD-NEXT:         _3: Option<
// REWRITES-FREEBSD-NEXT:             unsafe extern "C-unwind" fn(
// REWRITES-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:                 *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:             ) -> i32,
// REWRITES-FREEBSD-NEXT:         >,
// REWRITES-FREEBSD-NEXT:         _4: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:     );
// REWRITES-FREEBSD-NEXT: }
// REWRITES-FREEBSD-EMPTY:
// REWRITES-FREEBSD-NEXT: extern "C-unwind" fn old_compare(
// REWRITES-FREEBSD-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:     mut lhs: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:     mut rhs: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT: ) -> i32 {
// REWRITES-FREEBSD-NEXT:     ({{arg[0-9]+}} != lhs && lhs != rhs) as i32
// REWRITES-FREEBSD-NEXT: }
// REWRITES-FREEBSD-EMPTY:
// REWRITES-FREEBSD-NEXT: extern "C-unwind" fn new_compare(
// REWRITES-FREEBSD-NEXT:     mut lhs: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:     mut rhs: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT: ) -> i32 {
// REWRITES-FREEBSD-NEXT:     ({{arg[0-9]+}} != lhs && lhs != rhs) as i32
// REWRITES-FREEBSD-NEXT: }
// REWRITES-FREEBSD-EMPTY:
// REWRITES-FREEBSD-NEXT: fn sort_old({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// REWRITES-FREEBSD-NEXT:     unsafe {
// REWRITES-FREEBSD-NEXT:         __qsort_r_compat(
// REWRITES-FREEBSD-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:             {{arg[0-9]+}} as usize,
// REWRITES-FREEBSD-NEXT:             {{arg[0-9]+}} as usize,
// REWRITES-FREEBSD-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:             Some(old_compare),
// REWRITES-FREEBSD-NEXT:         )
// REWRITES-FREEBSD-NEXT:     };
// REWRITES-FREEBSD-NEXT:     return;
// REWRITES-FREEBSD-NEXT: }
// REWRITES-FREEBSD-EMPTY:
// REWRITES-FREEBSD-NEXT: fn sort_new({{arg[0-9]+}}: *mut core::ffi::c_void, {{arg[0-9]+}}: u64, {{arg[0-9]+}}: u64, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// REWRITES-FREEBSD-NEXT:     unsafe {
// REWRITES-FREEBSD-NEXT:         qsort_r(
// REWRITES-FREEBSD-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:             {{arg[0-9]+}} as usize,
// REWRITES-FREEBSD-NEXT:             {{arg[0-9]+}} as usize,
// REWRITES-FREEBSD-NEXT:             Some(new_compare),
// REWRITES-FREEBSD-NEXT:             {{arg[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-FREEBSD-NEXT:         )
// REWRITES-FREEBSD-NEXT:     };
// REWRITES-FREEBSD-NEXT:     return;
// REWRITES-FREEBSD-NEXT: }
// SLATE-FILECHECK-END rewrites
