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
// LOWERING-NEXT: struct atomic_flag {
// LOWERING-NEXT:     _Value: bool,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut flag: atomic_flag = atomic_flag { _Value: false };
// LOWERING-NEXT:     let mut _atomictmp: bool = false;
// LOWERING-NEXT:     let mut atomic_temp: bool = false;
// LOWERING-NEXT:     let mut _atomictmp2: bool = false;
// LOWERING-NEXT:     let mut _atomictmp3: bool = false;
// LOWERING-NEXT:     let mut atomic_temp2: bool = false;
// LOWERING-NEXT:     let mut _atomictmp4: bool = false;
// LOWERING-NEXT:     let mut atomic_temp3: bool = false;
// LOWERING-NEXT:     let mut _atomictmp5: bool = false;
// LOWERING-NEXT:     let mut _atomictmp6: bool = false;
// LOWERING-NEXT:     let mut atomic_temp4: bool = false;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: atomic_flag = atomic_flag { _Value: false };
// LOWERING-NEXT:     flag = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}}).swap({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bool = {{__v[0-9]+}} as *mut bool;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp2 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp2) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// LOWERING-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::Release)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp3 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp3) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp2) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::Acquire)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bool = {{__v[0-9]+}} as *mut bool;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp4 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp4) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp3) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::Relaxed)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bool = {{__v[0-9]+}} as *mut bool;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp5 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp5) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// LOWERING-NEXT:             .store({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp6 = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp6) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp4) as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// LOWERING-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// LOWERING-NEXT:             .swap({{__v[0-9]+}}, std::sync::atomic::Ordering::SeqCst)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut bool = {{__v[0-9]+}} as *mut bool;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = unsafe { *{{__v[0-9]+}} };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-NEXT: struct atomic_flag {
// REWRITES-NEXT:     _Value: bool,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut flag: atomic_flag = atomic_flag { _Value: false };
// REWRITES-NEXT:     let mut _atomictmp: bool = false;
// REWRITES-NEXT:     let mut atomic_temp: bool = false;
// REWRITES-NEXT:     let mut _atomictmp2: bool = false;
// REWRITES-NEXT:     let mut _atomictmp3: bool = false;
// REWRITES-NEXT:     let mut atomic_temp2: bool = false;
// REWRITES-NEXT:     let mut _atomictmp4: bool = false;
// REWRITES-NEXT:     let mut atomic_temp3: bool = false;
// REWRITES-NEXT:     let mut _atomictmp5: bool = false;
// REWRITES-NEXT:     let mut _atomictmp6: bool = false;
// REWRITES-NEXT:     let mut atomic_temp4: bool = false;
// REWRITES-NEXT:     flag = atomic_flag { _Value: false };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     _atomictmp = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// REWRITES-NEXT:             .swap(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *({{__v[0-9]+}} as *mut bool) }) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     _atomictmp2 = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp2) as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// REWRITES-NEXT:             .store(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::Release)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     _atomictmp3 = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp3) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp2) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// REWRITES-NEXT:             .swap(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::Acquire)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *({{__v[0-9]+}} as *mut bool) }) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     _atomictmp4 = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp4) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp3) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// REWRITES-NEXT:             .swap(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::Relaxed)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = (unsafe { *({{__v[0-9]+}} as *mut bool) }) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     _atomictmp5 = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp5) as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// REWRITES-NEXT:             .store(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     _atomictmp6 = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp6) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp4) as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: u8 = unsafe {
// REWRITES-NEXT:         std::sync::atomic::AtomicU8::from_ptr({{__v[0-9]+}})
// REWRITES-NEXT:             .swap(unsafe { *{{__v[0-9]+}} }, std::sync::atomic::Ordering::SeqCst)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             (unsafe { *({{__v[0-9]+}} as *mut bool) }) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
