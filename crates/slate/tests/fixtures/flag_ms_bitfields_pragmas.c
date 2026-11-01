/* { dg-options "-mms-bitfields" } */
int printf(const char *, ...);
struct plain { char a : 3; int b : 4; char c; };
struct __attribute__((gcc_struct)) gs { char a : 3; int b : 4; char c; };
#pragma ms_struct off
struct off { char a : 3; int b : 4; char c; };
#pragma ms_struct reset
struct reset { char a : 3; int b : 4; char c; };
#pragma ms_struct on
struct on { char a : 3; int b : 4; char c; };
int main(void) { printf("%zu %zu %zu %zu %zu\n", sizeof(struct plain), sizeof(struct gs), sizeof(struct off), sizeof(struct reset), sizeof(struct on)); }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-mms-bitfields" }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct plain {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits0U1,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct gs {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct off {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits2U1,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct reset {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits3U0,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits3U1,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct on {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT:     __slate_bits_1: __slate_bits::__SlateBits4U1,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
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
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: i8,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
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
// CHECK-NEXT:     pub struct __SlateBits0U1 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(28, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
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
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: i8,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(1, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
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
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: i8,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
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
// CHECK-NEXT:     pub struct __SlateBits2U1 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(28, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
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
// CHECK-NEXT:     pub struct __SlateBits3U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: i8,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
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
// CHECK-NEXT:     pub struct __SlateBits3U1 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(28, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
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
// CHECK-NEXT:     pub struct __SlateBits4U0 {
// CHECK-NEXT:         #[bits(3)]
// CHECK-NEXT:         pub a: i8,
// CHECK-NEXT:         #[bits(5, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
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
// CHECK-NEXT:     pub struct __SlateBits4U1 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(28, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %zu %zu %zu\n".as_ptr(),
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<plain>() as u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<gs>() as u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<off>() as u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<reset>() as u64,
// X86_64-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<on>() as u64,
// I686-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<plain>() as u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<gs>() as u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<off>() as u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<reset>() as u32,
// I686-UNKNOWN-LINUX-GNU-NEXT:             std::mem::size_of::<on>() as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
