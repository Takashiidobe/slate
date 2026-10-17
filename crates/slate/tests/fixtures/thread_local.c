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

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(thread_local)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn thrd_create(
// CHECK-NEXT:         __thr: *mut u64,
// CHECK-NEXT:         __func: Option<unsafe extern "C-unwind" fn(*mut ()) -> i32>,
// CHECK-NEXT:         __arg: *mut (),
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn thrd_join(__thr: u64, __res: *mut i32) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut file_value: i32 = 5 as i32;
// CHECK-NEXT: fn update_values(file_next: i32, block_next: i32) -> i32 {
// CHECK-NEXT:     let result: i32 = (unsafe { file_value }) * 100 + unsafe { block_value };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         file_value = file_next;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         block_value = block_next;
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: #[thread_local]
// CHECK-NEXT: static mut block_value: i32 = 7 as i32;
// CHECK-NEXT: extern "C-unwind" fn worker(argument: *mut ()) -> i32 {
// CHECK-NEXT:     let values: *mut i32 = argument as *mut i32;
// CHECK-NEXT:     update_values(
// CHECK-NEXT:         unsafe { *unsafe { values.offset(0 as isize) } },
// CHECK-NEXT:         unsafe { *unsafe { values.offset(1 as isize) } },
// CHECK-NEXT:     )
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut thread: u64 = 0;
// CHECK-NEXT:     let mut values: [i32; 2] = [11, 13];
// CHECK-NEXT:     let main_before: i32 = update_values(17 as i32, 19 as i32);
// CHECK-NEXT:     let mut worker_result: i32 = 0;
// CHECK-NEXT:     let created: i32 = unsafe {
// CHECK-NEXT:         thrd_create(
// CHECK-NEXT:             std::ptr::addr_of_mut!(thread),
// CHECK-NEXT:             Some(worker),
// CHECK-NEXT:             values.as_mut_ptr() as *mut (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut joined: i32 = 0;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     if created == 0 {
// CHECK-NEXT:         [[V_0]] = unsafe { thrd_join(thread, std::ptr::addr_of_mut!(worker_result)) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = -1;
// CHECK-NEXT:     }
// CHECK-NEXT:     joined = [[V_0]];
// CHECK-NEXT:     let main_after: i32 = update_values(23 as i32, 29 as i32);
// CHECK-NEXT:     println!("{} {} {} {} {}", main_before, worker_result, main_after, created, joined);
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
