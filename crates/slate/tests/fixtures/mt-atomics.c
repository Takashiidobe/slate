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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union __pthread_attr_t {
// LOWERING-NEXT:     __size: [i8; 56],
// LOWERING-NEXT:     __align: i64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut counter: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn pthread_create(_0: *mut u64, _1: *const __pthread_attr_t, _2: Option<unsafe extern "C" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void>, _3: *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn pthread_join(_0: u64, _1: *mut *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn worker(arg0: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// LOWERING-NEXT:     let mut arg: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     arg = arg0;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = arg;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = 100000;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v5: i32 = 1;
// LOWERING-NEXT:                 let _v6: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter)).fetch_add(_v5, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v7: i32 = i;
// LOWERING-NEXT:             let _v8: i32 = _v7 + 1;
// LOWERING-NEXT:             i = _v8;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v9: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: *mut core::ffi::c_void = __retval;
// LOWERING-NEXT:     return _v10;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut threads: aligned::Aligned<aligned::A16, [u64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = 4;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v5: i32 = i;
// LOWERING-NEXT:                 let _v6: i64 = _v5 as i64;
// LOWERING-NEXT:                 let _v7: *mut __pthread_attr_t = std::ptr::null_mut();
// LOWERING-NEXT:                 let _v8: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:                 let _v9: i32 = unsafe { pthread_create(std::ptr::addr_of_mut!(threads[(_v6 as usize)]) as *mut u64, _v7 as *const __pthread_attr_t, Some(worker), _v8 as *mut core::ffi::c_void) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v10: i32 = i;
// LOWERING-NEXT:             let _v11: i32 = _v10 + 1;
// LOWERING-NEXT:             i = _v11;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i2: i32 = 0;
// LOWERING-NEXT:         let _v12: i32 = 0;
// LOWERING-NEXT:         i2 = _v12;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v13: i32 = i2;
// LOWERING-NEXT:             let _v14: i32 = 4;
// LOWERING-NEXT:             let _v15: bool = _v13 < _v14;
// LOWERING-NEXT:             if !_v15 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v16: i32 = i2;
// LOWERING-NEXT:                 let _v17: i64 = _v16 as i64;
// LOWERING-NEXT:                 let _v18: u64 = threads[(_v17 as usize)];
// LOWERING-NEXT:                 let _v19: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:                 let _v20: i32 = unsafe { pthread_join(_v18 as u64, _v19 as *mut *mut core::ffi::c_void) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v21: i32 = i2;
// LOWERING-NEXT:             let _v22: i32 = _v21 + 1;
// LOWERING-NEXT:             i2 = _v22;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v23: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v24: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter)).load(std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v25: i32 = unsafe { printf(_v23 as *const i8, _v24) };
// LOWERING-NEXT:     let _v26: i32 = 0;
// LOWERING-NEXT:     __retval = _v26;
// LOWERING-NEXT:     let _v27: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v27 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union __pthread_attr_t {
// REWRITES-NEXT:     __size: [i8; 56],
// REWRITES-NEXT:     __align: i64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut counter: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn pthread_create(_0: *mut u64, _1: *const __pthread_attr_t, _2: Option<unsafe extern "C" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void>, _3: *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn pthread_join(_0: u64, _1: *mut *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn worker(arg0: *mut core::ffi::c_void) -> *mut core::ffi::c_void {
// REWRITES-NEXT: let mut __retval: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let _v0: *mut core::ffi::c_void = arg0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v3: i32 = 100000;
// REWRITES-NEXT:                     if !(i < _v3) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v5: i32 = 1;
// REWRITES-NEXT:                                     let _v6: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter)).fetch_add(_v5, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = std::ptr::null_mut();
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut threads: aligned::Aligned<aligned::A16, [u64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v3: i32 = 4;
// REWRITES-NEXT:                     if !(i < _v3) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v6: i64 = i as i64;
// REWRITES-NEXT:                                     let _v7: *mut __pthread_attr_t = std::ptr::null_mut();
// REWRITES-NEXT:                                     let _v8: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:                                     let _v9: i32 = unsafe { pthread_create(std::ptr::addr_of_mut!(threads[(_v6 as usize)]) as *mut u64, _v7 as *const __pthread_attr_t, Some(worker), _v8 as *mut core::ffi::c_void) };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i2: i32 = 0;
// REWRITES-NEXT:         i2 = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v14: i32 = 4;
// REWRITES-NEXT:                     if !(i2 < _v14) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v19: *mut *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT:                                     let _v20: i32 = unsafe { pthread_join(threads[((i2 as i64) as usize)] as u64, _v19 as *mut *mut core::ffi::c_void) };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i2 = i2 + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v23: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v24: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(counter)).load(std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v25: i32 = unsafe { printf(_v23 as *const i8, _v24) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
