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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum mcheck_status {
// LOWERING-NEXT:     MCHECK_DISABLED = -1,
// LOWERING-NEXT:     MCHECK_OK = 0,
// LOWERING-NEXT:     MCHECK_HEAD = 1,
// LOWERING-NEXT:     MCHECK_TAIL = 2,
// LOWERING-NEXT:     MCHECK_FREE = 3,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     static mut error_message_count: u32;
// LOWERING-NEXT:     static mut error_one_per_line: i32;
// LOWERING-NEXT:     fn on_exit(
// LOWERING-NEXT:         _0: Option<unsafe extern "C-unwind" fn(i32, *mut core::ffi::c_void)>,
// LOWERING-NEXT:         _1: *mut core::ffi::c_void,
// LOWERING-NEXT:     ) -> i32;
// LOWERING-NEXT:     fn mcheck(_0: Option<unsafe extern "C-unwind" fn(i32)>) -> i32;
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn mprobe(_0: *mut core::ffi::c_void) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn error(_0: i32, _1: i32, _2: *const core::ffi::c_char, ...);
// LOWERING-NEXT:     fn error_at_line(
// LOWERING-NEXT:         _0: i32,
// LOWERING-NEXT:         _1: i32,
// LOWERING-NEXT:         _2: *const core::ffi::c_char,
// LOWERING-NEXT:         _3: u32,
// LOWERING-NEXT:         _4: *const core::ffi::c_char,
// LOWERING-NEXT:         ...
// LOWERING-NEXT:     );
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut captured: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     captured = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(captured) as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { on_exit(Some(handle_exit), {{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32)> = None;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { mcheck({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{__v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { mprobe({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"mcheck:%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"mcheck:%d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = mcheck_status::MCHECK_OK as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error_one_per_line = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"first message\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"first message\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error(
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"second message\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"second message\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error(
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"count_after_two:%u\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"count_after_two:%u\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { error_message_count };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error_one_per_line = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"sample.c\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"sample.c\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 42;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"deduped message\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"deduped message\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error_at_line(
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"sample.c\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"sample.c\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 42;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"deduped message\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"deduped message\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error_at_line(
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"count_after_dedup:%u\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"count_after_dedup:%u\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { error_message_count };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"sample.c\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"sample.c\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = 43;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"different line\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"different line\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error_at_line(
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}} as u32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"count_after_new_line:%u\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"count_after_new_line:%u\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = unsafe { error_message_count };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"fatal message\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"fatal message\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         error(
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as i32,
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"unreachable\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"unreachable\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn handle_exit({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = {{arg[0-9]+}} as *mut i32;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{__v[0-9]+}} = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"on_exit:%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"on_exit:%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum mcheck_status {
// REWRITES-NEXT:     MCHECK_DISABLED = -1,
// REWRITES-NEXT:     MCHECK_OK = 0,
// REWRITES-NEXT:     MCHECK_HEAD = 1,
// REWRITES-NEXT:     MCHECK_TAIL = 2,
// REWRITES-NEXT:     MCHECK_FREE = 3,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     static mut error_message_count: u32;
// REWRITES-NEXT:     static mut error_one_per_line: i32;
// REWRITES-NEXT:     fn on_exit(
// REWRITES-NEXT:         _0: Option<unsafe extern "C-unwind" fn(i32, *mut core::ffi::c_void)>,
// REWRITES-NEXT:         _1: *mut core::ffi::c_void,
// REWRITES-NEXT:     ) -> i32;
// REWRITES-NEXT:     fn mcheck(_0: Option<unsafe extern "C-unwind" fn(i32)>) -> i32;
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn mprobe(_0: *mut core::ffi::c_void) -> i32;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT:     fn error(_0: i32, _1: i32, _2: *const core::ffi::c_char, ...);
// REWRITES-NEXT:     fn error_at_line(
// REWRITES-NEXT:         _0: i32,
// REWRITES-NEXT:         _1: i32,
// REWRITES-NEXT:         _2: *const core::ffi::c_char,
// REWRITES-NEXT:         _3: u32,
// REWRITES-NEXT:         _4: *const core::ffi::c_char,
// REWRITES-NEXT:         ...
// REWRITES-NEXT:     );
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut captured: i32 = 0;
// REWRITES-NEXT:     captured = -1;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(captured) as *mut core::ffi::c_void;
// REWRITES-NEXT:     unsafe { on_exit(Some(handle_exit), {{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32)> = None;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { mcheck({{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} == 0) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc((16 as u64) as usize) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { mprobe({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"mcheck:%d %d\n".as_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"mcheck:%d %d\n".as_ptr() as *mut u8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = mcheck_status::MCHECK_OK as i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             ({{__v[0-9]+}} == {{__v[0-9]+}}) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { free({{__v[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         error_one_per_line = 0;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { error(0 as i32, 0 as i32, c"first message".as_ptr()) };
// REWRITES-NEXT:     unsafe { error(0 as i32, 0 as i32, c"second message".as_ptr()) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(c"count_after_two:%u\n".as_ptr(), unsafe {
// REWRITES-NEXT:             error_message_count
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         error_one_per_line = 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         error_at_line(
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:             c"sample.c".as_ptr(),
// REWRITES-NEXT:             42 as u32,
// REWRITES-NEXT:             c"deduped message".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         error_at_line(
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:             c"sample.c".as_ptr(),
// REWRITES-NEXT:             42 as u32,
// REWRITES-NEXT:             c"deduped message".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(c"count_after_dedup:%u\n".as_ptr(), unsafe {
// REWRITES-NEXT:             error_message_count
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         error_at_line(
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:             c"sample.c".as_ptr(),
// REWRITES-NEXT:             43 as u32,
// REWRITES-NEXT:             c"different line".as_ptr(),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(c"count_after_new_line:%u\n".as_ptr(), unsafe {
// REWRITES-NEXT:             error_message_count
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe { error(5 as i32, 0 as i32, c"fatal message".as_ptr()) };
// REWRITES-NEXT:     unsafe { printf(c"unreachable\n".as_ptr()) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn handle_exit({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut core::ffi::c_void) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *({{arg[0-9]+}} as *mut i32) = {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"on_exit:%d\n".as_ptr(), {{arg[0-9]+}}) };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
