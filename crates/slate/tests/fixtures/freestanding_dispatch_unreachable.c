/* { dg-options "-ffreestanding" } */
#include <stdio.h>

static int jump_probe(int x) {
  static void *targets[] = {&&zero, &&one, &&two};
  if (x < 0 || x > 2)
    return -1;
  goto *targets[x];

zero:
  return 10;
one:
  return 20;
two:
  return 30;
}

static int retry(int *slot, int n) {
  int tries = 0;
restart:
  if (*slot < n && ++tries < 3) {
    (*slot)++;
    goto restart;
  }
  if (tries == 1)
    goto done;
  *slot += 100;
done:
  return *slot + tries;
}

int main(void) {
  volatile int input = 2;
  int slot = 0;
  int other = 5;
  printf("%d %d %d\n", jump_probe(input), retry(&slot, 4), retry(&other, 1));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-ffreestanding" }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: fn jump_probe(x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         if x < 0 || x > 2 {
// CHECK-NEXT:             return -1;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             match targets.0[x as usize] as usize {
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     return 10;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     return 20;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     return 30;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     core::hint::unreachable_unchecked();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut targets: __SlateAlign16<[*mut (); 3]> =
// CHECK-NEXT:     __SlateAlign16([4usize as *mut (), 5usize as *mut (), 6usize as *mut ()]);
// CHECK-EMPTY:
// CHECK-NEXT: fn retry(slot: *mut i32, n: i32) -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut tries: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         tries = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             if *slot < n {
// CHECK-NEXT:                 let [[V_1:__v[0-9]+]]: i32 = tries + 1;
// CHECK-NEXT:                 tries = [[V_1]];
// CHECK-NEXT:                 [[V_0]] = [[V_1]] < 3;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 [[V_0]] = false;
// CHECK-NEXT:             }
// CHECK-NEXT:             if [[V_0]] {
// CHECK-NEXT:                 *slot = *slot + 1;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if tries != 1 {
// CHECK-NEXT:             *slot = *slot + 100;
// CHECK-NEXT:         }
// CHECK-NEXT:         return *slot + tries;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: extern "C" fn main() -> i32 {
// CHECK-NEXT:     let input: i32 = 2;
// CHECK-NEXT:     let mut slot: i32 = 0;
// CHECK-NEXT:     let mut other: i32 = 5;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %d %d\n".as_ptr(),
// CHECK-NEXT:             jump_probe(core::ptr::read_volatile(core::ptr::addr_of!(input))),
// CHECK-NEXT:             retry(core::ptr::addr_of_mut!(slot), 4i32),
// CHECK-NEXT:             retry(core::ptr::addr_of_mut!(other), 1i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
