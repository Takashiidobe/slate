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
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __slate_jmp_buf_tag {
// LOWERING-NEXT:     __regs: [i64; 8],
// LOWERING-NEXT:     __mask_was_saved: i32,
// LOWERING-NEXT:     __saved_mask: [u64; 16],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut retry_buf: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// LOWERING-NEXT:     [__slate_jmp_buf_tag {
// LOWERING-NEXT:         __regs: [0; 8],
// LOWERING-NEXT:         __mask_was_saved: 0,
// LOWERING-NEXT:         __saved_mask: [0; 16],
// LOWERING-NEXT:     }; 1],
// LOWERING-NEXT: );
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
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
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag =
// LOWERING-NEXT:                     std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:                 attempt = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = b"attempt %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = attempt;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
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
// LOWERING-EMPTY:
// LOWERING-NEXT: fn might_fail({{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut attempt: i32 = 0;
// LOWERING-NEXT:     attempt = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = attempt;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag =
// LOWERING-NEXT:                 std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = attempt;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:             unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __slate_jmp_buf_tag {
// REWRITES-NEXT:     __regs: [i64; 8],
// REWRITES-NEXT:     __mask_was_saved: i32,
// REWRITES-NEXT:     __saved_mask: [u64; 16],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut retry_buf: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// REWRITES-NEXT:     [__slate_jmp_buf_tag {
// REWRITES-NEXT:         __regs: [0; 8],
// REWRITES-NEXT:         __mask_was_saved: 0,
// REWRITES-NEXT:         __saved_mask: [0; 16],
// REWRITES-NEXT:     }; 1],
// REWRITES-NEXT: );
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     while true {
// REWRITES-NEXT:         let mut attempt: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag =
// REWRITES-NEXT:             std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:         attempt = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:         unsafe { printf(c"attempt %d\n".as_ptr(), attempt) };
// REWRITES-NEXT:         if attempt >= 3 {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         might_fail(attempt);
// REWRITES-NEXT:     }
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn might_fail(mut attempt: i32) {
// REWRITES-NEXT:     let {{_v[0-9]+}}: bool = attempt < 3;
// REWRITES-NEXT:     if {{_v[0-9]+}} {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = attempt + 1;
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             longjmp(
// REWRITES-NEXT:                 std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>()
// REWRITES-NEXT:                     as *mut __slate_jmp_buf_tag,
// REWRITES-NEXT:                 {{_v[0-9]+}} as i32,
// REWRITES-NEXT:             )
// REWRITES-NEXT:         };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
