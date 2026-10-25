int printf(const char *, ...);

#pragma pack(2)
struct __attribute__((packed)) int_bits { char c; int x : 4; int y : 20; };
struct __attribute__((packed)) plain { char c; int i; };
#pragma pack(4)
struct __attribute__((packed)) char_bits { char c; char x : 4; };
struct __attribute__((packed)) wide_bits { char c; long long x : 4; };
struct field_packed { char c; char x : 4 __attribute__((packed)); };
struct __attribute__((packed)) unnamed { char c; int : 4; };
#pragma pack(8)
struct __attribute__((packed)) eight { char c; int x : 4; };

int main(void) {
  struct int_bits b = {1, 5, 1000};
  printf("%zu %zu %zu %zu\n", sizeof(struct int_bits), _Alignof(struct int_bits), sizeof(struct plain),
         _Alignof(struct plain));
  printf("%zu %zu %zu %zu %zu %zu\n", sizeof(struct char_bits), _Alignof(struct char_bits),
         sizeof(struct wide_bits), _Alignof(struct wide_bits), sizeof(struct field_packed),
         _Alignof(struct field_packed));
  printf("%zu %zu %zu %zu\n", sizeof(struct unnamed), _Alignof(struct unnamed), sizeof(struct eight),
         _Alignof(struct eight));
  printf("%d %d %d\n", b.c, b.x, b.y);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(2))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct int_bits {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
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
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub x: i32,
// CHECK-NEXT:         #[bits(20)]
// CHECK-NEXT:         pub y: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: int_bits = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: int_bits = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].c = 1i8;
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_x(5i32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_y(1000i32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 4u64, 2u64, 5u64, 1u64) };
// I686-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 4u32, 2u32, 5u32, 1u32) };
// CHECK-NEXT:     unsafe {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:         printf(c"%zu %zu %zu %zu %zu %zu\n".as_ptr(), 2u64, 1u64, 4u64, 4u64, 2u64, 1u64)
// I686-UNKNOWN-LINUX-GNU-NEXT:         printf(c"%zu %zu %zu %zu %zu %zu\n".as_ptr(), 2u32, 1u32, 4u32, 4u32, 2u32, 1u32)
// CHECK-NEXT:     };
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 2u64, 1u64, 4u64, 4u64) };
// I686-UNKNOWN-LINUX-GNU-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 2u32, 1u32, 4u32, 4u32) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:             b.c as i32,
// CHECK-NEXT:             b.__slate_bits_0.__get_x(),
// CHECK-NEXT:             b.__slate_bits_0.__get_y(),
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
