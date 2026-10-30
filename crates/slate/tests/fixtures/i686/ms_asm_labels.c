/* { dg-options "-fms-extensions" } */
#include <stdio.h>

static int loop_label(int n) {
  int acc = 0;
  __asm {
    mov ecx, n
    xor eax, eax
  top:
    add eax, ecx
    dec ecx
    jnz top
    mov acc, eax
  }
  return acc;
}

static int forward(int v) {
  int out = 0;
  __asm {
    mov eax, v
    test eax, eax
    jz done
    mov eax, 100
  done:
    mov out, eax
  }
  return out;
}

static int two_blocks(int n) {
  int out = 0;
  __asm {
    mov ecx, n
    xor eax, eax
  again:
    inc eax
    dec ecx
    jnz again
    mov out, eax
  }
  __asm {
    mov ecx, 3
  again:
    inc out
    dec ecx
    jnz again
  }
  return out;
}

int main(void) {
  printf("%d %d %d %d\n", loop_label(4), forward(0), forward(5), two_blocks(2));
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
// CHECK-NEXT: fn loop_label(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     let mut acc: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov ecx, [{0}]\nxor eax, eax\n2:\nadd eax, ecx\ndec ecx\njnz 2b\nmov [{1}], eax", in(reg) std::ptr::addr_of!(n), in(reg) std::ptr::addr_of_mut!(acc), out("ax") [[T_0]], out("cx") _);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     acc
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn forward(mut v: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0]]: u32 = 0;
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov eax, [{0}]\ntest eax, eax\njz 2f\nmov eax, 100\n2:\nmov [{1}], eax", in(reg) std::ptr::addr_of!(v), in(reg) std::ptr::addr_of_mut!(out), out("ax") [[T_0]]);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn two_blocks(mut n: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0]]: u32 = 0;
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov ecx, [{0}]\nxor eax, eax\n2:\ninc eax\ndec ecx\njnz 2b\nmov [{1}], eax", in(reg) std::ptr::addr_of!(n), in(reg) std::ptr::addr_of_mut!(out), out("ax") [[T_0]], out("cx") _);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov ecx, 3\n2:\ninc dword ptr [{0}]\ndec ecx\njnz 2b", in(reg) std::ptr::addr_of_mut!(out), lateout("ax") [[T_1]], out("cx") _);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}",
// CHECK-NEXT:         loop_label(4i32),
// CHECK-NEXT:         forward(0i32),
// CHECK-NEXT:         forward(5i32),
// CHECK-NEXT:         two_blocks(2i32)
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
