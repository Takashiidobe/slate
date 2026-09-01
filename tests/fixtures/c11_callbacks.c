#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

static int once_total;

static int thread_worker(void *argument) { return *(int *)argument + 1; }

static void once_handler(void) { once_total += 1; }

static void tss_destructor(void *value) { once_total += value != NULL; }

static void quick_handler(void) { once_total += 100; }

int main(void) {
  thrd_t thread;
  tss_t  key;
  int    argument       = 40;
  int    thread_result  = 0;
  int    thread_created = thrd_create(&thread, thread_worker, &argument);
  int    thread_joined =
      thread_created == thrd_success ? thrd_join(thread, &thread_result) : -1;

  once_flag control = ONCE_FLAG_INIT;
  call_once(&control, once_handler);
  call_once(&control, once_handler);

  int key_created = tss_create(&key, tss_destructor);
  if (key_created == thrd_success) {
    tss_delete(key);
  }

  int quick_registered = at_quick_exit(quick_handler);
  printf("%d %d %d %d %d %d\n", thread_created, thread_joined, thread_result,
         once_total, key_created, quick_registered);
  return 0;
}

// LOWERING-DAG: Option<unsafe extern "C" fn
// LOWERING-DAG: extern "C" fn thread_worker
// LOWERING-DAG: extern "C" fn once_handler
// LOWERING-DAG: extern "C" fn tss_destructor
// LOWERING-DAG: extern "C" fn quick_handler
// LOWERING-DAG: let mut control: i32 = 0;
// LOWERING-DAG: call_once(std::ptr::addr_of_mut!(control) as *mut i32
// LOWERING-NOT: Option<fn(

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
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
// REWRITES-NEXT: static mut once_total: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn thrd_create(
// REWRITES-NEXT:         _0: *mut u64,
// REWRITES-NEXT:         _1: Option<unsafe extern "C" fn(*mut core::ffi::c_void) -> i32>,
// REWRITES-NEXT:         _2: *mut core::ffi::c_void,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn thrd_join(_0: u64, _1: *mut i32) -> i32;
// REWRITES-NEXT:     fn call_once(_0: *mut i32, _1: Option<unsafe extern "C" fn()>);
// REWRITES-NEXT:     fn tss_create(_0: *mut u32, _1: Option<unsafe extern "C" fn(*mut core::ffi::c_void)>) -> i32;
// REWRITES-NEXT:     fn tss_delete(_0: u32);
// REWRITES-NEXT:     fn at_quick_exit(_0: Option<unsafe extern "C" fn()>) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn thread_worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     return (unsafe { *({{arg[0-9]+}} as *mut i32) }) + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn once_handler() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         once_total = (unsafe { once_total }) + {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn tss_destructor({{arg[0-9]+}}: *mut core::ffi::c_void) {
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         once_total = (unsafe { once_total }) + (({{arg[0-9]+}} != {{_v[0-9]+}}) as i32);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn quick_handler() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         once_total = (unsafe { once_total }) + {{_v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut thread: u64 = 0;
// REWRITES-NEXT:     let mut key: u32 = 0;
// REWRITES-NEXT:     let mut argument: i32 = 0;
// REWRITES-NEXT:     let mut thread_result: i32 = 0;
// REWRITES-NEXT:     let mut control: i32 = 0;
// REWRITES-NEXT:     let mut key_created: i32 = 0;
// REWRITES-NEXT:     argument = 40;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(argument) as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         thrd_create(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(thread) as *mut u64,
// REWRITES-NEXT:             Some(thread_worker),
// REWRITES-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:             thrd_join(
// REWRITES-NEXT:                 thread as u64,
// REWRITES-NEXT:                 std::ptr::addr_of_mut!(thread_result) as *mut i32,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     } else {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT:         {{_v[0-9]+}}
// REWRITES-NEXT:     };
// REWRITES-NEXT:     control = 0;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         call_once(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(control) as *mut i32,
// REWRITES-NEXT:             Some(once_handler),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         call_once(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(control) as *mut i32,
// REWRITES-NEXT:             Some(once_handler),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     key_created = unsafe {
// REWRITES-NEXT:         tss_create(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(key) as *mut u32,
// REWRITES-NEXT:             Some(tss_destructor),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = key_created == {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             unsafe { tss_delete(key as u32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             {{_v[0-9]+}},
// REWRITES-NEXT:             thread_result,
// REWRITES-NEXT:             unsafe { once_total },
// REWRITES-NEXT:             key_created,
// REWRITES-NEXT:             unsafe { at_quick_exit(Some(quick_handler)) },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
