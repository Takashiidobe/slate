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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-NEXT: struct atomic_flag {
// LOWERING-NEXT:     _Value: bool,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     flag = atomic_flag { _Value: false };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr({{_v[0-9]+}}).swap({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut bool = {{_v[0-9]+}} as *mut bool;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp2 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp2) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicU8::from_ptr({{_v[0-9]+}}).store({{_v[0-9]+}}, std::sync::atomic::Ordering::Release) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp3 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp3) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp2) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr({{_v[0-9]+}}).swap({{_v[0-9]+}}, std::sync::atomic::Ordering::Acquire) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut bool = {{_v[0-9]+}} as *mut bool;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp4 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp4) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp3) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr({{_v[0-9]+}}).swap({{_v[0-9]+}}, std::sync::atomic::Ordering::Relaxed) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut bool = {{_v[0-9]+}} as *mut bool;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp5 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp5) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     unsafe { std::sync::atomic::AtomicU8::from_ptr({{_v[0-9]+}}).store({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:     _atomictmp6 = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(flag._Value) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(_atomictmp6) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = std::ptr::addr_of_mut!(atomic_temp4) as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { std::sync::atomic::AtomicU8::from_ptr({{_v[0-9]+}}).swap({{_v[0-9]+}}, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut bool = {{_v[0-9]+}} as *mut bool;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: struct atomic_flag
// REWRITES-DAG: std::sync::atomic::AtomicU8::from_ptr
// REWRITES-DAG: .swap(
// REWRITES-DAG: std::sync::atomic::Ordering::Acquire
// REWRITES-DAG: .store(
// REWRITES-DAG: std::sync::atomic::Ordering::Release
