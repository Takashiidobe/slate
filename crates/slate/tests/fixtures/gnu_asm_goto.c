#include <stdio.h>

static int gnu_asm_goto(int value) {
  __asm__ goto("testl %[value], %[value]\n\tjz %l[zero]"
               :
               : [value] "r"(value)
               : "cc"
               : zero);
  return 11;
zero:
  return 13;
}

static int gnu_asm_goto_output(int value) {
  int output = value;
  __asm__ goto("testl %[output], %[output]\n\tjz %l[zero]"
               : [output] "+r"(output)
               :
               : "cc"
               : zero);
  return output + 1;
zero:
  return output + 2;
}

int main(void) {
  printf("%d %d %d\n", gnu_asm_goto(0), gnu_asm_goto_output(5),
         gnu_asm_goto_output(0));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(asm_goto_with_outputs)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_asm_goto(arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut __state0: i32 = 0;
// LOWERING-NEXT:     '__dispatch0: loop {
// LOWERING-NEXT:         match __state0 {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 value = arg1;
// LOWERING-NEXT:                 let _v0: i32 = value;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", in(reg) _v0, label {
// LOWERING-NEXT:                         __state0 = 1;
// LOWERING-NEXT:                         continue '__dispatch0;
// LOWERING-NEXT:                     }, options(att_syntax));
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v1: i32 = 11;
// LOWERING-NEXT:                 __retval = _v1;
// LOWERING-NEXT:                 let _v2: i32 = __retval;
// LOWERING-NEXT:                 return _v2;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let _v3: i32 = 13;
// LOWERING-NEXT:                 __retval = _v3;
// LOWERING-NEXT:                 let _v4: i32 = __retval;
// LOWERING-NEXT:                 return _v4;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_asm_goto_output(arg0: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut output: i32 = 0;
// LOWERING-NEXT:     let mut __state0: i32 = 0;
// LOWERING-NEXT:     '__dispatch0: loop {
// LOWERING-NEXT:         match __state0 {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 value = arg0;
// LOWERING-NEXT:                 let _v0: i32 = value;
// LOWERING-NEXT:                 output = _v0;
// LOWERING-NEXT:                 let _v1: i32 = output;
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", inlateout(reg) _v1 => output, label {
// LOWERING-NEXT:                         __state0 = 1;
// LOWERING-NEXT:                         continue '__dispatch0;
// LOWERING-NEXT:                     }, options(att_syntax));
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 output = output;
// LOWERING-NEXT:                 let _v2: i32 = output;
// LOWERING-NEXT:                 let _v3: i32 = 1;
// LOWERING-NEXT:                 let _v4: i32 = _v2 + _v3;
// LOWERING-NEXT:                 __retval = _v4;
// LOWERING-NEXT:                 let _v5: i32 = __retval;
// LOWERING-NEXT:                 return _v5;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let _v6: i32 = output;
// LOWERING-NEXT:                 let _v7: i32 = 2;
// LOWERING-NEXT:                 let _v8: i32 = _v6 + _v7;
// LOWERING-NEXT:                 __retval = _v8;
// LOWERING-NEXT:                 let _v9: i32 = __retval;
// LOWERING-NEXT:                 return _v9;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 0;
// LOWERING-NEXT:     let _v3: i32 = gnu_asm_goto(_v2);
// LOWERING-NEXT:     let _v4: i32 = 5;
// LOWERING-NEXT:     let _v5: i32 = gnu_asm_goto_output(_v4);
// LOWERING-NEXT:     let _v6: i32 = 0;
// LOWERING-NEXT:     let _v7: i32 = gnu_asm_goto_output(_v6);
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v1 as *const i8, _v3, _v5, _v7) };
// LOWERING-NEXT:     let _v9: i32 = 0;
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v10 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(asm_goto_with_outputs)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_asm_goto(arg1: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = 0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut __state0: i32 = 0;
// REWRITES-NEXT: '__dispatch0: loop {
// REWRITES-NEXT:         match __state0 {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         value = arg1;
// REWRITES-NEXT:                         let _v0: i32 = value;
// REWRITES-NEXT:                         unsafe {
// REWRITES-NEXT:                                         core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", in(reg) _v0, label {
// REWRITES-NEXT:                                             __state0 = 1;
// REWRITES-NEXT:                                             continue '__dispatch0;
// REWRITES-NEXT:                                         }, options(att_syntax));
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         __retval = 11;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         __retval = 13;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_asm_goto_output(arg0: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = 0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut output: i32 = 0;
// REWRITES-NEXT: let mut __state0: i32 = 0;
// REWRITES-NEXT: '__dispatch0: loop {
// REWRITES-NEXT:         match __state0 {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         value = arg0;
// REWRITES-NEXT:                         output = value;
// REWRITES-NEXT:                         let _v1: i32 = output;
// REWRITES-NEXT:                         unsafe {
// REWRITES-NEXT:                                         core::arch::asm!("testl {0:e}, {0:e}\n\tjz {1}", inlateout(reg) _v1 => output, label {
// REWRITES-NEXT:                                             __state0 = 1;
// REWRITES-NEXT:                                             continue '__dispatch0;
// REWRITES-NEXT:                                         }, options(att_syntax));
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         output = output;
// REWRITES-NEXT:                         let _v3: i32 = 1;
// REWRITES-NEXT:                         __retval = output + _v3;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         let _v7: i32 = 2;
// REWRITES-NEXT:                         __retval = output + _v7;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 0;
// REWRITES-NEXT: let _v3: i32 = gnu_asm_goto(_v2);
// REWRITES-NEXT: let _v4: i32 = 5;
// REWRITES-NEXT: let _v5: i32 = gnu_asm_goto_output(_v4);
// REWRITES-NEXT: let _v6: i32 = 0;
// REWRITES-NEXT: let _v7: i32 = gnu_asm_goto_output(_v6);
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v1 as *const i8, _v3, _v5, _v7) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
