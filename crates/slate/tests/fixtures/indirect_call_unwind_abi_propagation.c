#include <setjmp.h>
#include <stdio.h>

typedef void Callback(int x);
typedef void Dispatch(int x, int y);

struct Ctx {
  Dispatch *run;
};

static jmp_buf   env;
static int       failures = 0;
static Callback *g_callback;

static void quiet_callback(int x) { printf("quiet %d\n", x); }

static void panicky_callback(int x) {
  if (x == 2) {
    longjmp(env, 1);
  }
  printf("panicky %d\n", x);
}

static void dispatcher(int x, int y) {
  (void)y;
  g_callback(x);
}

int main(void) {
  struct Ctx c;
  c.run = dispatcher;

  g_callback = quiet_callback;
  for (int i = 0; i < 2; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered quiet %d\n", i);
      continue;
    }
    c.run(i, 0);
  }

  g_callback = panicky_callback;
  for (int i = 0; i < 5; i++) {
    if (setjmp(env)) {
      failures++;
      printf("recovered panicky %d\n", i);
      continue;
    }
    c.run(i, 0);
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
// CHECK-NEXT: struct Ctx {
// CHECK-NEXT:     run: Option<unsafe extern "C-unwind" fn(i32, i32)>,
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
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.quiet_callback")]
// CHECK-NEXT: extern "C-unwind" fn quiet_callback(x: i32) {
// CHECK-NEXT:     println!("quiet {}", x);
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.panicky_callback")]
// CHECK-NEXT: extern "C-unwind" fn panicky_callback(x: i32) {
// CHECK-NEXT:     if x == 2 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             longjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag, 1 as i32)
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("panicky {}", x);
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn dispatcher(x: i32, y: i32) {
// CHECK-NEXT:     unsafe { unsafe { g_callback }.unwrap()(x) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: Ctx = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     c.run = Some(dispatcher);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_callback = Some(quiet_callback);
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0 as i32..2 as i32 {
// CHECK-NEXT:         if (unsafe { _setjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag) }) != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 failures = failures + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             println!("recovered quiet {}", i);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe { c.run.unwrap()(i, 0 as i32) };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         g_callback = Some(panicky_callback);
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0 as i32..5 as i32 {
// CHECK-NEXT:         if (unsafe { _setjmp(std::ptr::addr_of_mut!(env.0) as *mut __jmp_buf_tag) }) != 0
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 failures = failures + 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             println!("recovered panicky {}", i);
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe { c.run.unwrap()(i, 0 as i32) };
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
