#include <stdio.h>

#define unlikely(x) __builtin_expect(!!(x), 0)

static volatile int trapped;

#define BUG()                                                                  \
  do {                                                                         \
    do {                                                                       \
      __asm__ volatile("" ::: "memory");                                       \
      trapped = 1;                                                             \
    } while (0);                                                               \
    __builtin_unreachable();                                                   \
  } while (0)

#define BUG_ON(condition)                                                      \
  do {                                                                         \
    if (unlikely(condition))                                                   \
      BUG();                                                                   \
  } while (0)

static int collapse(const int *values, int count, int start) {
  int total = 0;
restart:
  if (start >= count)
    goto end;
  if (values[start] < 0) {
    start++;
    goto restart;
  }
  while (start < count) {
    int copy = values[start];
    BUG_ON(copy < 0);
    if (copy > 9)
      goto end;
    total += copy;
    start++;
  }
end:
  return total;
}

int main(void) {
  int values[] = {-1, 3, 1, 4, 1, 5, 10, 2};
  printf("collapse %d trapped %d\n", collapse(values, 8, 0), trapped);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! unlikely {
// CHECK-NEXT:     ($x:expr) => {
// CHECK-NEXT:         !(!$x) as i64
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_expect([[ARG_0:arg[0-9]+]]: i64, [[ARG_1:arg[0-9]+]]: i64) -> i64;
// CHECK-NEXT:     fn __builtin_unreachable();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: static mut trapped: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn collapse(values: &[i32], mut start: i32) -> i32 {
// CHECK-NEXT:     let count: i32 = values.len() as i32;
// CHECK-NEXT:     let mut copy: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut total: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         total = 0;
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 if start >= count {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if values[start as usize] < 0 {
// CHECK-NEXT:                         start += 1;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         break;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             '__loop_1: loop {
// CHECK-NEXT:                 if start < count {
// CHECK-NEXT:                     copy = values[start as usize];
// CHECK-NEXT:                     loop {
// CHECK-NEXT:                         if unlikely!(copy < 0) != 0 {
// CHECK-NEXT:                             break '__loop_1;
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if true {
// CHECK-NEXT:                                 break;
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                     if copy > 9 {
// CHECK-NEXT:                         break '[[JOIN_0]];
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         total += copy;
// CHECK-NEXT:                         start += 1;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             loop {
// CHECK-NEXT:                 core::arch::asm!("", options(nostack, att_syntax));
// CHECK-NEXT:                 core::ptr::write_volatile(core::ptr::addr_of_mut!(trapped), 1i32);
// CHECK-NEXT:                 if true {
// CHECK-NEXT:                     core::hint::unreachable_unchecked();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return total;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 8]> = __SlateAlign16(
// CHECK-NEXT:         [-1i32, 3i32, 1i32, 4i32, 1i32, 5i32, 10i32, 2i32],
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "collapse {} trapped {}",
// CHECK-NEXT:         collapse(values.0.as_slice(), 0i32),
// CHECK-NEXT:         unsafe { core::ptr::read_volatile(core::ptr::addr_of!(trapped)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
