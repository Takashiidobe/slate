#include <stdio.h>

struct Flags {
  unsigned  x : 3;
  unsigned  set_x : 3;
  int       a : 4;
  unsigned  A : 4;
  int       type : 5;
  int       : 4;
  _Bool     ready : 1;
  char      tag;
  int       _flags2 : 24;
  long long wide : 40;
  _Bool     last : 1;
  int       : 0;
  short     after;
};

union Overlay {
  unsigned whole;
  struct {
    unsigned low : 12;
    unsigned high : 20;
  } parts;
  int signed_bits : 7;
};

static struct Flags global = {5, 2, -3, 9, -11, 1, 'g', -200, -5000000000LL, 1, 77};

static void bump(struct Flags *flags) {
  flags->a += 3;
  flags->wide++;
  flags->_flags2 = flags->_flags2 * 2;
  flags->ready = !flags->ready;
}

int main(void) {
  struct Flags local = {0};
  local.x       = 9;
  local.set_x   = 7;
  local.a       = -8;
  local.A       = 15;
  local.type    = 15;
  local.ready   = 1;
  local.tag     = 't';
  local._flags2 = 0x7fffff;
  local.wide    = 549755813887LL;
  local.last    = 1;
  local.after   = -2;
  int assigned  = (local.x = 12);
  printf("%d %u %u %d %u %d %d %c %d %lld %d %d %d\n", assigned, local.x, local.set_x,
         local.a, local.A, local.type, local.ready, local.tag, local._flags2,
         local.wide, local.last, local.after, (int)sizeof(struct Flags));
  bump(&local);
  bump(&global);
  printf("%d %lld %d %d\n", local.a, local.wide, local._flags2, local.ready);
  printf("%u %u %d %u %d %d %c %d %lld %d %d\n", global.x, global.set_x, global.a,
         global.A, global.type, global.ready, global.tag, global._flags2, global.wide,
         global.last, global.after);

  union Overlay overlay;
  overlay.whole = 0xabcdef12u;
  printf("%x %x %d\n", overlay.parts.low, overlay.parts.high, overlay.signed_bits);
  overlay.parts.high = 0x12345;
  overlay.signed_bits = -1;
  printf("%x %d\n", overlay.whole, (int)sizeof(union Overlay));

  fprintf(stdout, "%s\n", "file ok");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     static mut stdout: *mut _IO_FILE;
// CHECK-NEXT:     fn fprintf(__stream: *mut _IO_FILE, __format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
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
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
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
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Flags {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits9U0,
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits9U1,
// CHECK-NEXT:     __slate_pad_3: [u8; 1],
// CHECK-NEXT:     __slate_bits_2: __slate_bits::__SlateBits9U2,
// CHECK-NEXT:     __slate_pad_5: [u8; 2],
// CHECK-NEXT:     after: i16,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Overlay {
// CHECK-NEXT:     whole: u32,
// CHECK-NEXT:     parts: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits10U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits11U0,
// CHECK-NEXT: }
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
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits9U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub x: u32,
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub set_x: u32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub A: u32,
// CHECK-NEXT:         #[bits(5)]
// CHECK-NEXT:         pub r#type: i32,
// CHECK-NEXT:         #[bits(4, access = na)]
// CHECK-NEXT:         pub __slate_pad_5: u8,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub ready: bool,
// CHECK-NEXT:     }
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
// CHECK-NEXT:     pub struct __SlateBits9U1 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub _flags2: i32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;6],
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits9U2 {
// CHECK-NEXT:         #[bits(40)]
// CHECK-NEXT:         pub wide: i64,
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub last: bool,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits11U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub low: u32,
// CHECK-NEXT:         #[bits(20)]
// CHECK-NEXT:         pub high: u32,
// CHECK-NEXT:     }
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits10U0 {
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub signed_bits: i32,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut global: Flags = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: Flags = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_x((5 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_set_x((2 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(3 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_A((9 as i32) as u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_type(-(11 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_ready((1 as i32) != (0 as i32));
// CHECK-NEXT:     [[T_0]].tag = (103 as i32) as i8;
// CHECK-NEXT:     [[T_0]].__slate_bits_1.__set__flags2(-(200 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_2.__set_wide(-(5000000000 as i64));
// CHECK-NEXT:     [[T_0]].__slate_bits_2.__set_last((1 as i32) != (0 as i32));
// CHECK-NEXT:     [[T_0]].after = (77 as i32) as i16;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn bump(flags: *mut Flags) {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe { (*flags).__slate_bits_0.__get_a() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*flags).__slate_bits_0.__set_a([[V_0]] + 3);
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: i64 = unsafe { (*flags).__slate_bits_2.__get_wide() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*flags).__slate_bits_2.__set_wide([[V_1]] + 1);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*flags)
// CHECK-NEXT:             .__slate_bits_1
// CHECK-NEXT:             .__set__flags2((unsafe { (*flags).__slate_bits_1.__get__flags2() }) * 2);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*flags)
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__set_ready(
// CHECK-NEXT:                 !(((unsafe { (*flags).__slate_bits_0.__get_ready() }) as i32) != 0),
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut local: Flags = {
// CHECK-NEXT:         let mut [[T_0]]: Flags = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_x(0 as u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     local.__slate_bits_0.__set_x(9 as u32);
// CHECK-NEXT:     local.__slate_bits_0.__set_set_x(7 as u32);
// CHECK-NEXT:     local.__slate_bits_0.__set_a(-(8 as i32));
// CHECK-NEXT:     local.__slate_bits_0.__set_A(15 as u32);
// CHECK-NEXT:     local.__slate_bits_0.__set_type(15 as i32);
// CHECK-NEXT:     local.__slate_bits_0.__set_ready((1 as i32) != (0 as i32));
// CHECK-NEXT:     local.tag = 116;
// CHECK-NEXT:     local.__slate_bits_1.__set__flags2(8388607 as i32);
// CHECK-NEXT:     local.__slate_bits_2.__set_wide(549755813887 as i64);
// CHECK-NEXT:     local.__slate_bits_2.__set_last((1 as i32) != (0 as i32));
// CHECK-NEXT:     local.after = -2;
// CHECK-NEXT:     let assigned: i32 = (((12 as u8) & (7 as u8)) as u32) as i32;
// CHECK-NEXT:     local.__slate_bits_0.__set_x(12 as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %u %u %d %u %d %d %c %d %lld %d %d %d\n".as_ptr(),
// CHECK-NEXT:             assigned,
// CHECK-NEXT:             local.__slate_bits_0.__get_x() as i32,
// CHECK-NEXT:             local.__slate_bits_0.__get_set_x() as i32,
// CHECK-NEXT:             local.__slate_bits_0.__get_a(),
// CHECK-NEXT:             local.__slate_bits_0.__get_A() as i32,
// CHECK-NEXT:             local.__slate_bits_0.__get_type(),
// CHECK-NEXT:             local.__slate_bits_0.__get_ready() as i32,
// CHECK-NEXT:             local.tag as i32,
// CHECK-NEXT:             local.__slate_bits_1.__get__flags2(),
// CHECK-NEXT:             local.__slate_bits_2.__get_wide(),
// CHECK-NEXT:             local.__slate_bits_2.__get_last() as i32,
// CHECK-NEXT:             local.after as i32,
// CHECK-NEXT:             24 as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     bump(std::ptr::addr_of_mut!(local));
// CHECK-NEXT:     bump(unsafe { std::ptr::addr_of_mut!(global) });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %lld %d %d\n".as_ptr(),
// CHECK-NEXT:             local.__slate_bits_0.__get_a(),
// CHECK-NEXT:             local.__slate_bits_2.__get_wide(),
// CHECK-NEXT:             local.__slate_bits_1.__get__flags2(),
// CHECK-NEXT:             local.__slate_bits_0.__get_ready() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %d %u %d %d %c %d %lld %d %d\n".as_ptr(),
// CHECK-NEXT:             (unsafe { (*std::ptr::addr_of!(global.__slate_bits_0)).__get_x() }) as i32,
// CHECK-NEXT:             (unsafe { (*std::ptr::addr_of!(global.__slate_bits_0)).__get_set_x() })
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             unsafe { (*std::ptr::addr_of!(global.__slate_bits_0)).__get_a() },
// CHECK-NEXT:             (unsafe { (*std::ptr::addr_of!(global.__slate_bits_0)).__get_A() }) as i32,
// CHECK-NEXT:             unsafe { (*std::ptr::addr_of!(global.__slate_bits_0)).__get_type() },
// CHECK-NEXT:             (unsafe { (*std::ptr::addr_of!(global.__slate_bits_0)).__get_ready() })
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             (unsafe { global.tag }) as i32,
// CHECK-NEXT:             unsafe { (*std::ptr::addr_of!(global.__slate_bits_1)).__get__flags2() },
// CHECK-NEXT:             unsafe { (*std::ptr::addr_of!(global.__slate_bits_2)).__get_wide() },
// CHECK-NEXT:             (unsafe { (*std::ptr::addr_of!(global.__slate_bits_2)).__get_last() })
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             (unsafe { global.after }) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut overlay: Overlay = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         overlay.whole = 2882400018;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%x %x %d\n".as_ptr(),
// CHECK-NEXT:             (unsafe { overlay.parts.__slate_bits_0.__get_low() }) as i32,
// CHECK-NEXT:             (unsafe { overlay.parts.__slate_bits_0.__get_high() }) as i32,
// CHECK-NEXT:             unsafe { overlay.__slate_bits_0.__get_signed_bits() },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         overlay.parts.__slate_bits_0.__set_high(74565 as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         overlay.__slate_bits_0.__set_signed_bits(-(1 as i32));
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%x %d\n".as_ptr(), unsafe { overlay.whole }, 4 as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         fprintf(unsafe { stdout }, c"%s\n".as_ptr(), c"file ok".as_ptr() as *mut i8)
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
