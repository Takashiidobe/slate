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
// CHECK-NEXT: macro_rules! THROW {
// CHECK-NEXT:     ($v:expr) => {
// CHECK-NEXT:         let __v : i32 = unsafe { jb_top }; let [[V_0:__v[0-9]+]] : i32 = __v - 1; unsafe { jb_top =
// CHECK-NEXT:         [[V_0]]; } unsafe { longjmp((* unsafe { jb_stack } .offset([[V_0]] as isize))
// CHECK-NEXT:         .as_mut_ptr() as * mut __jmp_buf_tag, $v) };
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _setjmp([[ARG_0:arg[0-9]+]]: *mut __jmp_buf_tag) -> i32;
// CHECK-NEXT:     fn longjmp([[ARG_0]]: *mut __jmp_buf_tag, [[ARG_1:arg[0-9]+]]: i32);
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
// CHECK-NEXT: static mut jb_stack: *mut [__jmp_buf_tag; 1] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut jb_top: i32 = 0;
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: fn inner(fail: i32) {
// CHECK-NEXT:     if fail != 0 {
// CHECK-NEXT:         THROW!(42i32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn run_case(id: i32, fail: i32) {
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i32 = unsafe { jb_top };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         jb_top = [[V_1]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         _setjmp(
// CHECK-NEXT:             (*unsafe { jb_stack }.offset([[V_1]] as isize)).as_mut_ptr()
// CHECK-NEXT:                 as *mut __jmp_buf_tag,
// CHECK-NEXT:         )
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
// CHECK-NEXT:         jb_stack = (malloc(
// CHECK-NEXT:             (std::mem::size_of::<[__jmp_buf_tag; 1]>() as u64).wrapping_mul(8) as usize,
// CHECK-NEXT:         ) as *mut ()) as *mut [__jmp_buf_tag; 1];
// CHECK-NEXT:     }
// CHECK-NEXT:     run_case(0i32, 0i32);
// CHECK-NEXT:     run_case(1i32, 1i32);
// CHECK-NEXT:     run_case(2i32, 0i32);
// CHECK-NEXT:     unsafe { free((jb_stack as *mut ()) as *mut core::ffi::c_void) };
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
