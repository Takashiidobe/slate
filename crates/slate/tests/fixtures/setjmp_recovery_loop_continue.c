#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;
static int     failures          = 0;
static int     teardown_failures = 0;

static void run_test(int i) {
  if (i == 2) {
    longjmp(env, 1);
  }
  printf("ran %d\n", i);
}

int main(void) {
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered %d\n", i);
      continue;
    }
    run_test(i);
    if (i == 3) {
      teardown_failures++;
      continue;
    }
    printf("teardown ok %d\n", i);
  }
  printf("failures=%d teardown_failures=%d\n", failures, teardown_failures);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _setjmp(__env: *mut __jmp_buf_tag) -> i32;
// CHECK-NEXT:     fn longjmp(__env: *mut __jmp_buf_tag, __val: i32);
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __val: [u64; 16],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __jmp_buf_tag {
// CHECK-NEXT:     __jmpbuf: [i64; 8],
// CHECK-NEXT:     __mask_was_saved: i32,
// CHECK-NEXT:     __saved_mask: [[SLATERECORD_0]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut env: __SlateAlign16<[__jmp_buf_tag; 1]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut failures: i32 = 0;
// CHECK-NEXT: static mut teardown_failures: i32 = 0;
// CHECK-NEXT: fn run_test(i: i32) {
// CHECK-NEXT:     if i == 2 {
// CHECK-NEXT:         unsafe { longjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag, 1i32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("ran {}", i);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0i32..5i32 {
// CHECK-NEXT:         if (unsafe { _setjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag) }) != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 failures = failures + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             println!("recovered {}", i);
// CHECK-NEXT:             continue;
// CHECK-NEXT:         }
// CHECK-NEXT:         run_test(i);
// CHECK-NEXT:         if i == 3 {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 teardown_failures = teardown_failures + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             continue;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("teardown ok {}", i);
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "failures={} teardown_failures={}", unsafe { failures }, unsafe {
// CHECK-NEXT:         teardown_failures }
// CHECK-NEXT:     );
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
