#include <stdio.h>

static int gnu_extended_asm(int input) {
  int output;
  int tied;
  int early;

  __asm__ volatile("leal 3(%1), %0" : "=r"(output) : "r"(input) : "cc");
  __asm__ volatile("addl $2, %0" : "+r"(output) : : "cc");
  __asm__("imull %[right], %[result]"
          : [result] "=r"(tied)
          : "0"(output), [right] "r"(2)
          : "cc");
  __asm__("movl %[left], %[result]\n\taddl %[right], %[result]"
          : [result] "=&r"(early)
          : [left] "r"(tied), [right] "r"(1)
          : "cc");
  __asm__("addl $%c[amount], %[value]"
          : [value] "+r"(early)
          : [amount] "i"(4)
          : "cc");
  return early;
}

static int gnu_numeric_operands(int left, int right) {
  int result;
  __asm__("subl %2, %0" : "=r"(result) : "0"(left), "r"(right) : "cc");
  return result;
}

static int gnu_multiple_outputs(void) {
  int left;
  int right;
  __asm__("movl $3, %0\n\tmovl $4, %1" : "=r"(left), "=r"(right));
  return left * 10 + right;
}

int main(void) {
  printf("%d %d %d\n", gnu_extended_asm(7), gnu_numeric_operands(19, 4),
         gnu_multiple_outputs());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct anon_struct {
// LOWERING-NEXT:     __slate_anon_0: i32,
// LOWERING-NEXT:     __slate_anon_1: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_extended_asm(arg2: i32) -> i32 {
// LOWERING-NEXT:     let mut input: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut output: i32 = 0;
// LOWERING-NEXT:     let mut tied: i32 = 0;
// LOWERING-NEXT:     let mut early: i32 = 0;
// LOWERING-NEXT:     input = arg2;
// LOWERING-NEXT:     let _v0: i32 = input;
// LOWERING-NEXT:     let _v1: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("leal 3({1:e}), {0:e}", lateout(reg) _v1, in(reg) _v0, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     output = _v1;
// LOWERING-NEXT:     let _v2: i32 = output;
// LOWERING-NEXT:     let _v3: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("addl $2, {0:e}", inlateout(reg) _v2 => _v3, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     output = _v3;
// LOWERING-NEXT:     let _v4: i32 = output;
// LOWERING-NEXT:     let _v5: i32 = 2;
// LOWERING-NEXT:     let _v6: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("imull {1:e}, {0:e}", inlateout(reg) _v4 => _v6, in(reg) _v5, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     tied = _v6;
// LOWERING-NEXT:     let _v7: i32 = tied;
// LOWERING-NEXT:     let _v8: i32 = 1;
// LOWERING-NEXT:     let _v9: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) _v9, in(reg) _v7, in(reg) _v8, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     early = _v9;
// LOWERING-NEXT:     let _v10: i32 = early;
// LOWERING-NEXT:     let _v11: i32 = 4;
// LOWERING-NEXT:     let _v12: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) _v10 => _v12, const 4, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     early = _v12;
// LOWERING-NEXT:     let _v13: i32 = early;
// LOWERING-NEXT:     __retval = _v13;
// LOWERING-NEXT:     let _v14: i32 = __retval;
// LOWERING-NEXT:     return _v14;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_numeric_operands(arg0: i32, arg1: i32) -> i32 {
// LOWERING-NEXT:     let mut left: i32 = 0;
// LOWERING-NEXT:     let mut right: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     left = arg0;
// LOWERING-NEXT:     right = arg1;
// LOWERING-NEXT:     let _v0: i32 = left;
// LOWERING-NEXT:     let _v1: i32 = right;
// LOWERING-NEXT:     let _v2: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("subl {1:e}, {0:e}", inlateout(reg) _v0 => _v2, in(reg) _v1, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     result = _v2;
// LOWERING-NEXT:     let _v3: i32 = result;
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: i32 = __retval;
// LOWERING-NEXT:     return _v4;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn gnu_multiple_outputs() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut left: i32 = 0;
// LOWERING-NEXT:     let mut right: i32 = 0;
// LOWERING-NEXT:     let mut __asm_result: anon_struct = anon_struct { __slate_anon_0: 0, __slate_anon_1: 0 };
// LOWERING-NEXT:     let _v0: i32;
// LOWERING-NEXT:     let _v1: i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         core::arch::asm!("movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) _v0, lateout(reg) _v1, options(att_syntax));
// LOWERING-NEXT:     }
// LOWERING-NEXT:     left = _v0;
// LOWERING-NEXT:     right = _v1;
// LOWERING-NEXT:     let _v2: i32 = left;
// LOWERING-NEXT:     let _v3: i32 = 10;
// LOWERING-NEXT:     let _v4: i32 = _v2 * _v3;
// LOWERING-NEXT:     let _v5: i32 = right;
// LOWERING-NEXT:     let _v6: i32 = _v4 + _v5;
// LOWERING-NEXT:     __retval = _v6;
// LOWERING-NEXT:     let _v7: i32 = __retval;
// LOWERING-NEXT:     return _v7;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = 7;
// LOWERING-NEXT:     let _v3: i32 = gnu_extended_asm(_v2);
// LOWERING-NEXT:     let _v4: i32 = 19;
// LOWERING-NEXT:     let _v5: i32 = 4;
// LOWERING-NEXT:     let _v6: i32 = gnu_numeric_operands(_v4, _v5);
// LOWERING-NEXT:     let _v7: i32 = gnu_multiple_outputs();
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v1 as *const i8, _v3, _v6, _v7) };
// LOWERING-NEXT:     let _v9: i32 = 0;
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v10 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct anon_struct {
// REWRITES-NEXT:     __slate_anon_0: i32,
// REWRITES-NEXT:     __slate_anon_1: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_extended_asm(arg2: i32) -> i32 {
// REWRITES-NEXT: let mut input: i32 = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut output: i32 = 0;
// REWRITES-NEXT: let mut tied: i32 = 0;
// REWRITES-NEXT: let mut early: i32 = 0;
// REWRITES-NEXT: let _v0: i32 = input;
// REWRITES-NEXT: let _v1: i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("leal 3({1:e}), {0:e}", lateout(reg) _v1, in(reg) _v0, options(att_syntax));
// REWRITES-NEXT: }
// REWRITES-NEXT: output = _v1;
// REWRITES-NEXT: let _v2: i32 = output;
// REWRITES-NEXT: let _v3: i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("addl $2, {0:e}", inlateout(reg) _v2 => _v3, options(att_syntax));
// REWRITES-NEXT: }
// REWRITES-NEXT: output = _v3;
// REWRITES-NEXT: let _v4: i32 = output;
// REWRITES-NEXT: let _v5: i32 = 2;
// REWRITES-NEXT: let _v6: i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("imull {1:e}, {0:e}", inlateout(reg) _v4 => _v6, in(reg) _v5, options(att_syntax));
// REWRITES-NEXT: }
// REWRITES-NEXT: tied = _v6;
// REWRITES-NEXT: let _v7: i32 = tied;
// REWRITES-NEXT: let _v8: i32 = 1;
// REWRITES-NEXT: let _v9: i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) _v9, in(reg) _v7, in(reg) _v8, options(att_syntax));
// REWRITES-NEXT: }
// REWRITES-NEXT: early = _v9;
// REWRITES-NEXT: let _v10: i32 = early;
// REWRITES-NEXT: let _v11: i32 = 4;
// REWRITES-NEXT: let _v12: i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) _v10 => _v12, const 4, options(att_syntax));
// REWRITES-NEXT: }
// REWRITES-NEXT: early = _v12;
// REWRITES-NEXT: __retval = early;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_numeric_operands(arg0: i32, arg1: i32) -> i32 {
// REWRITES-NEXT: let mut left: i32 = arg0;
// REWRITES-NEXT: let mut right: i32 = arg1;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: let _v0: i32 = left;
// REWRITES-NEXT: let _v1: i32 = right;
// REWRITES-NEXT: let _v2: i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("subl {1:e}, {0:e}", inlateout(reg) _v0 => _v2, in(reg) _v1, options(att_syntax));
// REWRITES-NEXT: }
// REWRITES-NEXT: result = _v2;
// REWRITES-NEXT: __retval = result;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn gnu_multiple_outputs() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut left: i32 = 0;
// REWRITES-NEXT: let mut right: i32 = 0;
// REWRITES-NEXT: let mut __asm_result: anon_struct = anon_struct { __slate_anon_0: 0, __slate_anon_1: 0 };
// REWRITES-NEXT: let _v0: i32;
// REWRITES-NEXT: let _v1: i32;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         core::arch::asm!("movl $3, {0:e}\n\tmovl $4, {1:e}", lateout(reg) _v0, lateout(reg) _v1, options(att_syntax));
// REWRITES-NEXT: }
// REWRITES-NEXT: left = _v0;
// REWRITES-NEXT: right = _v1;
// REWRITES-NEXT: let _v3: i32 = 10;
// REWRITES-NEXT: __retval = left * _v3 + right;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = 7;
// REWRITES-NEXT: let _v3: i32 = gnu_extended_asm(_v2);
// REWRITES-NEXT: let _v4: i32 = 19;
// REWRITES-NEXT: let _v5: i32 = 4;
// REWRITES-NEXT: let _v6: i32 = gnu_numeric_operands(_v4, _v5);
// REWRITES-NEXT: let _v7: i32 = gnu_multiple_outputs();
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v1 as *const i8, _v3, _v6, _v7) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
