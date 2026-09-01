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
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct parser_t {
// LOWERING-NEXT:     read_handler:
// LOWERING-NEXT:         Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>,
// LOWERING-NEXT:     read_handler_data: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn read_bytes(
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut u8,
// LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// LOWERING-NEXT:     {{arg[0-9]+}}: *mut u64,
// LOWERING-NEXT: ) -> i32 {
// LOWERING-NEXT:     let mut buffer: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     let mut source: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     buffer = {{arg[0-9]+}};
// LOWERING-NEXT:     size = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = {{arg[0-9]+}} as *mut u8;
// LOWERING-NEXT:     source = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: u64 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = size;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut u8 = source;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: u64 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut u8 = buffer;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = size;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut input: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut tag: aligned::Aligned<aligned::A16, [u8; 22]> = aligned::Aligned([0; 22]);
// LOWERING-NEXT:     let mut buffer: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut size_read: u64 = 0;
// LOWERING-NEXT:     let mut parser: parser_t = parser_t {
// LOWERING-NEXT:         read_handler: None,
// LOWERING-NEXT:         read_handler_data: std::ptr::null_mut(),
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     input = [97, 98, 99, 0];
// LOWERING-NEXT:     *tag = [
// LOWERING-NEXT:         116, 97, 103, 58, 121, 97, 109, 108, 46, 111, 114, 103, 44, 50, 48, 48, 50, 58, 115, 116,
// LOWERING-NEXT:         114, 0,
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     buffer = [0, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 0;
// LOWERING-NEXT:     size_read = {{_v[0-9]+}};
// LOWERING-NEXT:     parser.read_handler = unsafe {
// LOWERING-NEXT:         std::mem::transmute::<
// LOWERING-NEXT:             *const (),
// LOWERING-NEXT:             Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>,
// LOWERING-NEXT:         >(read_bytes as *const ())
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = input.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     parser.read_handler_data = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32> =
// LOWERING-NEXT:         parser.read_handler;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = parser.read_handler_data;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = buffer.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, std::ptr::addr_of_mut!(size_read)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %lu %c %c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = size_read;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = buffer[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = tag[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct parser_t {
// REWRITES-NEXT:     read_handler:
// REWRITES-NEXT:         Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>,
// REWRITES-NEXT:     read_handler_data: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn read_bytes(
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut u8,
// REWRITES-NEXT:     {{arg[0-9]+}}: u64,
// REWRITES-NEXT:     {{arg[0-9]+}}: *mut u64,
// REWRITES-NEXT: ) -> i32 {
// REWRITES-NEXT:     let mut buffer: *mut u8 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut size: u64 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut source: *mut u8 = std::ptr::null_mut();
// REWRITES-NEXT:     source = {{arg[0-9]+}} as *mut u8;
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let mut i: u64 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:             if !(i < size) {
// REWRITES-NEXT:                 break;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: *mut u8 = source;
// REWRITES-NEXT:                 let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(i as usize) };
// REWRITES-NEXT:                 let {{_v[0-9]+}}: u8 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:                 let {{_v[0-9]+}}: *mut u8 = buffer;
// REWRITES-NEXT:                 let {{_v[0-9]+}}: *mut u8 = unsafe { {{_v[0-9]+}}.add(i as usize) };
// REWRITES-NEXT:                 unsafe {
// REWRITES-NEXT:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:             i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = size;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return 1;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut input: [u8; 4] = [97, 98, 99, 0];
// REWRITES-NEXT:     let mut tag: aligned::Aligned<aligned::A16, [u8; 22]> = aligned::Aligned([0; 22]);
// REWRITES-NEXT:     let mut buffer: [u8; 4] = [0; 4];
// REWRITES-NEXT:     let mut size_read: u64 = 0;
// REWRITES-NEXT:     let mut parser: parser_t = parser_t {
// REWRITES-NEXT:         read_handler: None,
// REWRITES-NEXT:         read_handler_data: std::ptr::null_mut(),
// REWRITES-NEXT:     };
// REWRITES-NEXT:     *tag = [
// REWRITES-NEXT:         116, 97, 103, 58, 121, 97, 109, 108, 46, 111, 114, 103, 44, 50, 48, 48, 50, 58, 115, 116,
// REWRITES-NEXT:         114, 0,
// REWRITES-NEXT:     ];
// REWRITES-NEXT:     buffer = [0, 0, 0, 0];
// REWRITES-NEXT:     parser.read_handler = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<
// REWRITES-NEXT:             *const (),
// REWRITES-NEXT:             Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>,
// REWRITES-NEXT:         >(read_bytes as *const ())
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = input.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     parser.read_handler_data = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32> =
// REWRITES-NEXT:         parser.read_handler;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = parser.read_handler_data;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut u8 = buffer.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %lu %c %c\n".as_ptr(),
// REWRITES-NEXT:             unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}, 3 as u64, std::ptr::addr_of_mut!(size_read)) },
// REWRITES-NEXT:             size_read,
// REWRITES-NEXT:             buffer[((1 as i64) as usize)] as i32,
// REWRITES-NEXT:             tag[((4 as i64) as usize)] as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
