#include <stdio.h>

/* noreturn via an infinite loop: correct C, but Slate's lowering does not yet
 * prove this diverges (the generated Rust loop keeps a conditional break
 * guard), so it must fall back to a conservative, non-`!` return type
 * instead of emitting Rust that rustc would reject. */
__attribute__((noreturn)) static void spin(void) {
  for (;;) {
  }
}

void (*unused_spin)(void) = spin;

int main(void) {
  printf("main\n");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut unused_spin: Option<unsafe extern "C" fn()> = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn()>>(spin as *const ()) };
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: /// noreturn via an infinite loop: correct C, but Slate's lowering does not yet
// LOWERING-NEXT: /// prove this diverges (the generated Rust loop keeps a conditional break
// LOWERING-NEXT: /// guard), so it must fall back to a conservative, non-`!` return type
// LOWERING-NEXT: /// instead of emitting Rust that rustc would reject.
// LOWERING-NEXT: extern "C" fn spin() {
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v0: bool = true;
// LOWERING-NEXT:             if !_v0 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"main\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = unsafe { printf(_v1 as *const i8) };
// LOWERING-NEXT:     let _v3: i32 = 0;
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v4 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut unused_spin: Option<unsafe extern "C" fn()> = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn()>>(spin as *const ()) };
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: /// noreturn via an infinite loop: correct C, but Slate's lowering does not yet
// REWRITES-NEXT: /// prove this diverges (the generated Rust loop keeps a conditional break
// REWRITES-NEXT: /// guard), so it must fall back to a conservative, non-`!` return type
// REWRITES-NEXT: /// instead of emitting Rust that rustc would reject.
// REWRITES-NEXT: extern "C" fn spin() {
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         if !true {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"main\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = unsafe { printf(_v1 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
