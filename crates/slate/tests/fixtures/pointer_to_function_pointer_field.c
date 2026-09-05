#include <stdio.h>
#include <stdlib.h>

typedef void (*test_fn)(void);

typedef struct {
  test_fn *tests;
  int      ntests;
} suite_t;

static int last_ran = -1;

static void test_a(void) { last_ran = 0; }

static void test_b(void) { last_ran = 1; }

int main(void) {
  suite_t suite;
  suite.tests    = malloc(2 * sizeof(test_fn));
  suite.tests[0] = test_a;
  suite.tests[1] = test_b;
  suite.ntests   = 2;

  for (int i = 0; i < suite.ntests; i++) {
    suite.tests[i]();
    printf("%d\n", last_ran);
  }

  free(suite.tests);
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
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct suite_t {
// COMMON-LOWERING-NEXT:     tests: *mut Option<unsafe extern "C-unwind" fn()>,
// COMMON-LOWERING-NEXT:     ntests: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut last_ran: i32 = -1;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut suite: suite_t = suite_t {
// COMMON-LOWERING-NEXT:         tests: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:         ntests: 0,
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> =
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}} as *mut Option<unsafe extern "C-unwind" fn()>;
// COMMON-LOWERING-NEXT:     suite.tests = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = suite.tests;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = Some(test_a);
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = suite.tests;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = Some(test_b);
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     suite.ntests = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = suite.ntests;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = suite.tests;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> =
// COMMON-LOWERING-NEXT:                     unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn()> = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                 unsafe { {{__v[0-9]+}}.unwrap()() };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { last_ran };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = suite.tests;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn test_a() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         last_ran = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn test_b() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         last_ran = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct suite_t {
// COMMON-REWRITES-NEXT:     tests: *mut Option<unsafe extern "C-unwind" fn()>,
// COMMON-REWRITES-NEXT:     ntests: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut last_ran: i32 = -1;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut suite: suite_t = suite_t {
// COMMON-REWRITES-NEXT:         tests: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:         ntests: 0,
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 2 * {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-REWRITES-NEXT:     suite.tests = {{__v[0-9]+}} as *mut Option<unsafe extern "C-unwind" fn()>;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = suite.tests;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = Some(test_a);
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = suite.tests;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = Some(test_b);
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     suite.ntests = 2;
// COMMON-REWRITES-NEXT:     for i in 0..suite.ntests {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> = suite.tests;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut Option<unsafe extern "C-unwind" fn()> =
// COMMON-REWRITES-NEXT:             unsafe { {{__v[0-9]+}}.offset((i as i64) as isize) };
// COMMON-REWRITES-NEXT:         unsafe { unsafe { *{{__v[0-9]+}} }.unwrap()() };
// COMMON-REWRITES-NEXT:         unsafe { printf(c"%d\n".as_ptr(), unsafe { last_ran }) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { free(suite.tests as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn test_a() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         last_ran = 0;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn test_b() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         last_ran = 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
