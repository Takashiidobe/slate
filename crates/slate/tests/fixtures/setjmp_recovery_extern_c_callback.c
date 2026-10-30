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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _setjmp([[ARG_0:arg[0-9]+]]: *mut __jmp_buf_tag) -> i32;
// CHECK-NEXT:     fn longjmp([[ARG_0]]: *mut __jmp_buf_tag, [[ARG_1:arg[0-9]+]]: i32);
// CHECK-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord___sigset_t_h_8 {
// CHECK-NEXT:     __val: [u64; 16],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __jmp_buf_tag {
// CHECK-NEXT:     __jmpbuf: [i64; 8],
// CHECK-NEXT:     __mask_was_saved: i32,
// CHECK-NEXT:     __saved_mask: __SlateRecord___sigset_t_h_8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut env: __SlateAlign16<[__jmp_buf_tag; 1]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut failures: i32 = 0;
// CHECK-NEXT: fn fail_now() {
// CHECK-NEXT:     unsafe { longjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag, 1i32) };
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn check(ok: i32) {
// CHECK-NEXT:     if ok == 0 {
// CHECK-NEXT:         fail_now();
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("PASS");
// CHECK-NEXT: }
// CHECK-NEXT: fn run_case(r#fn: Option<unsafe extern "C-unwind" fn(i32)>, ok: i32) {
// CHECK-NEXT:     if (unsafe { _setjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag) }) != 0 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             failures = failures + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("FAIL");
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { r#fn.unwrap()(ok) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let r#fn: Option<unsafe extern "C-unwind" fn(i32)> = Some(check);
// CHECK-NEXT:     run_case(r#fn, 0i32);
// CHECK-NEXT:     println!("failures: {}", unsafe { failures });
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
