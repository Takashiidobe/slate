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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct parser_t {
// LOWERING-NEXT:     read_handler: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>,
// LOWERING-NEXT:     read_handler_data: *mut core::ffi::c_void,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn read_bytes(arg0: *mut core::ffi::c_void, arg1: *mut u8, arg2: u64, arg3: *mut u64) -> i32 {
// LOWERING-NEXT:     let mut data: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut buffer: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut size: u64 = 0;
// LOWERING-NEXT:     let mut size_read: *mut u64 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut source: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:     data = arg0;
// LOWERING-NEXT:     buffer = arg1;
// LOWERING-NEXT:     size = arg2;
// LOWERING-NEXT:     size_read = arg3;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = data;
// LOWERING-NEXT:     let _v1: *mut u8 = _v0 as *mut u8;
// LOWERING-NEXT:     source = _v1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: u64 = 0;
// LOWERING-NEXT:         let _v2: u64 = 0;
// LOWERING-NEXT:         i = _v2;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v3: u64 = i;
// LOWERING-NEXT:             let _v4: u64 = size;
// LOWERING-NEXT:             let _v5: bool = _v3 < _v4;
// LOWERING-NEXT:             if !_v5 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v6: u64 = i;
// LOWERING-NEXT:                 let _v7: *mut u8 = source;
// LOWERING-NEXT:                 let _v8: *mut u8 = unsafe { _v7.add(_v6 as usize) };
// LOWERING-NEXT:                 let _v9: u8 = unsafe { *_v8 };
// LOWERING-NEXT:                 let _v10: u64 = i;
// LOWERING-NEXT:                 let _v11: *mut u8 = buffer;
// LOWERING-NEXT:                 let _v12: *mut u8 = unsafe { _v11.add(_v10 as usize) };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *_v12 = _v9;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v13: u64 = i;
// LOWERING-NEXT:             let _v14: u64 = _v13 + 1;
// LOWERING-NEXT:             i = _v14;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v15: u64 = size;
// LOWERING-NEXT:     let _v16: *mut u64 = size_read;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *_v16 = _v15;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v17: i32 = 1;
// LOWERING-NEXT:     __retval = _v17;
// LOWERING-NEXT:     let _v18: i32 = __retval;
// LOWERING-NEXT:     return _v18;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut input: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut tag: aligned::Aligned<aligned::A16, [u8; 22]> = aligned::Aligned([0; 22]);
// LOWERING-NEXT:     let mut buffer: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut size_read: u64 = 0;
// LOWERING-NEXT:     let mut parser: parser_t = parser_t { read_handler: None, read_handler_data: std::ptr::null_mut() };
// LOWERING-NEXT:     let mut ok: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     input = [97, 98, 99, 0];
// LOWERING-NEXT:     *tag = [116, 97, 103, 58, 121, 97, 109, 108, 46, 111, 114, 103, 44, 50, 48, 48, 50, 58, 115, 116, 114, 0];
// LOWERING-NEXT:     buffer = [0, 0, 0, 0];
// LOWERING-NEXT:     let _v1: u64 = 0;
// LOWERING-NEXT:     size_read = _v1;
// LOWERING-NEXT:     parser.read_handler = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>>(read_bytes as *const ()) };
// LOWERING-NEXT:     let _v2: *mut u8 = input.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v3: *mut core::ffi::c_void = _v2 as *mut core::ffi::c_void;
// LOWERING-NEXT:     parser.read_handler_data = _v3;
// LOWERING-NEXT:     let _v4: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32> = parser.read_handler;
// LOWERING-NEXT:     let _v5: *mut core::ffi::c_void = parser.read_handler_data;
// LOWERING-NEXT:     let _v6: *mut u8 = buffer.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let _v7: u64 = 3;
// LOWERING-NEXT:     let _v8: i32 = unsafe { _v4.unwrap()(_v5, _v6, _v7, std::ptr::addr_of_mut!(size_read)) };
// LOWERING-NEXT:     ok = _v8;
// LOWERING-NEXT:     let _v9: *mut i8 = b"%d %lu %c %c\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: i32 = ok;
// LOWERING-NEXT:     let _v11: u64 = size_read;
// LOWERING-NEXT:     let _v12: i64 = 1;
// LOWERING-NEXT:     let _v13: u8 = buffer[(_v12 as usize)];
// LOWERING-NEXT:     let _v14: i32 = _v13 as i32;
// LOWERING-NEXT:     let _v15: i64 = 4;
// LOWERING-NEXT:     let _v16: u8 = tag[(_v15 as usize)];
// LOWERING-NEXT:     let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:     let _v18: i32 = unsafe { printf(_v9 as *const i8, _v10, _v11, _v14, _v17) };
// LOWERING-NEXT:     let _v19: i32 = 0;
// LOWERING-NEXT:     __retval = _v19;
// LOWERING-NEXT:     let _v20: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v20 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct parser_t {
// REWRITES-NEXT:     read_handler: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>,
// REWRITES-NEXT:     read_handler_data: *mut core::ffi::c_void,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn read_bytes(arg0: *mut core::ffi::c_void, arg1: *mut u8, arg2: u64, arg3: *mut u64) -> i32 {
// REWRITES-NEXT: let mut data: *mut core::ffi::c_void = arg0;
// REWRITES-NEXT: let mut buffer: *mut u8 = arg1;
// REWRITES-NEXT: let mut size: u64 = arg2;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut source: *mut u8 = std::ptr::null_mut();
// REWRITES-NEXT: source = data as *mut u8;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: u64 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     if !(i < size) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v7: *mut u8 = source;
// REWRITES-NEXT:                                     let _v8: *mut u8 = unsafe { _v7.add(i as usize) };
// REWRITES-NEXT:                                     let _v9: u8 = unsafe { *_v8 };
// REWRITES-NEXT:                                     let _v11: *mut u8 = buffer;
// REWRITES-NEXT:                                     let _v12: *mut u8 = unsafe { _v11.add(i as usize) };
// REWRITES-NEXT:                                     unsafe {
// REWRITES-NEXT:                                                         *_v12 = _v9;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *arg3 = size;
// REWRITES-NEXT: }
// REWRITES-NEXT: __retval = 1;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut input: [u8; 4] = [0; 4];
// REWRITES-NEXT: let mut tag: aligned::Aligned<aligned::A16, [u8; 22]> = aligned::Aligned([0; 22]);
// REWRITES-NEXT: let mut buffer: [u8; 4] = [0; 4];
// REWRITES-NEXT: let mut size_read: u64 = 0;
// REWRITES-NEXT: let mut parser: parser_t = parser_t { read_handler: None, read_handler_data: std::ptr::null_mut() };
// REWRITES-NEXT: let mut ok: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: input = [97, 98, 99, 0];
// REWRITES-NEXT: *tag = [116, 97, 103, 58, 121, 97, 109, 108, 46, 111, 114, 103, 44, 50, 48, 48, 50, 58, 115, 116, 114, 0];
// REWRITES-NEXT: buffer = [0, 0, 0, 0];
// REWRITES-NEXT: size_read = 0;
// REWRITES-NEXT: parser.read_handler = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32>>(read_bytes as *const ()) };
// REWRITES-NEXT: let _v2: *mut u8 = input.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: parser.read_handler_data = _v2 as *mut core::ffi::c_void;
// REWRITES-NEXT: let _v4: Option<unsafe extern "C" fn(*mut core::ffi::c_void, *mut u8, u64, *mut u64) -> i32> = parser.read_handler;
// REWRITES-NEXT: let _v5: *mut core::ffi::c_void = parser.read_handler_data;
// REWRITES-NEXT: let _v6: *mut u8 = buffer.as_mut_ptr() as *mut u8;
// REWRITES-NEXT: let _v7: u64 = 3;
// REWRITES-NEXT: ok = unsafe { _v4.unwrap()(_v5, _v6, _v7, std::ptr::addr_of_mut!(size_read)) };
// REWRITES-NEXT: let _v9: *mut i8 = b"%d %lu %c %c\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i64 = 1;
// REWRITES-NEXT: let _v15: i64 = 4;
// REWRITES-NEXT: let _v18: i32 = unsafe { printf(_v9 as *const i8, ok, size_read, buffer[(_v12 as usize)] as i32, tag[(_v15 as usize)] as i32) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
