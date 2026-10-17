#include <stdarg.h>
#include <stdio.h>

static void print_values(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stdout, format, args);
  va_end(args);
}

int main(void) {
  print_values("%d %s\n", 42, "forwarded");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stdout: *mut _IO_FILE;
// CHECK-NEXT:     fn vfprintf(
// CHECK-NEXT:         __s: *mut _IO_FILE,
// CHECK-NEXT:         __format: *const i8,
// CHECK-NEXT:         __arg: core::ffi::VaList<'_>,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
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
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits5U0,
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
// CHECK-NEXT:     pub struct __SlateBits5U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn print_values(format: *const i8, mut __va_args: ...) {
// CHECK-NEXT:     let mut args: core::ffi::VaList<'_> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     args = __va_args.clone();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { vfprintf(unsafe { stdout }, format, args.clone()) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     {}
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         print_values(c"%d %s\n".as_ptr(), 42 as i32, c"forwarded".as_ptr() as *mut i8)
// CHECK-NEXT:     };
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
