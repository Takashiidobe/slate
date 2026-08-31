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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-NEXT: fn might_fail({{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut attempt: i32 = 0;
// LOWERING-NEXT:     attempt = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = attempt;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = attempt;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut attempt: i32 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:                 attempt = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = b"attempt %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = attempt;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = attempt;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         break;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = attempt;
// LOWERING-NEXT:                 might_fail({{_v[0-9]+}});
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-NEXT: fn might_fail({{arg[0-9]+}}: i32) {
// REWRITES-NEXT: let mut attempt: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = attempt < {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:                     unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, (attempt + {{_v[0-9]+}}) as i32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: loop {
// REWRITES-NEXT:         if !true {
// REWRITES-NEXT:                     break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let mut attempt: i32 = 0;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:                     attempt = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b"attempt %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     unsafe { printf({{_v[0-9]+}} as *const i8, attempt) };
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:                                     if attempt >= {{_v[0-9]+}} {
// REWRITES-NEXT:                                                         break;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     might_fail(attempt);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
