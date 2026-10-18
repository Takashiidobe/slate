#include <stdint.h>
#include <stdio.h>

register uintptr_t stack_pointer __asm__("rsp");
register char *stack_pointer_bytes __asm__("rsp");

static int near(uintptr_t sp, const void *local) {
  uintptr_t here = (uintptr_t)local;
  uintptr_t distance = here > sp ? here - sp : sp - here;
  return sp != 0 && sp % 8 == 0 && distance < 65536;
}

static int read_integer(void) {
  int local = 1;
  return near(stack_pointer, &local);
}

static int read_pointer(void) {
  int local = 2;
  return near((uintptr_t)stack_pointer_bytes, &local);
}

int main(void) {
  printf("%d %d\n", read_integer(), read_pointer());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn near(sp: u64, local: *const ()) -> i32 {
// CHECK-NEXT:     let here: u64 = local as u64;
// CHECK-NEXT:     let distance: u64 = if here > sp {
// CHECK-NEXT:         here.wrapping_sub(sp)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         sp.wrapping_sub(here)
// CHECK-NEXT:     };
// CHECK-NEXT:     (sp != 0 && sp % 8 == 0 && distance < 65536) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn read_integer() -> i32 {
// CHECK-NEXT:     let mut local: i32 = 1;
// CHECK-NEXT:     near(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]]: usize;
// CHECK-NEXT:             core::arch::asm!("mov {}, rsp", lateout(reg) [[T_0]]);
// CHECK-NEXT:             [[T_0]] as u64
// CHECK-NEXT:         },
// CHECK-NEXT:         std::ptr::addr_of_mut!(local) as *const (),
// CHECK-NEXT:     )
// CHECK-NEXT: }
// CHECK-NEXT: fn read_pointer() -> i32 {
// CHECK-NEXT:     let mut local: i32 = 2;
// CHECK-NEXT:     near(
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             let [[T_0]]: usize;
// CHECK-NEXT:             core::arch::asm!("mov {}, rsp", lateout(reg) [[T_0]]);
// CHECK-NEXT:             [[T_0]] as *mut i8
// CHECK-NEXT:         }) as u64,
// CHECK-NEXT:         std::ptr::addr_of_mut!(local) as *const (),
// CHECK-NEXT:     )
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {}", read_integer(), read_pointer());
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
