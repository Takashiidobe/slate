#include <stdio.h>
#include <unistd.h>

static long syscall3(long number, long a, long b, long c) {
  register long x8 __asm__("x8") = number;
  register long x0 __asm__("x0") = a;
  register long x1 __asm__("x1") = b;
  register long x2 __asm__("x2") = c;
  __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8), "r"(x1), "r"(x2) : "memory");
  return x0;
}

static long syscall0(long number) {
  register long x8 __asm__("x8") = number;
  register long x0 __asm__("x0");
  __asm__ volatile("svc #0" : "=r"(x0) : "r"(x8) : "memory");
  return x0;
}

static int add_w1(int a, int b) {
  register int w __asm__("w1") = a;
  __asm__("add %w0, %w0, %w1" : "+r"(w) : "r"(b));
  return w;
}

static double double_d3(double a) {
  register double d __asm__("d3") = a;
  __asm__("fadd %d0, %d0, %d0" : "+w"(d));
  return d;
}

int main(void) {
  static const char message[] = "hello from svc\n";
  fflush(stdout);
  long written = syscall3(64, 1, (long)message, sizeof message - 1);
  printf("%ld %d\n", written, syscall0(172) == getpid());
  printf("%d %.1f\n", add_w1(40, 2), double_d3(1.25));
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stdout: *mut _IO_FILE;
// CHECK-NEXT:     fn fflush([[ARG_0:arg[0-9]+]]: *mut _IO_FILE) -> i32;
// CHECK-NEXT:     fn printf([[ARG_0]]: *const u8, ...) -> i32;
// CHECK-NEXT:     fn getpid() -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct _IO_FILE {
// CHECK-NEXT:     _flags: i32,
// CHECK-NEXT:     _IO_read_ptr: *mut u8,
// CHECK-NEXT:     _IO_read_end: *mut u8,
// CHECK-NEXT:     _IO_read_base: *mut u8,
// CHECK-NEXT:     _IO_write_base: *mut u8,
// CHECK-NEXT:     _IO_write_ptr: *mut u8,
// CHECK-NEXT:     _IO_write_end: *mut u8,
// CHECK-NEXT:     _IO_buf_base: *mut u8,
// CHECK-NEXT:     _IO_buf_end: *mut u8,
// CHECK-NEXT:     _IO_save_base: *mut u8,
// CHECK-NEXT:     _IO_backup_base: *mut u8,
// CHECK-NEXT:     _IO_save_end: *mut u8,
// CHECK-NEXT:     _markers: *mut _IO_marker,
// CHECK-NEXT:     _chain: *mut _IO_FILE,
// CHECK-NEXT:     _fileno: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT:     _short_backupbuf: [u8; 1],
// CHECK-NEXT:     _old_offset: i64,
// CHECK-NEXT:     _cur_column: u16,
// CHECK-NEXT:     _vtable_offset: i8,
// CHECK-NEXT:     _shortbuf: [u8; 1],
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
// CHECK-NEXT:     _unused2: [u8; 8],
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
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn syscall3(mut x8: i64, mut x0: i64, mut x1: i64, mut x2: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "svc #0", inlateout("x0") x0 as u64 => [[T_0]], in ("x8") x8 as u64, in ("x1")
// CHECK-NEXT:             x1 as u64, in ("x2") x2 as u64, options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         x0 = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x0
// CHECK-NEXT: }
// CHECK-NEXT: fn syscall0(mut x8: i64) -> i64 {
// CHECK-NEXT:     let mut x0: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "svc #0", lateout("x0") [[T_0]], in ("x8") x8 as u64, options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         x0 = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x0
// CHECK-NEXT: }
// CHECK-NEXT: fn add_w1(mut w: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "add w1, w1, {0:w}", in (reg) b as u32, inlateout("x1") w as u32 => [[T_0]],
// CHECK-NEXT:             options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         w = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     w
// CHECK-NEXT: }
// CHECK-NEXT: fn double_d3(mut d: f64) -> f64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fadd d3, d3, d3", inlateout("v3") unsafe { std::mem::transmute::< f64, u64 >
// CHECK-NEXT:             (d) } => [[T_0]], options(pure, nomem, nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         d = unsafe { std::mem::transmute::<u64, f64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     d
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe { fflush(stdout) };
// CHECK-NEXT:     let written: i64 = syscall3(
// CHECK-NEXT:         64i64,
// CHECK-NEXT:         1i64,
// CHECK-NEXT:         (unsafe { std::ptr::addr_of!(message) as *const u8 }) as i64,
// CHECK-NEXT:         15i64,
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%ld %d\n".as_ptr(),
// CHECK-NEXT:             written,
// CHECK-NEXT:             (syscall0(172i64) == (getpid() as i64)) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %.1f\n".as_ptr(), add_w1(40i32, 2i32), double_d3(1.25f64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: static mut message: [u8; 16] = [
// CHECK-NEXT:     104, 101, 108, 108, 111, 32, 102, 114, 111, 109, 32, 115, 118, 99, 10, 0,
// CHECK-NEXT: ];
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
