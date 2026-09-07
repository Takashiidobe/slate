#include <stdio.h>

static int volatile debugtrap_never;

int main(void) {
  if (debugtrap_never) {
    __builtin_debugtrap();
  }
  printf("ok\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(link_llvm_intrinsics)]
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
// LOWERING-NEXT: static mut debugtrap_never: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() -> std::process::ExitCode {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(debugtrap_never)) };
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 unsafe { __slate_intrinsic_debugtrap_2b8d277b395439e1() };
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"ok\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     return std::process::ExitCode::SUCCESS;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "llvm-intrinsic" {
// LOWERING-NEXT:     #[link_name = "llvm.debugtrap"]
// LOWERING-NEXT:     fn __slate_intrinsic_debugtrap_2b8d277b395439e1();
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(link_llvm_intrinsics)]
// REWRITES-NEXT: #![feature(c_variadic)]
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
// REWRITES-NEXT: static mut debugtrap_never: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() -> std::process::ExitCode {
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(debugtrap_never)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             unsafe { __slate_intrinsic_debugtrap_2b8d277b395439e1() };
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     println!("ok");
// REWRITES-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-NEXT:     return std::process::ExitCode::SUCCESS;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "llvm-intrinsic" {
// REWRITES-NEXT:     #[link_name = "llvm.debugtrap"]
// REWRITES-NEXT:     fn __slate_intrinsic_debugtrap_2b8d277b395439e1();
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
