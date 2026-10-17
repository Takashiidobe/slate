#include <stdio.h>

int main(void) {
  remove("slate_stdio_fwrite_return.tmp");
  FILE *f = fopen("slate_stdio_fwrite_return.tmp", "w");
  if (!f) {
    puts("open-fail");
    return 0;
  }
  char   buf[16] = "0123456789abcde";
  size_t n       = fwrite(buf, 1, 15, f);
  printf("%zu\n", n);
  fclose(f);
  remove("slate_stdio_fwrite_return.tmp");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn remove(__filename: *const i8) -> i32;
// CHECK-NEXT:     fn fclose(__stream: *mut _IO_FILE) -> i32;
// CHECK-NEXT:     fn fopen(__filename: *const i8, __modes: *const i8) -> *mut _IO_FILE;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn puts(__s: *const i8) -> i32;
// CHECK-NEXT:     fn fwrite(__ptr: *const (), __size: u64, __n: u64, __s: *mut _IO_FILE) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_FILE {
// CHECK-NEXT:     _flags: i32,
// CHECK-NEXT:     _IO_read_ptr: *mut i8,
// CHECK-NEXT:     _IO_read_end: *mut i8,
// CHECK-NEXT:     _IO_read_base: *mut i8,
// CHECK-NEXT:     _IO_write_base: *mut i8,
// CHECK-NEXT:     _IO_write_ptr: *mut i8,
// CHECK-NEXT:     _IO_write_end: *mut i8,
// CHECK-NEXT:     _IO_buf_base: *mut i8,
// CHECK-NEXT:     _IO_buf_end: *mut i8,
// CHECK-NEXT:     _IO_save_base: *mut i8,
// CHECK-NEXT:     _IO_backup_base: *mut i8,
// CHECK-NEXT:     _IO_save_end: *mut i8,
// CHECK-NEXT:     _markers: *mut _IO_marker,
// CHECK-NEXT:     _chain: *mut _IO_FILE,
// CHECK-NEXT:     _fileno: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT:     _short_backupbuf: [i8; 1],
// CHECK-NEXT:     _old_offset: i64,
// CHECK-NEXT:     _cur_column: u16,
// CHECK-NEXT:     _vtable_offset: i8,
// CHECK-NEXT:     _shortbuf: [i8; 1],
// CHECK-NEXT:     _lock: *mut (),
// CHECK-NEXT:     _offset: i64,
// CHECK-NEXT:     _codecvt: *mut _IO_codecvt,
// CHECK-NEXT:     _wide_data: *mut _IO_wide_data,
// CHECK-NEXT:     _freeres_list: *mut _IO_FILE,
// CHECK-NEXT:     _freeres_buf: *mut (),
// CHECK-NEXT:     _prevchain: *mut *mut _IO_FILE,
// CHECK-NEXT:     _mode: i32,
// CHECK-NEXT:     _unused3: i32,
// CHECK-NEXT:     _total_written: u64,
// CHECK-NEXT:     _unused2: [i8; 8],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_marker {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_codecvt {}
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_wide_data {}
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;3],
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_fwrite_return.tmp".as_ptr()) };
// CHECK-NEXT:     let f: *mut _IO_FILE = unsafe {
// CHECK-NEXT:         fopen(c"slate_stdio_fwrite_return.tmp".as_ptr(), c"w".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     if !(f != std::ptr::null_mut::<_IO_FILE>()) {
// CHECK-NEXT:         unsafe { puts(c"open-fail".as_ptr()) };
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 16]> = __SlateAlign16([
// CHECK-NEXT:         48 as i8,
// CHECK-NEXT:         49 as i8,
// CHECK-NEXT:         50 as i8,
// CHECK-NEXT:         51 as i8,
// CHECK-NEXT:         52 as i8,
// CHECK-NEXT:         53 as i8,
// CHECK-NEXT:         54 as i8,
// CHECK-NEXT:         55 as i8,
// CHECK-NEXT:         56 as i8,
// CHECK-NEXT:         57 as i8,
// CHECK-NEXT:         97 as i8,
// CHECK-NEXT:         98 as i8,
// CHECK-NEXT:         99 as i8,
// CHECK-NEXT:         100 as i8,
// CHECK-NEXT:         101 as i8,
// CHECK-NEXT:         0 as i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let n: u64 = unsafe {
// CHECK-NEXT:         fwrite(buf.0.as_mut_ptr() as *const (), 1 as u64, 15 as u64, f)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { printf(c"%zu\n".as_ptr(), n) };
// CHECK-NEXT:     unsafe { fclose(f) };
// CHECK-NEXT:     unsafe { remove(c"slate_stdio_fwrite_return.tmp".as_ptr()) };
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
