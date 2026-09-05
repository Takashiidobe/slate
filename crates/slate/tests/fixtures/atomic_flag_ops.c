#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_flag flag = ATOMIC_FLAG_INIT;

  int first = atomic_flag_test_and_set(&flag);
  atomic_flag_clear_explicit(&flag, memory_order_release);
  int second = atomic_flag_test_and_set_explicit(&flag, memory_order_acquire);
  int third  = atomic_flag_test_and_set_explicit(&flag, memory_order_relaxed);
  atomic_flag_clear(&flag);
  int fourth = atomic_flag_test_and_set(&flag);

  printf("%d %d %d %d\n", first, second, third, fourth);
  return 0;
}

// REWRITES-DAG: struct atomic_flag
// REWRITES-DAG: std::sync::atomic::AtomicU8::from_ptr
// REWRITES-DAG: .swap(
// REWRITES-DAG: std::sync::atomic::Ordering::Acquire
// REWRITES-DAG: .store(
// REWRITES-DAG: std::sync::atomic::Ordering::Release

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
// COMMON-LOWERING-NEXT: struct atomic_flag {
// COMMON-LOWERING-NEXT:     _Value: bool,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut flag: atomic_flag = atomic_flag { _Value: false };
// COMMON-LOWERING-NEXT:     let mut _atomictmp: bool = false;
// COMMON-LOWERING-NEXT:     let mut atomic_temp: bool = false;
// COMMON-LOWERING-NEXT:     let mut _atomictmp2: bool = false;
// COMMON-LOWERING-NEXT:     let mut _atomictmp3: bool = false;
// COMMON-LOWERING-NEXT:     let mut atomic_temp2: bool = false;
// COMMON-LOWERING-NEXT:     let mut _atomictmp4: bool = false;
// COMMON-LOWERING-NEXT:     let mut atomic_temp3: bool = false;
// COMMON-LOWERING-NEXT:     let mut _atomictmp5: bool = false;
// COMMON-LOWERING-NEXT:     let mut _atomictmp6: bool = false;
// COMMON-LOWERING-NEXT:     let mut atomic_temp4: bool = false;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: atomic_flag = atomic_flag { _Value: false };
// COMMON-LOWERING-NEXT:     flag = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     _atomictmp = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}}).swap({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bool = {{__v[0-9]+}} as *mut bool;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     _atomictmp2 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp2) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-LOWERING-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::Release)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     _atomictmp3 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp3) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp2) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::Acquire)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bool = {{__v[0-9]+}} as *mut bool;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     _atomictmp4 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp4) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp3) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::Relaxed)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bool = {{__v[0-9]+}} as *mut bool;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     _atomictmp5 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp5) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-LOWERING-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     _atomictmp6 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp6) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp4) as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bool = {{__v[0-9]+}} as *mut bool;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: struct atomic_flag {
// COMMON-REWRITES-NEXT:     _Value: bool,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut flag: atomic_flag = atomic_flag { _Value: false };
// COMMON-REWRITES-NEXT:     let mut _atomictmp: bool = false;
// COMMON-REWRITES-NEXT:     let mut atomic_temp: bool = false;
// COMMON-REWRITES-NEXT:     let mut _atomictmp2: bool = false;
// COMMON-REWRITES-NEXT:     let mut _atomictmp3: bool = false;
// COMMON-REWRITES-NEXT:     let mut atomic_temp2: bool = false;
// COMMON-REWRITES-NEXT:     let mut _atomictmp4: bool = false;
// COMMON-REWRITES-NEXT:     let mut atomic_temp3: bool = false;
// COMMON-REWRITES-NEXT:     let mut _atomictmp5: bool = false;
// COMMON-REWRITES-NEXT:     let mut _atomictmp6: bool = false;
// COMMON-REWRITES-NEXT:     let mut atomic_temp4: bool = false;
// COMMON-REWRITES-NEXT:     flag = atomic_flag { _Value: false };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     _atomictmp = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-REWRITES-NEXT:             .swap(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *({{__v[0-9]+}} as *mut bool) }) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     _atomictmp2 = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp2) as *mut u8;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-REWRITES-NEXT:             .store(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::Release)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     _atomictmp3 = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp3) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp2) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-REWRITES-NEXT:             .swap(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::Acquire)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *({{__v[0-9]+}} as *mut bool) }) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     _atomictmp4 = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp4) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp3) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-REWRITES-NEXT:             .swap(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::Relaxed)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *({{__v[0-9]+}} as *mut bool) }) as i32;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     _atomictmp5 = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp5) as *mut u8;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-REWRITES-NEXT:             .store(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-REWRITES-NEXT:     _atomictmp6 = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp6) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp4) as *mut u8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// COMMON-REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// COMMON-REWRITES-NEXT:             .swap(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::SeqCst)
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             (unsafe { *({{__v[0-9]+}} as *mut bool) }) as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
