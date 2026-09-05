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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct parser_t {
// COMMON-LOWERING-NEXT:     read_handler:
// COMMON-LOWERING-NEXT:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>,
// COMMON-LOWERING-NEXT:     read_handler_data: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut input: [u8; 4] = [0; 4];
// COMMON-LOWERING-NEXT:     let mut buffer: [u8; 4] = [0; 4];
// COMMON-LOWERING-NEXT:     let mut size_read: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut parser: parser_t = parser_t {
// COMMON-LOWERING-NEXT:         read_handler: None,
// COMMON-LOWERING-NEXT:         read_handler_data: std::ptr::null_mut(),
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [97, 98, 99, 0];
// COMMON-LOWERING-NEXT:     input = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 22] = [
// COMMON-LOWERING-NEXT:         116, 97, 103, 58, 121, 97, 109, 108, 46, 111, 114, 103, 44, 50, 48, 48, 50, 58, 115, 116,
// COMMON-LOWERING-NEXT:         114, 0,
// COMMON-LOWERING-NEXT:     ];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [0; 4];
// COMMON-LOWERING-NEXT:     buffer = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:     size_read = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     parser.read_handler = unsafe {
// COMMON-LOWERING-NEXT:         std::mem::transmute::<
// COMMON-LOWERING-NEXT:             *const (),
// COMMON-LOWERING-NEXT:             Option<
// COMMON-LOWERING-NEXT:                 unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32,
// COMMON-LOWERING-NEXT:             >,
// COMMON-LOWERING-NEXT:         >(read_bytes as *const ())
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = input.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-LOWERING-NEXT:     parser.read_handler_data = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: Option<
// COMMON-LOWERING-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32,
// COMMON-LOWERING-NEXT:     > = parser.read_handler;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = parser.read_handler_data;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = buffer.as_mut_ptr() as *mut u8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(size_read)) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = size_read;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = buffer[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = tag[({{__v[0-9]+}} as usize)];
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: extern "C-unwind" fn read_bytes(
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut u8,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: u64,
// COMMON-LOWERING-NEXT:     {{arg[0-9]+}}: *mut u64,
// COMMON-LOWERING-NEXT: ) -> i32 {
// COMMON-LOWERING-NEXT:     let mut buffer: *mut u8 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut size: u64 = 0;
// COMMON-LOWERING-NEXT:     let mut source: *mut u8 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     buffer = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     size = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u8 = {{arg[0-9]+}} as *mut u8;
// COMMON-LOWERING-NEXT:     source = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut i: u64 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-NEXT:         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = size;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut u8 = source;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut u8 = buffer;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add({{__v[0-9]+}} as usize) };
// COMMON-LOWERING-NEXT:                 unsafe {
// COMMON-LOWERING-NEXT:                     *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = size;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{arg[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let mut tag: aligned::Aligned<aligned::A16, [u8; 22]> = aligned::Aligned([0; 22]);
// LOWERING-X86_64-GNU-NEXT:     *tag = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %lu %c %c\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let mut tag: [u8; 22] = [0; 22];
// LOWERING-AARCH64-GNU-NEXT:     tag = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %lu %c %c\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct parser_t {
// COMMON-REWRITES-NEXT:     read_handler:
// COMMON-REWRITES-NEXT:         Option<unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>,
// COMMON-REWRITES-NEXT:     read_handler_data: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut input: [u8; 4] = [0; 4];
// COMMON-REWRITES-NEXT:     let mut buffer: [u8; 4] = [0; 4];
// COMMON-REWRITES-NEXT:     let mut size_read: u64 = 0;
// COMMON-REWRITES-NEXT:     let mut parser: parser_t = parser_t {
// COMMON-REWRITES-NEXT:         read_handler: None,
// COMMON-REWRITES-NEXT:         read_handler_data: std::ptr::null_mut(),
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     input = [97, 98, 99, 0];
// COMMON-REWRITES-NEXT:         116, 97, 103, 58, 121, 97, 109, 108, 46, 111, 114, 103, 44, 50, 48, 48, 50, 58, 115, 116,
// COMMON-REWRITES-NEXT:         114, 0,
// COMMON-REWRITES-NEXT:     ];
// COMMON-REWRITES-NEXT:     buffer = [0; 4];
// COMMON-REWRITES-NEXT:     parser.read_handler = unsafe {
// COMMON-REWRITES-NEXT:         std::mem::transmute::<
// COMMON-REWRITES-NEXT:             *const (),
// COMMON-REWRITES-NEXT:             Option<
// COMMON-REWRITES-NEXT:                 unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32,
// COMMON-REWRITES-NEXT:             >,
// COMMON-REWRITES-NEXT:         >(read_bytes as *const ())
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = input.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     parser.read_handler_data = {{__v[0-9]+}} as *mut core::ffi::c_void;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: Option<
// COMMON-REWRITES-NEXT:         unsafe extern "C-unwind" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32,
// COMMON-REWRITES-NEXT:     > = parser.read_handler;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = parser.read_handler_data;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u8 = buffer.as_mut_ptr() as *mut u8;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %lu %c %c\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe { {{__v[0-9]+}}.unwrap()({{__v[0-9]+}}, {{__v[0-9]+}}, 3 as u64, std::ptr::addr_of_mut!(size_read)) },
// COMMON-REWRITES-NEXT:             size_read,
// COMMON-REWRITES-NEXT:             buffer[1] as i32,
// COMMON-REWRITES-NEXT:             tag[4] as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: extern "C-unwind" fn read_bytes(
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut core::ffi::c_void,
// COMMON-REWRITES-NEXT:     mut buffer: *mut u8,
// COMMON-REWRITES-NEXT:     mut size: u64,
// COMMON-REWRITES-NEXT:     {{arg[0-9]+}}: *mut u64,
// COMMON-REWRITES-NEXT: ) -> i32 {
// COMMON-REWRITES-NEXT:     let mut source: *mut u8 = {{arg[0-9]+}} as *mut u8;
// COMMON-REWRITES-NEXT:     for i in 0..size {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut u8 = source;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(i as usize) };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: u8 = unsafe { *{{__v[0-9]+}} };
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut u8 = buffer;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut u8 = unsafe { {{__v[0-9]+}}.add(i as usize) };
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{arg[0-9]+}} = size;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     1
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     let mut tag: aligned::Aligned<aligned::A16, [u8; 22]> = aligned::Aligned([0; 22]);
// REWRITES-X86_64-GNU-NEXT:     *tag = [
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     let mut tag: [u8; 22] = [0; 22];
// REWRITES-AARCH64-GNU-NEXT:     tag = [
// SLATE-FILECHECK-END rewrites-aarch64-gnu
