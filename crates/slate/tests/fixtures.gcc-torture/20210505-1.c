/* PR target/100402 */
/* Testcase by Hannes Domani <ssbssa@yahoo.de> */

/* { dg-require-effective-target indirect_jumps } */

#include <setjmp.h>
#include <stdbool.h>

static jmp_buf buf;
static _Bool   stop = false;

void call_func(void (*func)(void)) { func(); }

void func(void) {
  stop = true;
  longjmp(buf, 1);
}

int main(void) {
  setjmp(buf);

  while (!stop)
    call_func(func);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/100402
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by Hannes Domani <ssbssa@yahoo.de>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn _setjmp(__env: *mut __jmp_buf_tag) -> i32;
// CHECK-NEXT:     fn longjmp(__env: *mut __jmp_buf_tag, __val: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __val: [u64; 16],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __jmp_buf_tag {
// CHECK-NEXT:     __jmpbuf: [i64; 8],
// CHECK-NEXT:     __mask_was_saved: i32,
// CHECK-NEXT:     __saved_mask: [[SLATERECORD_0]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-require-effective-target indirect_jumps }
// CHECK-NEXT: static mut buf: __SlateAlign16<[__jmp_buf_tag; 1]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: static mut stop: bool = false as bool;
// CHECK-EMPTY:
// CHECK-NEXT: fn call_func(mut func: Option<unsafe extern "C-unwind" fn()>) {
// CHECK-NEXT:     unsafe { func.unwrap()() };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn func() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         stop = true as bool;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         longjmp(
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(buf.0) as *mut __jmp_buf_tag },
// CHECK-NEXT:             1 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { _setjmp(unsafe { std::ptr::addr_of_mut!(buf.0) as *mut __jmp_buf_tag }) };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while !unsafe { stop } {
// CHECK-NEXT:         call_func(Some(func));
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
