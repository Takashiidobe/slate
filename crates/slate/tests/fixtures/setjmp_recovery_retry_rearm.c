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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct __slate_jmp_buf_tag {
// COMMON-LOWERING-NEXT:     __regs: [i64; 8],
// COMMON-LOWERING-NEXT:     __mask_was_saved: i32,
// COMMON-LOWERING-NEXT:     __saved_mask: [u64; 16],
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT:     [__slate_jmp_buf_tag {
// COMMON-LOWERING-NEXT:         __regs: [0; 8],
// COMMON-LOWERING-NEXT:         __mask_was_saved: 0,
// COMMON-LOWERING-NEXT:         __saved_mask: [0; 16],
// COMMON-LOWERING-NEXT:     }; 1],
// COMMON-LOWERING-NEXT: );
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let mut attempt: i32 = 0;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag =
// COMMON-LOWERING-NEXT:                     std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { setjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// COMMON-LOWERING-NEXT:                 attempt = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = attempt;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = attempt;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         break;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = attempt;
// COMMON-LOWERING-NEXT:                 might_fail({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn might_fail({{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let mut attempt: i32 = 0;
// COMMON-LOWERING-NEXT:     attempt = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = attempt;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag =
// COMMON-LOWERING-NEXT:                 std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = attempt;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             unsafe { longjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag, {{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: static mut retry_buf: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = b"attempt %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: static mut retry_buf: aligned::Aligned<aligned::A8, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = b"attempt %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct __slate_jmp_buf_tag {
// COMMON-REWRITES-NEXT:     __regs: [i64; 8],
// COMMON-REWRITES-NEXT:     __mask_was_saved: i32,
// COMMON-REWRITES-NEXT:     __saved_mask: [u64; 16],
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT:     [__slate_jmp_buf_tag {
// COMMON-REWRITES-NEXT:         __regs: [0; 8],
// COMMON-REWRITES-NEXT:         __mask_was_saved: 0,
// COMMON-REWRITES-NEXT:         __saved_mask: [0; 16],
// COMMON-REWRITES-NEXT:     }; 1],
// COMMON-REWRITES-NEXT: );
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     while true {
// COMMON-REWRITES-NEXT:         let mut attempt: i32 = 0;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag =
// COMMON-REWRITES-NEXT:             std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>();
// COMMON-REWRITES-NEXT:         attempt = unsafe { setjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// COMMON-REWRITES-NEXT:         unsafe { printf(c"attempt %d\n".as_ptr(), attempt) };
// COMMON-REWRITES-NEXT:         if attempt >= 3 {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         might_fail(attempt);
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn might_fail(mut attempt: i32) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = attempt < 3;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = attempt + 1;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             longjmp(
// COMMON-REWRITES-NEXT:                 std::ptr::addr_of_mut!(retry_buf).cast::<__slate_jmp_buf_tag>()
// COMMON-REWRITES-NEXT:                     as *mut __slate_jmp_buf_tag,
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} as i32,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: static mut retry_buf: aligned::Aligned<aligned::A16, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: static mut retry_buf: aligned::Aligned<aligned::A8, [__slate_jmp_buf_tag; 1]> = aligned::Aligned(
// SLATE-FILECHECK-END rewrites-aarch64-gnu
