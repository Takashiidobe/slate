#include <stdio.h>
struct padded { unsigned char first; unsigned short second; };
struct flags { _Bool first; unsigned char second; };
struct bits { unsigned low : 3; unsigned high : 7; unsigned short tail; };
union partial { unsigned char byte; unsigned int word; };
struct nested { struct flags inner; _Bool done[2]; };
struct wide { char tag; int value; };
int main(void) {
  struct padded padded = { 1, 2 };
  struct flags flags = { 1, 2 };
  __asm__("" : "+r"(padded));
  __asm__("" : "+r"(flags));
  printf("%u %u %d %u\n", padded.first, padded.second, flags.first, flags.second);
  __asm__("addl $0x00030000, %0" : "+r"(padded));
  printf("%u %u\n", padded.first, padded.second);
  struct flags set;
  __asm__("movw $0x2a00, %w0\n\tcmpb $1, %b1\n\tsete %b0" : "=&r"(set) : "r"(5));
  printf("%d %u\n", set.first, set.second);
  struct bits bits = { 5, 100, 7 };
  __asm__("xorl $0x00010009, %0" : "+r"(bits));
  printf("%u %u %u\n", bits.low, bits.high, bits.tail);
  union partial partial;
  partial.word = 0;
  partial.byte = 9;
  unsigned int seen;
  __asm__("movl %1, %0" : "=r"(seen) : "r"(partial));
  printf("%u\n", seen);
  __asm__("movl $0x01020304, %0" : "=r"(partial));
  printf("%u %u\n", partial.byte, partial.word);
  struct nested nested = { { 0, 3 }, { 1, 0 } };
  __asm__("xorl $0x01000101, %0" : "+r"(nested));
  printf("%d %u %d %d\n", nested.inner.first, nested.inner.second, nested.done[0],
         nested.done[1]);
  struct wide wide = { 'a', -7 };
  struct wide copy;
  __asm__("movq %1, %0\n\taddq $1, %0" : "=r"(copy) : "r"(wide));
  printf("%c %d\n", copy.tag, copy.value);
  return 0;
}

// SLATE-FILECHECK-BEGIN
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
// CHECK-NEXT: struct padded {
// CHECK-NEXT:     first: u8,
// CHECK-NEXT:     second: u16,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct flags {
// CHECK-NEXT:     first: bool,
// CHECK-NEXT:     second: u8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bits {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT:     tail: u16,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union partial {
// CHECK-NEXT:     byte: u8,
// CHECK-NEXT:     word: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct nested {
// CHECK-NEXT:     inner: flags,
// CHECK-NEXT:     done: [bool; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct wide {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u16,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub low: u32,
// CHECK-NEXT:         #[bits(7)]
// CHECK-NEXT:         pub high: u32,
// CHECK-NEXT:         #[bits(6, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut padded: padded = padded { first: 1, second: 2 };
// CHECK-NEXT:     let mut flags: flags = flags {
// CHECK-NEXT:         first: (1 as i32) != (0 as i32),
// CHECK-NEXT:         second: 2,
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "\n# {0:e}", inlateout(reg) unsafe { std::mem::transmute::< padded, u32 >
// CHECK-NEXT:             (padded) } => [[T_0]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         padded = unsafe { std::mem::transmute::<u32, padded>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u16;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "\n# {0:x}", inlateout(reg) unsafe { std::mem::transmute::< flags, u16 >
// CHECK-NEXT:             (flags) } => [[T_1]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         flags = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 u16,
// CHECK-NEXT:                 flags,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ([[T_1]] as u16) & 65280u16
// CHECK-NEXT:                     | ((((([[T_1]] as u16) >> 0) as u8) != 0) as u16) << 0,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %d %u\n".as_ptr(),
// CHECK-NEXT:             (padded.first as u32) as i32,
// CHECK-NEXT:             (padded.second as u32) as i32,
// CHECK-NEXT:             flags.first as i32,
// CHECK-NEXT:             (flags.second as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $0x00030000, {0:e}", inlateout(reg) unsafe { std::mem::transmute::<
// CHECK-NEXT:             padded, u32 > (padded) } => [[T_2]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         padded = unsafe { std::mem::transmute::<u32, padded>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u\n".as_ptr(),
// CHECK-NEXT:             (padded.first as u32) as i32,
// CHECK-NEXT:             (padded.second as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut set: flags = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u16;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movw $0x2a00, {0:x}\n\tcmpb $1, {1:l}\n\tsete {0:l}", out(reg) [[T_3]], in
// CHECK-NEXT:             (reg) (5 as i32) as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         set = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 u16,
// CHECK-NEXT:                 flags,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ([[T_3]] as u16) & 65280u16
// CHECK-NEXT:                     | ((((([[T_3]] as u16) >> 0) as u8) != 0) as u16) << 0,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %u\n".as_ptr(), set.first as i32, (set.second as u32) as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut bits: bits = {
// CHECK-NEXT:         let mut [[T_4:__t[0-9]+]]: bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_4]].__slate_bits_0.__set_low(5 as u32);
// CHECK-NEXT:         [[T_4]].__slate_bits_0.__set_high(100 as u32);
// CHECK-NEXT:         [[T_4]].tail = 7 as u16;
// CHECK-NEXT:         [[T_4]]
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xorl $0x00010009, {0:e}", inlateout(reg) unsafe { std::mem::transmute::<
// CHECK-NEXT:             bits, u32 > (bits) } => [[T_5]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         bits = unsafe { std::mem::transmute::<u32, bits>([[T_5]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %u\n".as_ptr(),
// CHECK-NEXT:             bits.__slate_bits_0.__get_low() as i32,
// CHECK-NEXT:             bits.__slate_bits_0.__get_high() as i32,
// CHECK-NEXT:             (bits.tail as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut partial: partial = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         partial.word = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         partial.byte = 9;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut seen: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl {1:e}, {0:e}", lateout(reg) [[T_6]], in (reg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< partial, u32 > (partial) }, options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         seen = unsafe { std::mem::transmute::<u32, u32>([[T_6]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), seen) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_7:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $0x01020304, {0:e}", lateout(reg) [[T_7]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         partial = unsafe { std::mem::transmute::<u32, partial>([[T_7]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u\n".as_ptr(),
// CHECK-NEXT:             ((unsafe { partial.byte }) as u32) as i32,
// CHECK-NEXT:             unsafe { partial.word },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut nested: nested = nested {
// CHECK-NEXT:         inner: flags {
// CHECK-NEXT:             first: (0 as i32) != (0 as i32),
// CHECK-NEXT:             second: 3,
// CHECK-NEXT:         },
// CHECK-NEXT:         done: [(1 as i32) != (0 as i32), (0 as i32) != (0 as i32)],
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_8:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xorl $0x01000101, {0:e}", inlateout(reg) unsafe { std::mem::transmute::<
// CHECK-NEXT:             nested, u32 > (nested) } => [[T_8]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         nested = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 u32,
// CHECK-NEXT:                 nested,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ([[T_8]] as u32) & 65280u32
// CHECK-NEXT:                     | ((((([[T_8]] as u32) >> 0) as u8) != 0) as u32) << 0
// CHECK-NEXT:                     | ((((([[T_8]] as u32) >> 16) as u8) != 0) as u32) << 16
// CHECK-NEXT:                     | ((((([[T_8]] as u32) >> 24) as u8) != 0) as u32) << 24,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %u %d %d\n".as_ptr(),
// CHECK-NEXT:             nested.inner.first as i32,
// CHECK-NEXT:             (nested.inner.second as u32) as i32,
// CHECK-NEXT:             nested.done[0] as i32,
// CHECK-NEXT:             nested.done[1] as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut wide: wide = wide { tag: 97, value: -7 };
// CHECK-NEXT:     let mut copy: wide = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_9:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq {1:r}, {0:r}\n\taddq $1, {0:r}", lateout(reg) [[T_9]], in (reg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< wide, u64 > (wide) }, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         copy = unsafe { std::mem::transmute::<u64, wide>([[T_9]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%c %d\n".as_ptr(), copy.tag as i32, copy.value) };
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
