#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_DEPTH 8

static jmp_buf *jb_stack;
static int      jb_top = 0;

#define TRY if (setjmp(jb_stack[jb_top++]) == 0)
#define CATCH else
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
  CATCH {
    printf("case %d: caught\n", id);
  }
}

int main(void) {
  jb_stack = malloc(sizeof(jmp_buf) * MAX_DEPTH);
  run_case(0, 0);
  run_case(1, 1);
  run_case(2, 0);
  free(jb_stack);
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
// LOWERING-NEXT: static mut jb_stack: *mut [__slate_jmp_buf_tag; 1] = std::ptr::null_mut();
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut jb_top: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn inner({{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut fail: i32 = 0;
// LOWERING-NEXT:     fail = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = fail;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { jb_top };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - 1;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 jb_top = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = {{_v[0-9]+}} as *mut __slate_jmp_buf_tag;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:             unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn run_case({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut id: i32 = 0;
// LOWERING-NEXT:     let mut fail: i32 = 0;
// LOWERING-NEXT:     id = {{arg[0-9]+}};
// LOWERING-NEXT:     fail = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { jb_top };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:         unsafe {
// LOWERING-NEXT:             jb_top = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = {{_v[0-9]+}} as *mut __slate_jmp_buf_tag;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = fail;
// LOWERING-NEXT:             inner({{_v[0-9]+}});
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"case %d: no exception\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = id;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"case %d: caught\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = id;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 200;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = {{_v[0-9]+}} as *mut [__slate_jmp_buf_tag; 1];
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         jb_stack = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     run_case({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     run_case({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     run_case({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
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
// REWRITES-NEXT: static mut jb_stack: *mut [__slate_jmp_buf_tag; 1] = std::ptr::null_mut();
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut jb_top: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn inner({{arg[0-9]+}}: i32) {
// REWRITES-NEXT: let mut fail: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = fail != 0;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = (unsafe { jb_top }) - 1;
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     jb_top = {{_v[0-9]+}};
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { {{_v[0-9]+}}.offset(({{_v[0-9]+}} as i64) as isize) };
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 42;
// REWRITES-NEXT:                     unsafe { longjmp(({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn run_case({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) {
// REWRITES-NEXT: let mut id: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut fail: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { jb_top };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:                     jb_top = {{_v[0-9]+}} + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { jb_stack };
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut [__slate_jmp_buf_tag; 1] = unsafe { {{_v[0-9]+}}.offset(({{_v[0-9]+}} as i64) as isize) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { setjmp(({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     inner(fail);
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b"case %d: no exception\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     unsafe { printf({{_v[0-9]+}} as *const i8, id) };
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b"case %d: caught\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     unsafe { printf({{_v[0-9]+}} as *const i8, id) };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 200;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc(({{_v[0-9]+}} * {{_v[0-9]+}}) as usize) };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         jb_stack = {{_v[0-9]+}} as *mut [__slate_jmp_buf_tag; 1];
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: run_case({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: run_case({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: run_case({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT: unsafe { free(((unsafe { jb_stack }) as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
