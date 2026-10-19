#include <stdio.h>

typedef unsigned char yaml_char_t;
typedef int           yaml_read_handler_t(void *, yaml_char_t *, unsigned long,
                                          unsigned long *);

typedef struct {
  yaml_read_handler_t *read_handler;
  void                *read_handler_data;
} parser_t;

static int read_bytes(void *data, yaml_char_t *buffer, unsigned long size,
                      unsigned long *size_read) {
  yaml_char_t *source = (yaml_char_t *)data;
  for (unsigned long i = 0; i < size; i++) {
    buffer[i] = source[i];
  }
  *size_read = size;
  return 1;
}

int main(void) {
  yaml_char_t   input[]   = "abc";
  yaml_char_t   tag[]     = "tag:yaml.org,2002:str";
  yaml_char_t   buffer[4] = {0};
  unsigned long size_read = 0;
  parser_t      parser;
  parser.read_handler      = read_bytes;
  parser.read_handler_data = input;
  int ok = parser.read_handler(parser.read_handler_data, buffer, 3, &size_read);
  printf("%d %lu %c %c\n", ok, size_read, buffer[1], tag[4]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     read_handler: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(*mut (), *mut u8, u64, *mut u64) -> i32,
// CHECK-NEXT:     >,
// CHECK-NEXT:     read_handler_data: *mut (),
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: extern "C-unwind" fn read_bytes(
// CHECK-NEXT:     data: *mut (),
// CHECK-NEXT:     buffer: *mut u8,
// CHECK-NEXT:     size: u64,
// CHECK-NEXT:     size_read: *mut u64,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let source: *mut u8 = data as *mut u8;
// CHECK-NEXT:     for i in 0..size {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *buffer.offset(i as isize) = *source.offset(i as isize);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *size_read = size;
// CHECK-NEXT:     }
// CHECK-NEXT:     1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut input: [u8; 4] = [97, 98, 99, 0];
// CHECK-NEXT:     let tag: __SlateAlign16<[u8; 22]> = __SlateAlign16([
// CHECK-NEXT:         116u8, 97u8, 103u8, 58u8, 121u8, 97u8, 109u8, 108u8, 46u8, 111u8, 114u8, 103u8,
// CHECK-NEXT:         44u8, 50u8, 48u8, 48u8, 50u8, 58u8, 115u8, 116u8, 114u8, 0u8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut buffer: [u8; 4] = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: [u8; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]][0] = 0u8;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut size_read: u64 = 0;
// CHECK-NEXT:     let mut parser: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     parser.read_handler = Some(read_bytes);
// CHECK-NEXT:     parser.read_handler_data = input.as_mut_ptr() as *mut ();
// CHECK-NEXT:     let ok: i32 = unsafe {
// CHECK-NEXT:         parser
// CHECK-NEXT:             .read_handler
// CHECK-NEXT:             .unwrap()(
// CHECK-NEXT:             parser.read_handler_data,
// CHECK-NEXT:             buffer.as_mut_ptr() as *mut u8,
// CHECK-NEXT:             3u64,
// CHECK-NEXT:             std::ptr::addr_of_mut!(size_read),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %lu %c %c\n".as_ptr(),
// CHECK-NEXT:             ok,
// CHECK-NEXT:             size_read,
// CHECK-NEXT:             (buffer[1] as u32) as i32,
// CHECK-NEXT:             (tag.0[4] as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
