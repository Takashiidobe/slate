#include <setjmp.h>
#include <stdio.h>

static jmp_buf frame;

int main(void) {
  if (setjmp(frame) == 0) {
    longjmp(frame, 42);
  }
  printf("returned\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN
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
// CHECK-NEXT: static mut frame: __SlateAlign16<[__jmp_buf_tag; 1]> = __SlateAlign16(unsafe {
// CHECK-NEXT:     std::mem::zeroed()
// CHECK-NEXT: });
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         _setjmp(unsafe { std::ptr::addr_of_mut!(frame.0) as *mut __jmp_buf_tag })
// CHECK-NEXT:     }) == 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             longjmp(
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(frame.0) as *mut __jmp_buf_tag },
// CHECK-NEXT:                 42 as i32,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("returned");
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
