int printf(const char *, ...);

struct plain { char c; int : 4; };
struct __attribute__((packed)) packed { char c; int : 4; };
struct wide { char c; long long : 4; };
struct zero_width { char c; int : 0; char d; };
#pragma pack(4)
struct __attribute__((packed)) packed_pragma { char c; int : 4; };
struct pragma { char c; long long : 4; };
#pragma pack()
struct named_after { char c; int : 4; short s; };
union unnamed_union { int : 1; };
union named_union { int x : 1; };
union mixed_union { int : 20; char c; };
#pragma pack(1)
union pragma_union { int x : 20; };
#pragma pack()

#define SHOW(T) printf("%s %zu %zu\n", #T, sizeof(struct T), _Alignof(struct T))
#define SHOW_UNION(T) printf("%s %zu %zu\n", #T, sizeof(union T), _Alignof(union T))

int main(void) {
  SHOW(plain);
  SHOW(packed);
  SHOW(wide);
  SHOW(zero_width);
  SHOW(packed_pragma);
  SHOW(pragma);
  SHOW(named_after);
  SHOW_UNION(unnamed_union);
  SHOW_UNION(named_union);
  SHOW_UNION(mixed_union);
  SHOW_UNION(pragma_union);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct plain {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct packed {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct wide {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct zero_width {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_pad_1: [u8; 3],
// CHECK-NEXT:     d: i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct packed_pragma {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits4U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pragma {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits5U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct named_after {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits6U0,
// CHECK-NEXT:     s: i16,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union unnamed_union {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits7U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union named_union {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits8U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union mixed_union {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits9U0,
// CHECK-NEXT:     c: i8,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union pragma_union {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits10U0,
// CHECK-NEXT: }
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
// CHECK-NEXT:         #[bits(8, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u8,
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
// CHECK-NEXT:         #[bits(8, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u8,
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
// CHECK-NEXT:         #[bits(8, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u8,
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
// CHECK-NEXT:         #[bits(8, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u8,
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
// CHECK-NEXT:     pub struct __SlateBits5U0 {
// CHECK-NEXT:         #[bits(8, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u8,
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
// CHECK-NEXT:     pub struct __SlateBits6U0 {
// CHECK-NEXT:         #[bits(8, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u8,
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
// CHECK-NEXT:     pub struct __SlateBits7U0 {
// CHECK-NEXT:         #[bits(8, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u8,
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
// CHECK-NEXT:     pub struct __SlateBits8U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub x: i32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
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
// CHECK-NEXT:         #[bits(24, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u32,
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
// CHECK-NEXT:     pub struct __SlateBits10U0 {
// CHECK-NEXT:         #[bits(20)]
// CHECK-NEXT:         pub x: i32,
// CHECK-NEXT:         #[bits(4, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"plain".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<plain>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"packed".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<packed>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"wide".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<wide>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"zero_width".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<zero_width>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"packed_pragma".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<packed_pragma>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"pragma".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<pragma>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"named_after".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<named_after>() as u64,
// CHECK-NEXT:             2u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"unnamed_union".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<unnamed_union>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"named_union".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<named_union>() as u64,
// CHECK-NEXT:             4u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"mixed_union".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<mixed_union>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %zu %zu\n".as_ptr(),
// CHECK-NEXT:             c"pragma_union".as_ptr() as *mut i8,
// CHECK-NEXT:             std::mem::size_of::<pragma_union>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:         )
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
