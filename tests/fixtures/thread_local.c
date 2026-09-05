#include <stdio.h>
#include <threads.h>

static _Thread_local int file_value = 5;

static int update_values(int file_next, int block_next) {
  static _Thread_local int block_value = 7;
  int                      result      = file_value * 100 + block_value;
  file_value                           = file_next;
  block_value                          = block_next;
  return result;
}

static int worker(void *argument) {
  int *values = argument;
  return update_values(values[0], values[1]);
}

int main(void) {
  thrd_t thread;
  int    values[2]     = {11, 13};
  int    main_before   = update_values(17, 19);
  int    worker_result = 0;
  int    created       = thrd_create(&thread, worker, values);
  int joined = created == thrd_success ? thrd_join(thread, &worker_result) : -1;
  int main_after = update_values(23, 29);
  printf("%d %d %d %d %d\n", main_before, worker_result, main_after, created,
         joined);
  return 0;
}

// REWRITES-DAG: #![feature(thread_local)]
// REWRITES: #[thread_local]
// REWRITES-NEXT: static mut file_value: i32 = 5;

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(thread_local)]
// COMMON-LOWERING-NEXT: #![feature(c_variadic)]
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
// COMMON-LOWERING-NEXT: #[thread_local]
// COMMON-LOWERING-NEXT: static mut file_value: i32 = 5;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[thread_local]
// COMMON-LOWERING-NEXT: static mut update_values_block_value: i32 = 7;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn thrd_create(
// COMMON-LOWERING-NEXT:         _0: *mut u64,
// COMMON-LOWERING-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// COMMON-LOWERING-NEXT:         _2: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn thrd_join(_0: u64, _1: *mut i32) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut thread: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut values: [i32; 2] = [0; 2];
// COMMON-LOWERING-NEXT:     let mut worker_result: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 2] = [11, 13];
// COMMON-LOWERING-NEXT:     values = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 17;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 19;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = update_values({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     worker_result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         thrd_create(
// COMMON-LOWERING-NEXT:             std::ptr::addr_of_mut!(thread) as *mut u64,
// COMMON-LOWERING-NEXT:             Some(worker),
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = thread;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:             thrd_join(
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} as u64,
// COMMON-LOWERING-NEXT:                 std::ptr::addr_of_mut!(worker_result) as *mut i32,
// COMMON-LOWERING-NEXT:             )
// COMMON-LOWERING-NEXT:         };
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:         {{__v[0-9]+}}
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 23;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 29;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = update_values({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = worker_result;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
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
// COMMON-LOWERING-NEXT: fn update_values({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { file_value };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { update_values_block_value };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         file_value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         update_values_block_value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = update_values({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(thread_local)]
// COMMON-REWRITES-NEXT: #![feature(c_variadic)]
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
// COMMON-REWRITES-NEXT: #[thread_local]
// COMMON-REWRITES-NEXT: static mut file_value: i32 = 5;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[thread_local]
// COMMON-REWRITES-NEXT: static mut update_values_block_value: i32 = 7;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn thrd_create(
// COMMON-REWRITES-NEXT:         _0: *mut u64,
// COMMON-REWRITES-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// COMMON-REWRITES-NEXT:         _2: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn thrd_join(_0: u64, _1: *mut i32) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut thread: u64 = 0;
// COMMON-REWRITES-NEXT:     let mut values: [i32; 2] = [0; 2];
// COMMON-REWRITES-NEXT:     let mut worker_result: i32 = 0;
// COMMON-REWRITES-NEXT:     values = [11, 13];
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = update_values(17, 19);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         thrd_create(
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(thread) as *mut u64,
// COMMON-REWRITES-NEXT:             Some(worker),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:             thrd_join(
// COMMON-REWRITES-NEXT:                 thread as u64,
// COMMON-REWRITES-NEXT:                 std::ptr::addr_of_mut!(worker_result) as *mut i32,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = -1;
// COMMON-REWRITES-NEXT:         {{__v[0-9]+}}
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             worker_result,
// COMMON-REWRITES-NEXT:             update_values(23, 29),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn update_values({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { file_value }) * 100 + unsafe { update_values_block_value };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         file_value = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         update_values_block_value = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     {{__v[0-9]+}}
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(0) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.add(1) };
// COMMON-REWRITES-NEXT:     update_values({{__v[0-9]+}}, unsafe { *{{__v[0-9]+}} })
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
