#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

static atomic_int counter = 0;

static void *worker(void *arg) {
  (void)arg;
  for (int i = 0; i < 100000; i++) {
    atomic_fetch_add(&counter, 1);
  }
  return 0;
}

int main(void) {
  pthread_t threads[4];
  for (int i = 0; i < 4; i++) {
    pthread_create(&threads[i], 0, worker, 0);
  }
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], 0);
  }
  printf("%d\n", (int)counter);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum memory_order {
// LOWERING-NEXT:     memory_order_relaxed = 0,
// LOWERING-NEXT:     memory_order_consume = 1,
// LOWERING-NEXT:     memory_order_acquire = 2,
// LOWERING-NEXT:     memory_order_release = 3,
// LOWERING-NEXT:     memory_order_acq_rel = 4,
// LOWERING-NEXT:     memory_order_seq_cst = 5,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union __pthread_attr_t {
// LOWERING-NEXT:     __size: [i8; 56],
// LOWERING-NEXT:     __align: i64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut counter: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn pthread_create(
// LOWERING-NEXT:         _0: *mut u64,
// LOWERING-NEXT:         _1: *const __pthread_attr_t,
// LOWERING-NEXT:         _2: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void>,
// LOWERING-NEXT:         _3: *mut core::ffi::c_void,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn pthread_join(_0: u64, _1: *mut *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 100000;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:                     std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter))
// LOWERING-NEXT:                         .fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:                 };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut threads: aligned::Aligned<aligned::A16, [u64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut __pthread_attr_t = std::ptr::null_mut();
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:                     pthread_create(
// LOWERING-NEXT:                         std::ptr::addr_of_mut!(threads[({{_v[0-9]+}} as usize)]) as *mut u64,
// LOWERING-NEXT:                         {{_v[0-9]+}} as *const __pthread_attr_t,
// LOWERING-NEXT:                         Some(worker),
// LOWERING-NEXT:                         {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-NEXT:                     )
// LOWERING-NEXT:                 };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i2: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i2 = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = threads[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 =
// LOWERING-NEXT:                     unsafe { pthread_join({{_v[0-9]+}} as u64, {{_v[0-9]+}} as *mut *mut core::ffi::c_void) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i2;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i2 = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter))
// LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum memory_order {
// REWRITES-NEXT:     memory_order_relaxed = 0,
// REWRITES-NEXT:     memory_order_consume = 1,
// REWRITES-NEXT:     memory_order_acquire = 2,
// REWRITES-NEXT:     memory_order_release = 3,
// REWRITES-NEXT:     memory_order_acq_rel = 4,
// REWRITES-NEXT:     memory_order_seq_cst = 5,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union __pthread_attr_t {
// REWRITES-NEXT:     __size: [i8; 56],
// REWRITES-NEXT:     __align: i64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut counter: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn pthread_create(
// REWRITES-NEXT:         _0: *mut u64,
// REWRITES-NEXT:         _1: *const __pthread_attr_t,
// REWRITES-NEXT:         _2: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void>,
// REWRITES-NEXT:         _3: *mut core::ffi::c_void,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn pthread_join(_0: u64, _1: *mut *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// REWRITES-NEXT:     for i in 0..100000 {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter))
// REWRITES-NEXT:                 .fetch_add({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:         };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     std::ptr::null_mut()
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut threads: aligned::Aligned<aligned::A16, [u64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT:     for i in 0..4 {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i64 = i as i64;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut __pthread_attr_t = std::ptr::null_mut();
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             pthread_create(
// REWRITES-NEXT:                 std::ptr::addr_of_mut!(threads[({{_v[0-9]+}} as usize)]) as *mut u64,
// REWRITES-NEXT:                 {{_v[0-9]+}} as *const __pthread_attr_t,
// REWRITES-NEXT:                 Some(worker),
// REWRITES-NEXT:                 {{_v[0-9]+}} as *mut core::ffi::c_void,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let mut i2: i32 = 0;
// REWRITES-NEXT:     while i2 < 4 {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             pthread_join(
// REWRITES-NEXT:                 threads[((i2 as i64) as usize)] as u64,
// REWRITES-NEXT:                 {{_v[0-9]+}} as *mut *mut core::ffi::c_void,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:         i2 += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter))
// REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
