#include <setjmp.h>
#include <stdio.h>

static jmp_buf retry_buf;

static void might_fail(int attempt) {
  if (attempt < 3) {
    longjmp(retry_buf, attempt + 1);
  }
}

int main(void) {
  for (;;) {
    int attempt = setjmp(retry_buf);
    printf("attempt %d\n", attempt);
    if (attempt >= 3) {
      break;
    }
    might_fail(attempt);
  }
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
// LOWERING-NEXT: static mut retry_buf: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned([__slate_jmp_buf_tag { __regs: [0; 8], __mask_was_saved: 0, __saved_mask: [0; 16] }; 1]);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn might_fail(arg0: i32) {
// LOWERING-NEXT:     let mut attempt: i32 = 0;
// LOWERING-NEXT:     attempt = arg0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v0: i32 = attempt;
// LOWERING-NEXT:         let _v1: i32 = 3;
// LOWERING-NEXT:         let _v2: bool = _v0 < _v1;
// LOWERING-NEXT:         if _v2 {
// LOWERING-NEXT:             let _v3: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:             let _v4: i32 = attempt;
// LOWERING-NEXT:             let _v5: i32 = 1;
// LOWERING-NEXT:             let _v6: i32 = _v4 + _v5;
// LOWERING-NEXT:             unsafe { longjmp(_v3 as *mut __slate_jmp_buf_tag, _v6 as i32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v1: bool = true;
// LOWERING-NEXT:             if !_v1 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut attempt: i32 = 0;
// LOWERING-NEXT:                 let _v2: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:                 let _v3: i32 = unsafe { setjmp(_v2 as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:                 attempt = _v3;
// LOWERING-NEXT:                 let _v4: *mut i8 = b"attempt %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let _v5: i32 = attempt;
// LOWERING-NEXT:                 let _v6: i32 = unsafe { printf(_v4 as *const i8, _v5) };
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let _v7: i32 = attempt;
// LOWERING-NEXT:                     let _v8: i32 = 3;
// LOWERING-NEXT:                     let _v9: bool = _v7 >= _v8;
// LOWERING-NEXT:                     if _v9 {
// LOWERING-NEXT:                         break;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let _v10: i32 = attempt;
// LOWERING-NEXT:                 might_fail(_v10);
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v11: i32 = 0;
// LOWERING-NEXT:     __retval = _v11;
// LOWERING-NEXT:     let _v12: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v12 as i32);
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
// REWRITES-NEXT: static mut retry_buf: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned([__slate_jmp_buf_tag { __regs: [0; 8], __mask_was_saved: 0, __saved_mask: [0; 16] }; 1]);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn might_fail(arg0: i32) {
// REWRITES-NEXT: let mut attempt: i32 = arg0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v1: i32 = 3;
// REWRITES-NEXT:         let _v2: bool = attempt < _v1;
// REWRITES-NEXT:         if _v2 {
// REWRITES-NEXT:                     let _v3: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                     let _v5: i32 = 1;
// REWRITES-NEXT:                     unsafe { longjmp(_v3 as *mut __slate_jmp_buf_tag, (attempt + _v5) as i32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         if !true {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let mut attempt: i32 = 0;
// REWRITES-NEXT:                     let _v2: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                     attempt = unsafe { setjmp(_v2 as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:                     let _v4: *mut i8 = b"attempt %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let _v6: i32 = unsafe { printf(_v4 as *const i8, attempt) };
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v8: i32 = 3;
// REWRITES-NEXT:                                     if attempt >= _v8 {
// REWRITES-NEXT:                                                         break;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     might_fail(attempt);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
