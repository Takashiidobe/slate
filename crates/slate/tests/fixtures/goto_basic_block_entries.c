#include <stdio.h>

static volatile int observed;

static int forwarding_cycle(int stop) {
  if (stop) return 7;
first:
  goto second;
second:
  goto first;
}

static int run(int mode) {
  int sum = 0;
  int visits = 0;
  int checks = 0;
  int steps = 0;
  int i = 0;
  if (mode) goto second;
first:
  sum += 2;
  observed = sum;
  ++visits;
  if (visits < 4) goto alias;
  goto join;
alias:
  goto second;
second:
  sum += 3;
  observed = sum;
  ++visits;
  if (visits < 4) goto first;
join:
  for (; ++checks && i < 4; ++steps, ++i) {
    if (i == 1) continue;
    switch (i) {
    case 0:
      sum += 4;
      goto middle;
    case 2:
      sum += 5;
    middle:
      sum += observed;
      break;
    default:
      goto done;
    }
  }
done:
  printf("%d %d %d %d %d\n", sum, visits, checks, steps, observed);
  return sum;
}

int main(void) {
  run(0);
  run(1);
  printf("%d\n", forwarding_cycle(1));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut observed: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn forwarding_cycle(stop: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         if stop != 0 {
// CHECK-NEXT:             return 7;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             loop {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn run(mode: i32) -> i32 {
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut steps: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut checks: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut visits: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut sum: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 10usize;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             sum = 0;
// CHECK-NEXT:             visits = 0;
// CHECK-NEXT:             checks = 0;
// CHECK-NEXT:             steps = 0;
// CHECK-NEXT:             i = 0;
// CHECK-NEXT:             if mode != 0 {
// CHECK-NEXT:                 __slate_state = 8;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 __slate_state = 9;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 8 => {
// CHECK-NEXT:                     sum += 3;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::ptr::write_volatile(std::ptr::addr_of_mut!(observed), sum)
// CHECK-NEXT:                     };
// CHECK-NEXT:                     visits += 1;
// CHECK-NEXT:                     if visits < 4 {
// CHECK-NEXT:                         __slate_state = 9;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     sum += 2;
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::ptr::write_volatile(std::ptr::addr_of_mut!(observed), sum)
// CHECK-NEXT:                     };
// CHECK-NEXT:                     visits += 1;
// CHECK-NEXT:                     if visits < 4 {
// CHECK-NEXT:                         __slate_state = 8;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '__loop_1: loop {
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: i32 = checks + 1;
// CHECK-NEXT:             checks = [[V_0]];
// CHECK-NEXT:             if [[V_0]] != 0 && i < 4 {
// CHECK-NEXT:                 if i == 1 {} else {
// CHECK-NEXT:                     '[[JOIN_1:__join_[0-9]+]]: {
// CHECK-NEXT:                         let [[T_0:__t[0-9]+]]: i32 = i;
// CHECK-NEXT:                         if [[T_0]] == 0 {
// CHECK-NEXT:                             sum += 4;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_0]] == 2 {
// CHECK-NEXT:                                 sum += 5;
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 break '__loop_1;
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     let [[V_1:__v[0-9]+]]: i32 = sum
// CHECK-NEXT:                         + unsafe {
// CHECK-NEXT:                             std::ptr::read_volatile(std::ptr::addr_of!(observed))
// CHECK-NEXT:                         };
// CHECK-NEXT:                     sum = [[V_1]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 steps += 1;
// CHECK-NEXT:                 i += 1;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {} {} {} {}", sum, visits, checks, steps, unsafe {
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(observed)) }
// CHECK-NEXT:         );
// CHECK-NEXT:         return sum;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     run(0 as i32);
// CHECK-NEXT:     run(1 as i32);
// CHECK-NEXT:     println!("{}", forwarding_cycle(1 as i32));
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
