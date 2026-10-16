/* { dg-require-effective-target int32plus } */
#pragma pack(1)
struct S {
  int f0 : 15;
  int f1 : 29;
};

int             e = 1, i;
static struct S d[6];

int main(void) {
  if (e) {
    d[i].f0 = 1;
    d[i].f1 = 1;
  }
  if (d[0].f1 != 1)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 6], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub f0: i32,
// CHECK-NEXT:         #[bits(29)]
// CHECK-NEXT:         pub f1: i32,
// CHECK-NEXT:         #[bits(4, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: __SlateAlign16<[S; 6]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { e }) != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(d.0) as *mut S }.offset((unsafe { i }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__set_f0(1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(d.0) as *mut S }.offset((unsafe { i }) as isize)
// CHECK-NEXT:             })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__set_f1(1 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         (*unsafe { unsafe { std::ptr::addr_of_mut!(d.0) as *mut S }.offset((0 as i32) as isize) })
// CHECK-NEXT:             .__slate_bits_0
// CHECK-NEXT:             .__get_f1()
// CHECK-NEXT:     }) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
// SLATE-FILECHECK-END
