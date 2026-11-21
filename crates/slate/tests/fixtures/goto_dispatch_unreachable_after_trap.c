#include <stdio.h>

#define unlikely(x) __builtin_expect(!!(x), 0)
#define trace(...) ({ if (0) printf(__VA_ARGS__); 0; })

static volatile int trapped;

#define TRAP()                                                                 \
  do {                                                                         \
    __asm__ volatile("" ::: "memory");                                         \
    trapped = 1;                                                               \
  } while (0)

#define BUG()                                                                  \
  do {                                                                         \
    TRAP();                                                                    \
    __builtin_unreachable();                                                   \
  } while (0)

#define BUG_ON(condition)                                                      \
  do {                                                                         \
    if (unlikely(condition))                                                   \
      BUG();                                                                   \
  } while (0)

static int walk(const int *values, int count) {
  int level = 0, sc_level = 0, steps = 0;
jumped:
  if (level & 1)
  consider:
    trace("consider %d\n", level);
  if (++steps > count)
    return level;
  sc_level = level + values[steps % count];
  BUG_ON(sc_level < level);
  do {
    sc_level += 2;
  } while (sc_level < level + 3);
  if ((level ^ sc_level) & 4) {
    level = sc_level;
    goto jumped;
  } else {
    level = sc_level;
    goto consider;
  }
}

int main(void) {
  int values[] = {3, 1, 4, 1, 5};
  printf("walk %d trapped %d\n", walk(values, 5), trapped);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
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
// CHECK-NEXT: fn walk(values: &[i32]) -> i32 {
// CHECK-NEXT:     let count: i32 = values.len() as i32;
// CHECK-NEXT:     let mut steps: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut sc_level: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut level: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut __slate_state: usize = 12;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             match __slate_state {
// CHECK-NEXT:                 0 => {
// CHECK-NEXT:                     level = sc_level;
// CHECK-NEXT:                     __slate_state = 11;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 1 => {
// CHECK-NEXT:                     level = sc_level;
// CHECK-NEXT:                     __slate_state = 10;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 2 => {
// CHECK-NEXT:                     if (level ^ sc_level) & 4 != 0 {
// CHECK-NEXT:                         __slate_state = 0;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 1;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 3 => {
// CHECK-NEXT:                     sc_level += 2;
// CHECK-NEXT:                     if sc_level < level + 3 {
// CHECK-NEXT:                         __slate_state = 3;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 2;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 4 => {
// CHECK-NEXT:                     if false {
// CHECK-NEXT:                         __slate_state = 6;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 3;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 5 => {
// CHECK-NEXT:                     core::arch::asm!("", options(nostack, att_syntax));
// CHECK-NEXT:                     core::ptr::write_volatile(core::ptr::addr_of_mut!(trapped), 1i32);
// CHECK-NEXT:                     if false {
// CHECK-NEXT:                         __slate_state = 5;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         core::hint::unreachable_unchecked();
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 6 => {
// CHECK-NEXT:                     if unlikely!(sc_level < level) != 0 {
// CHECK-NEXT:                         __slate_state = 5;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 4;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 7 => {
// CHECK-NEXT:                     sc_level = level + values[(steps % count) as usize];
// CHECK-NEXT:                     __slate_state = 6;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 8 => {
// CHECK-NEXT:                     return level;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 9 => {
// CHECK-NEXT:                     let [[V_0:__v[0-9]+]]: i32 = steps + 1;
// CHECK-NEXT:                     steps = [[V_0]];
// CHECK-NEXT:                     if [[V_0]] > count {
// CHECK-NEXT:                         __slate_state = 8;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 7;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 10 => {
// CHECK-NEXT:                     __slate_state = 9;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 11 => {
// CHECK-NEXT:                     if level & 1 != 0 {
// CHECK-NEXT:                         __slate_state = 10;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     } else {
// CHECK-NEXT:                         __slate_state = 9;
// CHECK-NEXT:                         continue;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 12 => {
// CHECK-NEXT:                     level = 0;
// CHECK-NEXT:                     sc_level = 0;
// CHECK-NEXT:                     steps = 0;
// CHECK-NEXT:                     __slate_state = 11;
// CHECK-NEXT:                     continue;
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {
// CHECK-NEXT:                     unreachable!();
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 5]> = __SlateAlign16(
// CHECK-NEXT:         [3i32, 1i32, 4i32, 1i32, 5i32],
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "walk {} trapped {}",
// CHECK-NEXT:         walk(values.0.as_slice()),
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
