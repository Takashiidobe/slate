#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_DEPTH 8

static jmp_buf *jb_stack;
static int      jb_top = 0;

#define TRY      if (setjmp(jb_stack[jb_top++]) == 0)
#define CATCH    else
#define THROW(v) longjmp(jb_stack[--jb_top], (v))

static void inner(int fail) {
  if (fail) {
    THROW(42);
  }
}

static void run_case(int id, int fail) {
  TRY {
    inner(fail);
    printf("case %d: no exception\n", id);
  }
  CATCH { printf("case %d: caught\n", id); }
}

int main(void) {
  jb_stack = malloc(sizeof(jmp_buf) * MAX_DEPTH);
  run_case(0, 0);
  run_case(1, 1);
  run_case(2, 0);
  free(jb_stack);
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
// COMMON-LOWERING-NEXT: static mut jb_stack: *mut [__slate_jmp_buf_tag; 1] = std::ptr::null_mut();
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut jb_top: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 200;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = {{__v[0-9]+}} as *mut [__slate_jmp_buf_tag; 1];
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         jb_stack = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     run_case({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     run_case({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     run_case({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn run_case({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let mut id: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut fail: i32 = 0;
// COMMON-LOWERING-NEXT:     id = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     fail = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { jb_top };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:         unsafe {
// COMMON-LOWERING-NEXT:             jb_top = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag = {{__v[0-9]+}} as *mut __slate_jmp_buf_tag;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = unsafe { setjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = fail;
// COMMON-LOWERING-NEXT:             inner({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = id;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = id;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn inner({{arg[0-9]+}}: i32) {
// COMMON-LOWERING-NEXT:     let mut fail: i32 = 0;
// COMMON-LOWERING-NEXT:     fail = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = fail;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = unsafe { jb_top };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - 1;
// COMMON-LOWERING-NEXT:             unsafe {
// COMMON-LOWERING-NEXT:                 jb_top = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: *mut __slate_jmp_buf_tag = {{__v[0-9]+}} as *mut __slate_jmp_buf_tag;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:             unsafe { longjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag, {{__v[0-9]+}} as i32) };
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"case %d: no exception\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{__v[0-9]+}}: *mut i8 = b"case %d: caught\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"case %d: no exception\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:             let {{__v[0-9]+}}: *mut u8 = b"case %d: caught\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: static mut jb_stack: *mut [__slate_jmp_buf_tag; 1] = std::ptr::null_mut();
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut jb_top: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// COMMON-REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = 200 * {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         jb_stack = {{__v[0-9]+}} as *mut [__slate_jmp_buf_tag; 1];
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     run_case(0, 0);
// COMMON-REWRITES-NEXT:     run_case(1, 1);
// COMMON-REWRITES-NEXT:     run_case(2, 0);
// COMMON-REWRITES-NEXT:     unsafe { free((unsafe { jb_stack }) as *mut core::ffi::c_void) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn run_case(mut id: i32, mut fail: i32) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { jb_top };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         jb_top = {{__v[0-9]+}} + 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { {{__v[0-9]+}}.offset(({{__v[0-9]+}} as i64) as isize) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { setjmp({{__v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         inner(fail);
// COMMON-REWRITES-NEXT:         unsafe { printf(c"case %d: no exception\n".as_ptr(), id) };
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { printf(c"case %d: caught\n".as_ptr(), id) };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn inner(mut {{__v[0-9]+}}: i32) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = (unsafe { jb_top }) - 1;
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             jb_top = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             longjmp(
// COMMON-REWRITES-NEXT:                 (unsafe { {{__v[0-9]+}}.offset(({{__v[0-9]+}} as i64) as isize) }) as *mut __slate_jmp_buf_tag,
// COMMON-REWRITES-NEXT:                 42 as i32,
// COMMON-REWRITES-NEXT:             )
// COMMON-REWRITES-NEXT:         };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
