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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(thread_local)]
// LOWERING-NEXT: #![feature(c_variadic)]
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
// LOWERING-NEXT: #[thread_local]
// LOWERING-NEXT: static mut file_value: i32 = 5;
// LOWERING-EMPTY:
// LOWERING-NEXT: #[thread_local]
// LOWERING-NEXT: static mut update_values_block_value: i32 = 7;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn thrd_create(
// LOWERING-NEXT:         _0: *mut u64,
// LOWERING-NEXT:         _1: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> i32>,
// LOWERING-NEXT:         _2: *mut core::ffi::c_void,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn thrd_join(_0: u64, _1: *mut i32) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut thread: u64 = 0;
// LOWERING-NEXT:     let mut values: [i32; 2] = [0; 2];
// LOWERING-NEXT:     let mut worker_result: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 2] = [11, 13];
// LOWERING-NEXT:     values = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 17;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 19;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = update_values({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     worker_result = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         thrd_create(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(thread) as *mut u64,
// LOWERING-NEXT:             Some(worker),
// LOWERING-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = thread;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:             thrd_join(
// LOWERING-NEXT:                 {{_v[0-9]+}} as u64,
// LOWERING-NEXT:                 std::ptr::addr_of_mut!(worker_result) as *mut i32,
// LOWERING-NEXT:             )
// LOWERING-NEXT:         };
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:         {{_v[0-9]+}}
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 23;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 29;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = update_values({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = worker_result;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn update_values({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { file_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { update_values_block_value };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         file_value = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         update_values_block_value = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = update_values({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering
