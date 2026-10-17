/* { dg-options "-fms-extensions" } */
#include <stdio.h>

static int implicit_return(int v) {
  __asm mov eax, v
  __asm shl eax, 1
}

static unsigned long long implicit_return64(void) {
  __asm {
    mov eax, 0x11223344
    mov edx, 0x55667788
  }
}

static unsigned char narrow(int v) {
  __asm mov eax, v
}

static short narrow16(int v) {
  __asm {
    mov eax, v
    add eax, 1
  }
}

static int global_value = 5;

static int *pointer(void) {
  __asm lea eax, global_value
}

static int explicit_wins(int v) {
  __asm mov eax, 99
  return v;
}

int main(void) {
  printf("%d %llx %u %d\n", implicit_return(21), implicit_return64(), narrow(0x1ff), narrow16(0x7fff));
  printf("%d %d\n", *pointer(), explicit_wins(3));
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fms-extensions" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn implicit_return(mut v: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov eax, [{0}]\nshl eax, 1", in(reg) std::ptr::addr_of!(v), out("ax") [[T_0]]);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]] as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn implicit_return64() -> u64 {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov eax, 287454020\nmov edx, 1432778632", out("ax") [[T_0]], out("dx") [[T_1]]);
// CHECK-NEXT:         [[V_1]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:         [[V_2]] = unsafe { std::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     ([[V_1]] as u64) | ([[V_2]] as u64) << (32 as u32)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn narrow(mut v: i32) -> u8 {
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov eax, [{0}]", in(reg) std::ptr::addr_of!(v), out("ax") [[T_0]]);
// CHECK-NEXT:         [[V_3]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_3]] as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn narrow16(mut v: i32) -> i16 {
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov eax, [{0}]\nadd eax, 1", in(reg) std::ptr::addr_of!(v), out("ax") [[T_0]]);
// CHECK-NEXT:         [[V_4]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     ([[V_4]] as u16) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut global_value: i32 = 5 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn pointer() -> *mut i32 {
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("lea eax, [{0}]", in(reg) std::ptr::addr_of!(global_value), out("ax") [[T_0]]);
// CHECK-NEXT:         [[V_5]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_5]] as *mut i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn explicit_wins(v: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov eax, 99", out("ax") [[T_0]]);
// CHECK-NEXT:         [[V_6]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %llx %u %d\n".as_ptr(),
// CHECK-NEXT:             implicit_return(21 as i32),
// CHECK-NEXT:             implicit_return64(),
// CHECK-NEXT:             (narrow(511 as i32) as u32) as i32,
// CHECK-NEXT:             narrow16(32767 as i32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{} {}", unsafe { *pointer() }, explicit_wins(3 as i32));
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
