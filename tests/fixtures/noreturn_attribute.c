#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn)) void die(int code) {
  printf("dying with %d\n", code);
  exit(code);
}

int main(void) {
  if (0) {
    die(1);
  }
  printf("main\n");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn exit(_0: i32) -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn die(arg0: i32) -> ! {
// LOWERING-NEXT:     let mut code: i32 = 0;
// LOWERING-NEXT:     code = arg0;
// LOWERING-NEXT:     let _v0: *mut i8 = b"dying with %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v1: i32 = code;
// LOWERING-NEXT:     let _v2: i32 = unsafe { printf(_v0 as *const i8, _v1) };
// LOWERING-NEXT:     let _v3: i32 = code;
// LOWERING-NEXT:     unsafe { exit(_v3 as i32) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         let _v2: bool = _v1 != 0;
// LOWERING-NEXT:         if _v2 {
// LOWERING-NEXT:             let _v3: i32 = 1;
// LOWERING-NEXT:             die(_v3);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v4: *mut i8 = b"main\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v4 as *const i8) };
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
// REWRITES-NEXT:     fn exit(_0: i32) -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn die(arg0: i32) -> ! {
// REWRITES-NEXT: let mut code: i32 = arg0;
// REWRITES-NEXT: let _v0: *mut i8 = b"dying with %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = unsafe { printf(_v0 as *const i8, code) };
// REWRITES-NEXT: unsafe { std::process::exit(code as i32) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v1: i32 = 0;
// REWRITES-NEXT:         let _v2: bool = _v1 != 0;
// REWRITES-NEXT:         if _v2 {
// REWRITES-NEXT:                     let _v3: i32 = 1;
// REWRITES-NEXT:                     die(_v3);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v4: *mut i8 = b"main\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v4 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
