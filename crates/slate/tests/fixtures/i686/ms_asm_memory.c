/* { dg-options "-fms-extensions" } */
#include <stdio.h>

static int add_mem(int a, int b) {
  int result;
  __asm {
    mov eax, a
    add eax, b
    mov result, eax
  }
  return result;
}

static int field_and_index(void) {
  struct { int x; int y; } s = { 3, 4 };
  int arr[4] = { 10, 20, 30, 40 };
  int out;
  __asm {
    mov eax, s.y
    add eax, arr[8]
    mov out, eax
  }
  return out;
}

static int sized(void) {
  unsigned char bytes[4] = { 1, 2, 3, 4 };
  unsigned out;
  __asm {
    movzx eax, byte ptr bytes[2]
    mov out, eax
  }
  return (int)out;
}

static int indexed(int i) {
  int arr[4] = { 5, 6, 7, 8 };
  int out;
  __asm {
    mov ecx, i
    mov eax, arr[ecx*4]
    mov out, eax
  }
  return out;
}

int main(void) {
  printf("%d %d %d %d\n", add_mem(2, 5), field_and_index(), sized(), indexed(2));
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fms-extensions" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_ms_asm_memory_c_15 {
// CHECK-NEXT:     x: i32,
// CHECK-NEXT:     y: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn add_mem(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov eax, [{0}]\nadd eax, [{1}]\nmov [{2}], eax", in(reg) std::ptr::addr_of!(a), in(reg) std::ptr::addr_of!(b), in(reg) std::ptr::addr_of_mut!(result), out("ax") [[T_0]]);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn field_and_index() -> i32 {
// CHECK-NEXT:     let mut [[V_0]]: u32 = 0;
// CHECK-NEXT:     let mut s: __SlateRecord_ms_asm_memory_c_15 = __SlateRecord_ms_asm_memory_c_15 { x: 3, y: 4 };
// CHECK-NEXT:     let mut arr: [i32; 4] = [10, 20, 30, 40];
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov eax, [{0}+4]\nadd eax, [{1}+8]\nmov [{2}], eax", in(reg) std::ptr::addr_of!(s), in(reg) std::ptr::addr_of!(arr), in(reg) std::ptr::addr_of_mut!(out), out("ax") [[T_0]]);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sized() -> i32 {
// CHECK-NEXT:     let mut [[V_0]]: u32 = 0;
// CHECK-NEXT:     let mut bytes: [u8; 4] = [1, 2, 3, 4];
// CHECK-NEXT:     let mut out: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("movzx eax, byte ptr [{0}+2]\nmov [{1}], eax", in(reg) std::ptr::addr_of!(bytes), in(reg) std::ptr::addr_of_mut!(out), out("ax") [[T_0]]);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn indexed(mut i: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0]]: u32 = 0;
// CHECK-NEXT:     let mut arr: [i32; 4] = [5, 6, 7, 8];
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov ecx, [{0}]\nmov eax, [{1}+ecx*4]\nmov [{2}], eax", in(reg) std::ptr::addr_of!(i), in(reg) std::ptr::addr_of!(arr), in(reg) std::ptr::addr_of_mut!(out), out("ax") [[T_0]], out("cx") _);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}",
// CHECK-NEXT:         add_mem(2i32, 5i32),
// CHECK-NEXT:         field_and_index(),
// CHECK-NEXT:         sized(),
// CHECK-NEXT:         indexed(2i32)
// CHECK-NEXT:     );
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
