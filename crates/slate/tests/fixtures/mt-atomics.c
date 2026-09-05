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
// COMMON-LOWERING-NEXT: #[allow(non_camel_case_types)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-LOWERING-NEXT: enum memory_order {
// COMMON-LOWERING-NEXT:     memory_order_relaxed = 0,
// COMMON-LOWERING-NEXT:     memory_order_consume = 1,
// COMMON-LOWERING-NEXT:     memory_order_acquire = 2,
// COMMON-LOWERING-NEXT:     memory_order_release = 3,
// COMMON-LOWERING-NEXT:     memory_order_acq_rel = 4,
// COMMON-LOWERING-NEXT:     memory_order_seq_cst = 5,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union __pthread_attr_t {
// COMMON-LOWERING-NEXT:     __align: i64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut counter: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn pthread_create(
// COMMON-LOWERING-NEXT:         _0: *mut u64,
// COMMON-LOWERING-NEXT:         _1: *const __pthread_attr_t,
// COMMON-LOWERING-NEXT:         _2: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void>,
// COMMON-LOWERING-NEXT:         _3: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     ) -> i32;
// COMMON-LOWERING-NEXT:     fn pthread_join(_0: u64, _1: *mut *mut core::ffi::c_void) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut __pthread_attr_t = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:                     pthread_create(
// COMMON-LOWERING-NEXT:                         std::ptr::addr_of_mut!(threads[({{__v[0-9]+}} as usize)]) as *mut u64,
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *const __pthread_attr_t,
// COMMON-LOWERING-NEXT:                         Some(worker),
// COMMON-LOWERING-NEXT:                         {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:                     )
// COMMON-LOWERING-NEXT:                 };
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i2: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = threads[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 =
// COMMON-LOWERING-NEXT:                     unsafe { pthread_join({{__v[0-9]+}} as u64, {{__v[0-9]+}} as *mut *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i2;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter))
// COMMON-LOWERING-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 100000;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:                     std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter))
// COMMON-LOWERING-NEXT:                         .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:                 };
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     __size: [i8; 56],
// LOWERING-X86_64-GNU-NEXT:     let mut threads: aligned::Aligned<aligned::A16, [u64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     __size: [u8; 64],
// LOWERING-AARCH64-GNU-NEXT:     let mut threads: [u64; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: #[allow(non_camel_case_types)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// COMMON-REWRITES-NEXT: enum memory_order {
// COMMON-REWRITES-NEXT:     memory_order_relaxed = 0,
// COMMON-REWRITES-NEXT:     memory_order_consume = 1,
// COMMON-REWRITES-NEXT:     memory_order_acquire = 2,
// COMMON-REWRITES-NEXT:     memory_order_release = 3,
// COMMON-REWRITES-NEXT:     memory_order_acq_rel = 4,
// COMMON-REWRITES-NEXT:     memory_order_seq_cst = 5,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union __pthread_attr_t {
// COMMON-REWRITES-NEXT:     __align: i64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut counter: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn pthread_create(
// COMMON-REWRITES-NEXT:         _0: *mut u64,
// COMMON-REWRITES-NEXT:         _1: *const __pthread_attr_t,
// COMMON-REWRITES-NEXT:         _2: Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void>,
// COMMON-REWRITES-NEXT:         _3: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     ) -> i32;
// COMMON-REWRITES-NEXT:     fn pthread_join(_0: u64, _1: *mut *mut core::ffi::c_void) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     for i in 0..4 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i64 = i as i64;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut __pthread_attr_t = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             pthread_create(
// COMMON-REWRITES-NEXT:                 std::ptr::addr_of_mut!(threads[({{__v[0-9]+}} as usize)]) as *mut u64,
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *const __pthread_attr_t,
// COMMON-REWRITES-NEXT:                 Some(worker),
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let mut i2: i32 = 0;
// COMMON-REWRITES-NEXT:     while i2 < 4 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             pthread_join(
// COMMON-REWRITES-NEXT:                 threads[((i2 as i64) as usize)] as u64,
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as *mut *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:         i2 += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter))
// COMMON-REWRITES-NEXT:             .load(std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn worker({{arg[0-9]+}}: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// COMMON-REWRITES-NEXT:     for i in 0..100000 {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter))
// COMMON-REWRITES-NEXT:                 .fetch_add({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     std::ptr::null_mut()
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     __size: [i8; 56],
// REWRITES-X86_64-GNU-NEXT:     let mut threads: aligned::Aligned<aligned::A16, [u64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     __size: [u8; 64],
// REWRITES-AARCH64-GNU-NEXT:     let mut threads: [u64; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d\n".as_ptr() as *mut u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
