#include <stdio.h>

static int asm_probe(int x) {
  __asm__ volatile("" : "+r"(x));
  return x + 1;
}

int main(void) {
  volatile int input = 4;
  printf("%d\n", asm_probe(input));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn asm_probe(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("\n/* {0:e} */", inlateout(reg) _v0 => _v1, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: i32 = x;
// LOWERING-NEXT:     let _v3: i32 = 1;
// LOWERING-NEXT:     let _v4: i32 = _v2 + _v3;
// LOWERING-NEXT:     __retval = _v4;
// LOWERING-NEXT:     let _v5: i32 = __retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut input: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 4;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v1) };
// LOWERING-NEXT:     let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let _v4: i32 = asm_probe(_v3);
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v2 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     __retval = _v6;
// LOWERING-NEXT:     let _v7: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v7 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn asm_probe(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = arg0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let _v0: i32 = x;
// REWRITES-NEXT: let _v1: i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("\n/* {0:e} */", inlateout(reg) _v0 => _v1, options(att_syntax));
// REWRITES-NEXT: }
// REWRITES-NEXT: x = _v1;
// REWRITES-NEXT: let _v3: i32 = 1;
// REWRITES-NEXT: __retval = x + _v3;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut input: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: i32 = 4;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), _v1) };
// REWRITES-NEXT: let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let _v4: i32 = asm_probe(_v3);
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v2 as *const i8, _v4) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
