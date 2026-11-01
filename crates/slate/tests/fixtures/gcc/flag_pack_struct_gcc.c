/* { dg-options "-fpack-struct" } */
int printf(const char *, ...);
struct ua { char c; int i __attribute__((aligned(8))); };
#pragma pack(4)
struct p4 { char c; double d; };
#pragma pack()
struct reset { char c; double d; };
#pragma pack(push, 8)
struct p8 { char c; double d; };
#pragma pack(pop)
struct popped { char c; double d; };
struct __attribute__((aligned(16))) al16 { char c; int i; };
union u { char c; double d; };
struct bf { char c; int x : 4; int y : 20; };
int main(void) {
  printf("%zu %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu\n",
    sizeof(struct ua), __builtin_offsetof(struct ua, i), _Alignof(struct ua),
    sizeof(struct p4), _Alignof(struct p4), sizeof(struct reset), _Alignof(struct reset),
    sizeof(struct p8), _Alignof(struct p8), sizeof(struct popped), _Alignof(struct popped),
    sizeof(struct al16), _Alignof(struct al16), sizeof(union u), _Alignof(union u),
    sizeof(struct bf), _Alignof(struct bf));
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "-fpack-struct" }
// CHECK-NEXT: #[repr(C, align(8))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct ua {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_pad_1: [u8; 7],
// CHECK-NEXT:     i: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct p4 {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct reset {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct p8 {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct popped {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union u {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct bf {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits7U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits7U0 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub x: i32,
// CHECK-NEXT:         #[bits(20)]
// CHECK-NEXT:         pub y: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%zu %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu | %zu %zu\n"
// CHECK-NEXT:                 .as_ptr(),
// CHECK-NEXT:             std::mem::size_of::<ua>() as u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             8u64,
// CHECK-NEXT:             std::mem::size_of::<p4>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             std::mem::size_of::<reset>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             std::mem::size_of::<p8>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             std::mem::size_of::<popped>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             16u64,
// CHECK-NEXT:             16u64,
// CHECK-NEXT:             std::mem::size_of::<u>() as u64,
// CHECK-NEXT:             1u64,
// CHECK-NEXT:             std::mem::size_of::<bf>() as u64,
// CHECK-NEXT:             1u64,
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
