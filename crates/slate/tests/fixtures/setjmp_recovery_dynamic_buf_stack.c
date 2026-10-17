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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _setjmp(__env: *mut __jmp_buf_tag) -> i32;
// CHECK-NEXT:     fn longjmp(__env: *mut __jmp_buf_tag, __val: i32);
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
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
// CHECK-NEXT: static mut jb_stack: *mut [__jmp_buf_tag; 1] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut jb_top: i32 = 0 as i32;
// CHECK-NEXT: fn inner(fail: i32) {
// CHECK-NEXT:     if fail != 0 {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: i32 = (unsafe { jb_top }) - 1;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             jb_top = [[V_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             longjmp(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe { unsafe { jb_stack }.offset([[V_0]] as isize) }).as_mut_ptr()
// CHECK-NEXT:                         as *mut __jmp_buf_tag
// CHECK-NEXT:                 },
// CHECK-NEXT:                 42 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn run_case(id: i32, fail: i32) {
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = unsafe { jb_top };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         jb_top = [[V_1]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         _setjmp(unsafe {
// CHECK-NEXT:             (*unsafe { unsafe { jb_stack }.offset([[V_1]] as isize) }).as_mut_ptr()
// CHECK-NEXT:                 as *mut __jmp_buf_tag
// CHECK-NEXT:         })
// CHECK-NEXT:     }) == 0
// CHECK-NEXT:     {
// CHECK-NEXT:         inner(fail);
// CHECK-NEXT:         println!("case {}: no exception", id);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("case {}: caught", id);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         jb_stack = (unsafe { malloc((200 as u64).wrapping_mul(8 as u64)) })
// CHECK-NEXT:             as *mut [__jmp_buf_tag; 1];
// CHECK-NEXT:     }
// CHECK-NEXT:     run_case(0 as i32, 0 as i32);
// CHECK-NEXT:     run_case(1 as i32, 1 as i32);
// CHECK-NEXT:     run_case(2 as i32, 0 as i32);
// CHECK-NEXT:     unsafe { free((unsafe { jb_stack }) as *mut ()) };
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
