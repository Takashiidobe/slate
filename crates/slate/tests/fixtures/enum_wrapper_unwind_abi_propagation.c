#include <setjmp.h>
#include <stdio.h>

enum Status { STATUS_OK, STATUS_FAIL };
typedef enum Status Processor(int x);
typedef void        Callback(int x);

struct Dispatcher {
  Processor *run;
};

static jmp_buf   env;
static int       failures = 0;
static Callback *g_callback;

static enum Status risky(int x) {
  if (x == 3) {
    longjmp(env, 1);
  }
  return STATUS_OK;
}

static void panicky_callback(int x) {
  if (x == 2) {
    longjmp(env, 1);
  }
  printf("callback %d\n", x);
}

static enum Status content_like(int x) {
  g_callback(x);
  return STATUS_OK;
}

int main(void) {
  struct Dispatcher d;
  g_callback = panicky_callback;

  d.run = risky;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered risky %d\n", i);
      continue;
    }
    d.run(i);
  }

  d.run = content_like;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered content_like %d\n", i);
      continue;
    }
    d.run(i);
  }

  printf("failures=%d\n", failures);
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Dispatcher {
// CHECK-NEXT:     run: Option<unsafe extern "C-unwind" fn(i32) -> u32>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut env: __SlateAlign16<[__jmp_buf_tag; 1]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: static mut failures: i32 = 0 as i32;
// CHECK-NEXT: static mut g_callback: Option<unsafe extern "C-unwind" fn(i32)> = unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: };
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.risky")]
// CHECK-NEXT: extern "C-unwind" fn risky(x: i32) -> u32 {
// CHECK-NEXT:     if x == 3 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             longjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag, 1 as i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn panicky_callback(x: i32) {
// CHECK-NEXT:     if x == 2 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             longjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag, 1 as i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("callback {}", x);
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.content_like")]
// CHECK-NEXT: extern "C-unwind" fn content_like(x: i32) -> u32 {
// CHECK-NEXT:     unsafe { unsafe { g_callback }.unwrap()(x) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut d: Dispatcher = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_callback = Some(panicky_callback);
// CHECK-NEXT:     }
// CHECK-NEXT:     d.run = Some(risky);
// CHECK-NEXT:     for i in 0 as i32..5 as i32 {
// CHECK-NEXT:         if (unsafe { _setjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag) }) != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 failures = failures + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             println!("recovered risky {}", i);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe { d.run.unwrap()(i) };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     d.run = Some(content_like);
// CHECK-NEXT:     for i in 0 as i32..5 as i32 {
// CHECK-NEXT:         if (unsafe { _setjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag) }) != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 failures = failures + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             println!("recovered content_like {}", i);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe { d.run.unwrap()(i) };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("failures={}", unsafe { failures });
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
