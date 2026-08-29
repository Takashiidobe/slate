#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;
static int     failures          = 0;
static int     teardown_failures = 0;

static void run_test(int i) {
  if (i == 2) {
    longjmp(env, 1);
  }
  printf("ran %d\n", i);
}

int main(void) {
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered %d\n", i);
      continue;
    }
    run_test(i);
    if (i == 3) {
      teardown_failures++;
      continue;
    }
    printf("teardown ok %d\n", i);
  }
  printf("failures=%d teardown_failures=%d\n", failures, teardown_failures);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __slate_jmp_buf_tag {
// LOWERING-NEXT:     __regs: [i64; 8],
// LOWERING-NEXT:     __mask_was_saved: i32,
// LOWERING-NEXT:     __saved_mask: [u64; 16],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut env: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned([__slate_jmp_buf_tag { __regs: [0; 8], __mask_was_saved: 0, __saved_mask: [0; 16] }; 1]);
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut failures: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut teardown_failures: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn run_test(arg0: i32) {
// LOWERING-NEXT:     let mut i: i32 = 0;
// LOWERING-NEXT:     i = arg0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v0: i32 = i;
// LOWERING-NEXT:         let _v1: i32 = 2;
// LOWERING-NEXT:         let _v2: bool = _v0 == _v1;
// LOWERING-NEXT:         if _v2 {
// LOWERING-NEXT:             let _v3: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:             let _v4: i32 = 1;
// LOWERING-NEXT:             unsafe { longjmp(_v3 as *mut __slate_jmp_buf_tag, _v4 as i32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v5: *mut i8 = b"ran %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i32 = i;
// LOWERING-NEXT:     let _v7: i32 = unsafe { printf(_v5 as *const i8, _v6) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let _v1: i32 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         '__loop0: loop {
// LOWERING-NEXT:             let _v2: i32 = i;
// LOWERING-NEXT:             let _v3: i32 = 5;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             '__continue0: {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let _v5: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:                         let _v6: i32 = unsafe { setjmp(_v5 as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:                         let _v7: bool = _v6 != 0;
// LOWERING-NEXT:                         if _v7 {
// LOWERING-NEXT:                             let _v8: i32 = unsafe { failures };
// LOWERING-NEXT:                             let _v9: i32 = _v8 + 1;
// LOWERING-NEXT:                             unsafe {
// LOWERING-NEXT:                                 failures = _v9;
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                             let _v10: *mut i8 = b"recovered %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                             let _v11: i32 = i;
// LOWERING-NEXT:                             let _v12: i32 = unsafe { printf(_v10 as *const i8, _v11) };
// LOWERING-NEXT:                             break '__continue0;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let _v13: i32 = i;
// LOWERING-NEXT:                     run_test(_v13);
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let _v14: i32 = i;
// LOWERING-NEXT:                         let _v15: i32 = 3;
// LOWERING-NEXT:                         let _v16: bool = _v14 == _v15;
// LOWERING-NEXT:                         if _v16 {
// LOWERING-NEXT:                             let _v17: i32 = unsafe { teardown_failures };
// LOWERING-NEXT:                             let _v18: i32 = _v17 + 1;
// LOWERING-NEXT:                             unsafe {
// LOWERING-NEXT:                                 teardown_failures = _v18;
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                             break '__continue0;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let _v19: *mut i8 = b"teardown ok %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                     let _v20: i32 = i;
// LOWERING-NEXT:                     let _v21: i32 = unsafe { printf(_v19 as *const i8, _v20) };
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v22: i32 = i;
// LOWERING-NEXT:             let _v23: i32 = _v22 + 1;
// LOWERING-NEXT:             i = _v23;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v24: *mut i8 = b"failures=%d teardown_failures=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: i32 = unsafe { failures };
// LOWERING-NEXT:     let _v26: i32 = unsafe { teardown_failures };
// LOWERING-NEXT:     let _v27: i32 = unsafe { printf(_v24 as *const i8, _v25, _v26) };
// LOWERING-NEXT:     let _v28: i32 = 0;
// LOWERING-NEXT:     __retval = _v28;
// LOWERING-NEXT:     let _v29: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v29 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __slate_jmp_buf_tag {
// REWRITES-NEXT:     __regs: [i64; 8],
// REWRITES-NEXT:     __mask_was_saved: i32,
// REWRITES-NEXT:     __saved_mask: [u64; 16],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut env: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned([__slate_jmp_buf_tag { __regs: [0; 8], __mask_was_saved: 0, __saved_mask: [0; 16] }; 1]);
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut failures: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut teardown_failures: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn run_test(arg0: i32) {
// REWRITES-NEXT: let mut i: i32 = arg0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v1: i32 = 2;
// REWRITES-NEXT:         let _v2: bool = i == _v1;
// REWRITES-NEXT:         if _v2 {
// REWRITES-NEXT:                     let _v3: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                     let _v4: i32 = 1;
// REWRITES-NEXT:                     unsafe { longjmp(_v3 as *mut __slate_jmp_buf_tag, _v4 as i32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v5: *mut i8 = b"ran %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = unsafe { printf(_v5 as *const i8, i) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         '__loop0: loop {
// REWRITES-NEXT:                     let _v3: i32 = 5;
// REWRITES-NEXT:                     if !(i < _v3) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     '__continue0: {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 let _v5: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                                                                                 let _v6: i32 = unsafe { setjmp(_v5 as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:                                                                                 let _v7: bool = _v6 != 0;
// REWRITES-NEXT:                                                                                 if _v7 {
// REWRITES-NEXT:                                                                                                             unsafe {
// REWRITES-NEXT:                                                                                                                                             failures = (unsafe { failures }) + 1;
// REWRITES-NEXT:                                                                                                             }
// REWRITES-NEXT:                                                                                                             let _v10: *mut i8 = b"recovered %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                                             let _v12: i32 = unsafe { printf(_v10 as *const i8, i) };
// REWRITES-NEXT:                                                                                                             break '__continue0;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                                         run_test(i);
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 let _v15: i32 = 3;
// REWRITES-NEXT:                                                                                 if i == _v15 {
// REWRITES-NEXT:                                                                                                             unsafe {
// REWRITES-NEXT:                                                                                                                                             teardown_failures = (unsafe { teardown_failures }) + 1;
// REWRITES-NEXT:                                                                                                             }
// REWRITES-NEXT:                                                                                                             break '__continue0;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                                         let _v19: *mut i8 = b"teardown ok %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                         let _v21: i32 = unsafe { printf(_v19 as *const i8, i) };
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v24: *mut i8 = b"failures=%d teardown_failures=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v27: i32 = unsafe { printf(_v24 as *const i8, unsafe { failures }, unsafe { teardown_failures }) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
