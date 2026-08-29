#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;
static int     failures = 0;

static void record_failure(const char *phase) {
  failures++;
  printf("FAIL: %s\n", phase);
}

static void inner_check(int ok) {
  if (!ok) {
    longjmp(env, 1);
  }
}

static void run_case(int id, int should_fail) {
  if (setjmp(env)) {
    record_failure("case");
    return;
  }
  inner_check(!should_fail);
  printf("PASS: case %d\n", id);
}

int main(void) {
  for (int i = 0; i < 4; i++) {
    if (setjmp(env)) {
      record_failure("loop");
      continue;
    }
    run_case(i, i == 2);
  }
  printf("failures: %d\n", failures);
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
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn record_failure(arg3: *mut i8) {
// LOWERING-NEXT:     let mut phase: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     phase = arg3;
// LOWERING-NEXT:     let _v0: i32 = unsafe { failures };
// LOWERING-NEXT:     let _v1: i32 = _v0 + 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         failures = _v1;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v2: *mut i8 = b"FAIL: %s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut i8 = phase;
// LOWERING-NEXT:     let _v4: i32 = unsafe { printf(_v2 as *const i8, _v3) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn inner_check(arg2: i32) {
// LOWERING-NEXT:     let mut ok: i32 = 0;
// LOWERING-NEXT:     ok = arg2;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v0: i32 = ok;
// LOWERING-NEXT:         let _v1: bool = _v0 != 0;
// LOWERING-NEXT:         let _v2: bool = !_v1;
// LOWERING-NEXT:         if _v2 {
// LOWERING-NEXT:             let _v3: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:             let _v4: i32 = 1;
// LOWERING-NEXT:             unsafe { longjmp(_v3 as *mut __slate_jmp_buf_tag, _v4 as i32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn run_case(arg0: i32, arg1: i32) {
// LOWERING-NEXT:     let mut id: i32 = 0;
// LOWERING-NEXT:     let mut should_fail: i32 = 0;
// LOWERING-NEXT:     id = arg0;
// LOWERING-NEXT:     should_fail = arg1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v0: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:         let _v1: i32 = unsafe { setjmp(_v0 as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:         let _v2: bool = _v1 != 0;
// LOWERING-NEXT:         if _v2 {
// LOWERING-NEXT:             let _v3: *mut i8 = b"case\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             record_failure(_v3);
// LOWERING-NEXT:             return;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v4: i32 = should_fail;
// LOWERING-NEXT:     let _v5: bool = _v4 != 0;
// LOWERING-NEXT:     let _v6: bool = !_v5;
// LOWERING-NEXT:     let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:     inner_check(_v7);
// LOWERING-NEXT:     let _v8: *mut i8 = b"PASS: case %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v9: i32 = id;
// LOWERING-NEXT:     let _v10: i32 = unsafe { printf(_v8 as *const i8, _v9) };
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
// LOWERING-NEXT:             let _v3: i32 = 4;
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
// LOWERING-NEXT:                             let _v8: *mut i8 = b"loop\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                             record_failure(_v8);
// LOWERING-NEXT:                             break '__continue0;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let _v9: i32 = i;
// LOWERING-NEXT:                     let _v10: i32 = i;
// LOWERING-NEXT:                     let _v11: i32 = 2;
// LOWERING-NEXT:                     let _v12: bool = _v10 == _v11;
// LOWERING-NEXT:                     let _v13: i32 = _v12 as i32;
// LOWERING-NEXT:                     run_case(_v9, _v13);
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v14: i32 = i;
// LOWERING-NEXT:             let _v15: i32 = _v14 + 1;
// LOWERING-NEXT:             i = _v15;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v16: *mut i8 = b"failures: %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v17: i32 = unsafe { failures };
// LOWERING-NEXT:     let _v18: i32 = unsafe { printf(_v16 as *const i8, _v17) };
// LOWERING-NEXT:     let _v19: i32 = 0;
// LOWERING-NEXT:     __retval = _v19;
// LOWERING-NEXT:     let _v20: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v20 as i32);
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
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn record_failure(arg3: *mut i8) {
// REWRITES-NEXT: let mut phase: *mut i8 = arg3;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         failures = (unsafe { failures }) + 1;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v2: *mut i8 = b"FAIL: %s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: i32 = unsafe { printf(_v2 as *const i8, phase) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn inner_check(arg2: i32) {
// REWRITES-NEXT: let mut ok: i32 = arg2;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v1: bool = ok != 0;
// REWRITES-NEXT:         let _v2: bool = !_v1;
// REWRITES-NEXT:         if _v2 {
// REWRITES-NEXT:                     let _v3: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                     let _v4: i32 = 1;
// REWRITES-NEXT:                     unsafe { longjmp(_v3 as *mut __slate_jmp_buf_tag, _v4 as i32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn run_case(arg0: i32, arg1: i32) {
// REWRITES-NEXT: let mut id: i32 = arg0;
// REWRITES-NEXT: let mut should_fail: i32 = arg1;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v0: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:         let _v1: i32 = unsafe { setjmp(_v0 as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:         let _v2: bool = _v1 != 0;
// REWRITES-NEXT:         if _v2 {
// REWRITES-NEXT:                     let _v3: *mut i8 = b"case\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     record_failure(_v3);
// REWRITES-NEXT:                     return;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v5: bool = should_fail != 0;
// REWRITES-NEXT: let _v6: bool = !_v5;
// REWRITES-NEXT: inner_check(_v6 as i32);
// REWRITES-NEXT: let _v8: *mut i8 = b"PASS: case %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: i32 = unsafe { printf(_v8 as *const i8, id) };
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
// REWRITES-NEXT:                     let _v3: i32 = 4;
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
// REWRITES-NEXT:                                                                                                             let _v8: *mut i8 = b"loop\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                                                                                                             record_failure(_v8);
// REWRITES-NEXT:                                                                                                             break '__continue0;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                                         let _v11: i32 = 2;
// REWRITES-NEXT:                                                         run_case(i, (i == _v11) as i32);
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v16: *mut i8 = b"failures: %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: i32 = unsafe { printf(_v16 as *const i8, unsafe { failures }) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
