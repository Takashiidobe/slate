#define _GNU_SOURCE
#include <error.h>
#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>

static void handle_exit(int status, void *arg) {
  int *captured = (int *)arg;
  *captured     = status;
  printf("on_exit:%d\n", status);
}

int main(void) {
  int captured = -1;
  on_exit(handle_exit, &captured);

  int                mcheck_enabled = mcheck(NULL) == 0;
  void              *block          = malloc(16);
  enum mcheck_status probe          = mprobe(block);
  printf("mcheck:%d %d\n", mcheck_enabled, probe == MCHECK_OK);
  free(block);

  error_one_per_line = 0;
  error(0, 0, "first message");
  error(0, 0, "second message");
  printf("count_after_two:%u\n", error_message_count);

  error_one_per_line = 1;
  error_at_line(0, 0, "sample.c", 42, "deduped message");
  error_at_line(0, 0, "sample.c", 42, "deduped message");
  printf("count_after_dedup:%u\n", error_message_count);

  error_at_line(0, 0, "sample.c", 43, "different line");
  printf("count_after_new_line:%u\n", error_message_count);

  error(5, 0, "fatal message");
  printf("unreachable\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut error_message_count: u32;
// CHECK-NEXT:     static mut error_one_per_line: i32;
// CHECK-NEXT:     fn error(__status: i32, __errnum: i32, __format: *const i8, ...);
// CHECK-NEXT:     fn error_at_line(
// CHECK-NEXT:         __status: i32,
// CHECK-NEXT:         __errnum: i32,
// CHECK-NEXT:         __fname: *const i8,
// CHECK-NEXT:         __lineno: u32,
// CHECK-NEXT:         __format: *const i8,
// CHECK-NEXT:         ...
// CHECK-NEXT:     );
// CHECK-NEXT:     fn mcheck(__abortfunc: Option<unsafe extern "C-unwind" fn(i32)>) -> i32;
// CHECK-NEXT:     fn mprobe(__ptr: *mut ()) -> i32;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT:     fn on_exit(
// CHECK-NEXT:         __func: Option<unsafe extern "C-unwind" fn(i32, *mut ())>,
// CHECK-NEXT:         __arg: *mut (),
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: extern "C-unwind" fn handle_exit(status: i32, arg: *mut ()) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(arg as *mut i32) = status;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("on_exit:{}", status);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut captured: i32 = -1;
// CHECK-NEXT:     unsafe { on_exit(Some(handle_exit), std::ptr::addr_of_mut!(captured) as *mut ()) };
// CHECK-NEXT:     let mcheck_enabled: i32 = ((unsafe { mcheck(None) }) == 0) as i32;
// CHECK-NEXT:     let block: *mut () = unsafe { malloc(16 as u64) };
// CHECK-NEXT:     let probe: i32 = unsafe { mprobe(block) };
// CHECK-NEXT:     println!("mcheck:{} {}", mcheck_enabled, ((probe as i32) == 0) as i32);
// CHECK-NEXT:     unsafe { free(block) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         error_one_per_line = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { error(0 as i32, 0 as i32, c"first message".as_ptr()) };
// CHECK-NEXT:     unsafe { error(0 as i32, 0 as i32, c"second message".as_ptr()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"count_after_two:%u\n".as_ptr(), unsafe { error_message_count }) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         error_one_per_line = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         error_at_line(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             c"sample.c".as_ptr(),
// CHECK-NEXT:             42 as u32,
// CHECK-NEXT:             c"deduped message".as_ptr(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         error_at_line(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             c"sample.c".as_ptr(),
// CHECK-NEXT:             42 as u32,
// CHECK-NEXT:             c"deduped message".as_ptr(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"count_after_dedup:%u\n".as_ptr(), unsafe { error_message_count })
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         error_at_line(
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             c"sample.c".as_ptr(),
// CHECK-NEXT:             43 as u32,
// CHECK-NEXT:             c"different line".as_ptr(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"count_after_new_line:%u\n".as_ptr(), unsafe { error_message_count })
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { error(5 as i32, 0 as i32, c"fatal message".as_ptr()) };
// CHECK-NEXT:     println!("unreachable");
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
