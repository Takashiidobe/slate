void abort(void);
void exit(int);

struct foo {
  unsigned       : 12;
  unsigned field : 4;
} foo;
unsigned oldfoo;

int bar(unsigned k) {
  oldfoo    = foo.field;
  foo.field = k;
  if (k)
    return 1;
  return 2;
}

int main(void) {
  if (bar(1U) != 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct foo {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
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
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(12, access = na)]
// CHECK-NEXT:         pub __slate_pad_0: u16,
// CHECK-NEXT:         #[bits(4)]
// CHECK-NEXT:         pub field: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut foo: foo = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut oldfoo: u32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn bar(mut k: u32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         oldfoo = ((unsafe { (*std::ptr::addr_of!(foo.__slate_bits_0)).__get_field() })
// CHECK-NEXT:             as i32) as u32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(foo.__slate_bits_0)).__set_field(k);
// CHECK-NEXT:     }
// CHECK-NEXT:     if k != (0 as u32) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     2 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if bar(1 as u32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
