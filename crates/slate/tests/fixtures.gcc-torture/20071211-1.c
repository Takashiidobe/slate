extern void abort() __attribute__((noreturn));

struct s {
  unsigned long long f1 : 40;
#if (__SIZEOF_INT__ >= 4)
  unsigned int f2 : 24;
#else
  unsigned long int f2 : 24;
#endif
} sv;

int main() {
  int f2;
  sv.f2 = (1 << 24) - 1;
  __asm__ volatile("" : : : "memory");
  ++sv.f2;
  f2 = sv.f2;
  if (f2 != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u64,
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
// CHECK-NEXT:         #[bits(40)]
// CHECK-NEXT:         pub f1: u64,
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub f2: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut sv: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut f2: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sv.__slate_bits_0))
// CHECK-NEXT:             .__set_f2((((1 as i32) << (24 as i32)) - (1 as i32)) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = unsafe { (*std::ptr::addr_of!(sv.__slate_bits_0)).__get_f2() };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = (([[V_0]] as i32) + (1 as i32)) as u32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(sv.__slate_bits_0)).__set_f2([[V_1]]);
// CHECK-NEXT:     }
// CHECK-NEXT:     f2 = (unsafe { (*std::ptr::addr_of!(sv.__slate_bits_0)).__get_f2() }) as i32;
// CHECK-NEXT:     if f2 != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
