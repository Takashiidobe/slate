/* { dg-options "-fms-extensions" } */
#include <stdio.h>
int sink;
static void bump(void) { sink += 100; }
static void call_fn(void) {
  __asm {
    call bump
  }
}
static int clobbers(void) {
  int r = 5;
  __asm {
    mov ecx, 3
    mov edx, 4
    add ecx, edx
  }
  return r + 1;
}
static int stack_balance(void) {
  int r = 9;
  __asm {
    push 7
    push 8
    pop ecx
    pop edx
    add ecx, edx
  }
  return r;
}
int main(void) {
  call_fn();
  printf("%d %d\n", sink, clobbers());
  printf("%d\n", stack_balance());
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
// CHECK-NEXT: static mut sink: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.bump")]
// CHECK-NEXT: extern "C-unwind" fn bump() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sink = sink + 100;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn call_fn() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("call {0}", sym bump, out("ax") _, out("cx") _, out("dx") _);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn clobbers() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u32 = 0;
// CHECK-NEXT:     let r: i32 = 5;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!("mov ecx, 3\nmov edx, 4\nadd ecx, edx", lateout("ax") [[T_0]], lateout("cx") _, lateout("dx") _);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r + 1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn stack_balance() -> i32 {
// CHECK-NEXT:     let mut [[V_0]]: u32 = 0;
// CHECK-NEXT:     let r: i32 = 9;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("push 7\npush 8\npop ecx\npop edx\nadd ecx, edx", lateout("ax") [[T_0]], lateout("cx") _, lateout("dx") _);
// CHECK-NEXT:         [[V_0]] = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     call_fn();
// CHECK-NEXT:     println!("{} {}", unsafe { sink }, clobbers());
// CHECK-NEXT:     println!("{}", stack_balance());
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
