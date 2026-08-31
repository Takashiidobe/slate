#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;
static int     failures = 0;

static void fail_now(void) { longjmp(env, 1); }

static void check(int ok) {
  if (!ok) {
    fail_now();
  }
  printf("PASS\n");
}

static void run_case(void (*fn)(int), int ok) {
  if (setjmp(env)) {
    failures++;
    printf("FAIL\n");
    return;
  }
  fn(ok);
}

int main(void) {
  void (*fn)(int) = check;
  run_case(fn, 0);
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
// LOWERING-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// LOWERING-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fail_now() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn check({{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut ok: i32 = 0;
// LOWERING-NEXT:     ok = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = ok;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             fail_now();
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"PASS\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn run_case({{arg[0-9]+}}: Option<unsafe extern "C" fn(i32)>, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 failures = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"FAIL\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// LOWERING-NEXT:             return;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     run_case(Some(check), {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"failures: %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT:     fn longjmp(_0: *mut __slate_jmp_buf_tag, _1: i32) -> !;
// REWRITES-NEXT:     fn setjmp(_0: *mut __slate_jmp_buf_tag) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fail_now() {
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: unsafe { longjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag, {{_v[0-9]+}} as i32) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn check({{arg[0-9]+}}: i32) {
// REWRITES-NEXT: let mut ok: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = ok != 0;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     fail_now();
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"PASS\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn run_case({{arg[0-9]+}}: Option<unsafe extern "C" fn(i32)>, {{arg[0-9]+}}: i32) {
// REWRITES-NEXT: {
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut __slate_jmp_buf_tag = std::ptr::addr_of_mut!(env).cast::<__slate_jmp_buf_tag>();
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { setjmp({{_v[0-9]+}} as *mut __slate_jmp_buf_tag) };
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:                     unsafe {
// REWRITES-NEXT:                                     failures = (unsafe { failures }) + 1;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b"FAIL\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8) };
// REWRITES-NEXT:                     return;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: run_case(Some(check), {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"failures: %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, unsafe { failures }) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
