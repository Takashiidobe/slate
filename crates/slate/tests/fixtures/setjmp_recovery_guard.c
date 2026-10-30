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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _setjmp([[ARG_0:arg[0-9]+]]: *mut __jmp_buf_tag) -> i32;
// CHECK-NEXT:     fn longjmp([[ARG_0]]: *mut __jmp_buf_tag, [[ARG_1:arg[0-9]+]]: i32);
// CHECK-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
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
// CHECK-NEXT: fn record_failure(phase: *const i8) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         failures = failures + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"FAIL: %s\n".as_ptr(), phase) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn inner_check(ok: i32) {
// CHECK-NEXT:     if ok == 0 {
// CHECK-NEXT:         unsafe { longjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag, 1i32) };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn run_case(id: i32, should_fail: i32) {
// CHECK-NEXT:     if (unsafe { _setjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag) }) != 0 {
// CHECK-NEXT:         record_failure(c"case".as_ptr());
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     inner_check((should_fail == 0) as i32);
// CHECK-NEXT:     println!("PASS: case {}", id);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         if (unsafe { _setjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag) }) != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             record_failure(c"loop".as_ptr());
// CHECK-NEXT:             continue;
// CHECK-NEXT:         }
// CHECK-NEXT:         run_case(i, (i == 2) as i32);
// CHECK-NEXT:     }
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
