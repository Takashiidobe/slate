#include <stdbool.h>
#include <stdio.h>

int main(void) {
  bool flag   = false;
  int  first  = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  int  second = __atomic_test_and_set(&flag, __ATOMIC_SEQ_CST);
  __atomic_clear(&flag, __ATOMIC_SEQ_CST);
  printf("%d %d %d\n", first, second, !flag);
  return 0;
}

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU: #![feature(c_variadic)]
// REWRITES-X86_64-GNU-NEXT: #![allow(
// REWRITES-X86_64-GNU-NEXT:     dead_code,
// REWRITES-X86_64-GNU-NEXT:     unused,
// REWRITES-X86_64-GNU-NEXT:     non_camel_case_types,
// REWRITES-X86_64-GNU-NEXT:     non_snake_case,
// REWRITES-X86_64-GNU-NEXT:     non_upper_case_globals,
// REWRITES-X86_64-GNU-NEXT:     arithmetic_overflow,
// REWRITES-X86_64-GNU-NEXT:     unconditional_panic,
// REWRITES-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-X86_64-GNU-NEXT:     unused_comparisons
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn main() -> std::process::ExitCode {
// REWRITES-X86_64-GNU-NEXT:     let mut flag: bool = false;
// REWRITES-X86_64-GNU-NEXT:     flag = false;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = (unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::sync::atomic::AtomicI8::from_ptr({{__v[0-9]+}}).swap(1, std::sync::atomic::Ordering::SeqCst)
// REWRITES-X86_64-GNU-NEXT:     }) != 0;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = (unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::sync::atomic::AtomicI8::from_ptr({{__v[0-9]+}}).swap(1, std::sync::atomic::Ordering::SeqCst)
// REWRITES-X86_64-GNU-NEXT:     }) != 0;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::sync::atomic::AtomicI8::from_ptr({{__v[0-9]+}}).store(0, std::sync::atomic::Ordering::SeqCst)
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d %d %d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = !flag;
// REWRITES-X86_64-GNU-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-X86_64-GNU-NEXT:     unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}} as i32) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// REWRITES-X86_64-GNU-NEXT:     return std::process::ExitCode::SUCCESS;
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU: #![feature(c_variadic)]
// LOWERING-X86_64-GNU-NEXT: #![allow(
// LOWERING-X86_64-GNU-NEXT:     dead_code,
// LOWERING-X86_64-GNU-NEXT:     unused,
// LOWERING-X86_64-GNU-NEXT:     non_camel_case_types,
// LOWERING-X86_64-GNU-NEXT:     non_snake_case,
// LOWERING-X86_64-GNU-NEXT:     non_upper_case_globals,
// LOWERING-X86_64-GNU-NEXT:     arithmetic_overflow,
// LOWERING-X86_64-GNU-NEXT:     unconditional_panic,
// LOWERING-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-X86_64-GNU-NEXT:     unused_comparisons
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn main() -> std::process::ExitCode {
// LOWERING-X86_64-GNU-NEXT:     let mut flag: bool = false;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:     flag = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = (unsafe {
// LOWERING-X86_64-GNU-NEXT:         std::sync::atomic::AtomicI8::from_ptr({{__v[0-9]+}}).swap(1, std::sync::atomic::Ordering::SeqCst)
// LOWERING-X86_64-GNU-NEXT:     }) != 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = (unsafe {
// LOWERING-X86_64-GNU-NEXT:         std::sync::atomic::AtomicI8::from_ptr({{__v[0-9]+}}).swap(1, std::sync::atomic::Ordering::SeqCst)
// LOWERING-X86_64-GNU-NEXT:     }) != 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = std::ptr::addr_of_mut!(flag) as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         std::sync::atomic::AtomicI8::from_ptr({{__v[0-9]+}}).store(0, std::sync::atomic::Ordering::SeqCst)
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = flag;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     return std::process::ExitCode::SUCCESS;
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering
