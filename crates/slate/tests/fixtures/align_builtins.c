#include <stdint.h>
#include <stdio.h>

static int calls;
static int next(void) { return ++calls * 8; }

int main(void) {
  char buffer[64];
  char *base = buffer + 3;
  const int *cp = (const int *)(buffer + 5);
  unsigned char small = 13;
  int i = 0;
  char *up = __builtin_align_up(base, 16);
  char *down = __builtin_align_down(base, 16);
  const int *cup = __builtin_align_up(cp, 8);
  printf("%d %d %d\n", (int)(up - buffer) - (int)(((uintptr_t)buffer + 3 + 15) / 16 * 16 - (uintptr_t)buffer),
         (int)(base - down) == (int)((uintptr_t)base % 16),
         ((uintptr_t)cup % 8) == 0);
  printf("%d %d\n", __builtin_is_aligned(up, 16), __builtin_is_aligned(buffer + 1, 1));
  printf("%d %d %d\n", (int)__builtin_align_up(small, 4), (int)__builtin_align_down(small, 4),
         (int)sizeof(__builtin_align_up(small, 4)));
  printf("%d %d\n", __builtin_align_up(100, next()), calls);
  char *p = buffer + 1;
  char *q = __builtin_align_up(p++, 4);
  printf("%d %d\n", (int)(p - buffer), __builtin_is_aligned(q, 4));
  printf("%d %d\n", (int)__builtin_align_down(-7, 4), (int)__builtin_align_up(-7, 4));
  printf("%d\n", __builtin_is_aligned(i++, 2) + i);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![allow(arithmetic_overflow, unconditional_panic)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn next() -> i32 {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = (unsafe { calls }) + 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = [[V_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_0]] * 8
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buffer: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     let base: *mut i8 = unsafe { (buffer.0.as_mut_ptr() as *mut i8).add(3) };
// CHECK-NEXT:     let cp: *const i32 = (unsafe { (buffer.0.as_mut_ptr() as *mut i8).add(5) })
// CHECK-NEXT:         as *const i32;
// CHECK-NEXT:     let small: u8 = 13;
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: u64 = 15u64;
// CHECK-NEXT:     let [[V_0]]: *mut i8 = base;
// CHECK-NEXT:     let up: *mut i8 = (unsafe {
// CHECK-NEXT:         ([[V_0]] as *mut u8)
// CHECK-NEXT:             .offset(
// CHECK-NEXT:                 (([[V_0]] as u64).wrapping_add([[V_1]]) & ![[V_1]]).wrapping_sub([[V_0]] as u64)
// CHECK-NEXT:                     as isize,
// CHECK-NEXT:             )
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: u64 = 15u64;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: *mut i8 = base;
// CHECK-NEXT:     let down: *mut i8 = (unsafe {
// CHECK-NEXT:         ([[V_3]] as *mut u8).sub((([[V_3]] as u64) & [[V_2]]) as usize)
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     let [[V_4:__v[0-9]+]]: u64 = 7u64;
// CHECK-NEXT:     let [[V_5:__v[0-9]+]]: *const i32 = cp;
// CHECK-NEXT:     let cup: *const i32 = (unsafe {
// CHECK-NEXT:         ([[V_5]] as *const u8)
// CHECK-NEXT:             .offset(
// CHECK-NEXT:                 (([[V_5]] as u64).wrapping_add([[V_4]]) & ![[V_4]]).wrapping_sub([[V_5]] as u64)
// CHECK-NEXT:                     as isize,
// CHECK-NEXT:             )
// CHECK-NEXT:     }) as *const i32;
// CHECK-NEXT:     let __slate_printf_arg0 = (((unsafe {
// CHECK-NEXT:         up.offset_from(buffer.0.as_mut_ptr() as *mut i8)
// CHECK-NEXT:     }) as i64) as i32)
// CHECK-NEXT:         - (((((buffer.0.as_mut_ptr() as *mut i8) as u64).wrapping_add(3).wrapping_add(15)
// CHECK-NEXT:             / 16)
// CHECK-NEXT:             .wrapping_mul(16)
// CHECK-NEXT:             .wrapping_sub((buffer.0.as_mut_ptr() as *mut i8) as u64) as u32) as i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", __slate_printf_arg0, ((((unsafe { base.offset_from(down as * mut i8)
// CHECK-NEXT:         }) as i64) as i32) == ((((base as u64) % 16) as u32) as i32)) as i32, ((cup as
// CHECK-NEXT:         u64) % 8 == 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let [[V_6:__v[0-9]+]]: u64 = 15u64;
// CHECK-NEXT:     let [[V_7:__v[0-9]+]]: *mut i8 = up;
// CHECK-NEXT:     let [[V_8:__v[0-9]+]]: u64 = 0u64;
// CHECK-NEXT:     let [[V_9:__v[0-9]+]]: *mut i8 = unsafe { (buffer.0.as_mut_ptr() as *mut i8).add(1) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", (([[V_7]] as u64) & [[V_6]] == 0) as i32, (([[V_9]] as u64) & [[V_8]] == 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let [[V_10:__v[0-9]+]]: u8 = 3u8;
// CHECK-NEXT:     let [[V_11:__v[0-9]+]]: u8 = 3u8;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", ((small.wrapping_add([[V_10]]) & ! [[V_10]]) as u32) as i32, ((small & !
// CHECK-NEXT:         [[V_11]]) as u32) as i32, 1i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let [[V_12:__v[0-9]+]]: i32 = next().wrapping_sub(1);
// CHECK-NEXT:     println!("{} {}", 100i32.wrapping_add([[V_12]]) & ! [[V_12]], unsafe { calls });
// CHECK-NEXT:     let mut p: *mut i8 = unsafe { (buffer.0.as_mut_ptr() as *mut i8).add(1) };
// CHECK-NEXT:     let [[V_13:__v[0-9]+]]: u64 = 3u64;
// CHECK-NEXT:     let [[V_14:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:     p = unsafe { [[V_14]].add(1) };
// CHECK-NEXT:     let [[V_15:__v[0-9]+]]: *mut i8 = [[V_14]];
// CHECK-NEXT:     let q: *mut i8 = (unsafe {
// CHECK-NEXT:         ([[V_15]] as *mut u8)
// CHECK-NEXT:             .offset(
// CHECK-NEXT:                 (([[V_15]] as u64).wrapping_add([[V_13]]) & ![[V_13]]).wrapping_sub([[V_15]] as u64)
// CHECK-NEXT:                     as isize,
// CHECK-NEXT:             )
// CHECK-NEXT:     }) as *mut i8;
// CHECK-NEXT:     let [[V_16:__v[0-9]+]]: u64 = 3u64;
// CHECK-NEXT:     let [[V_17:__v[0-9]+]]: *mut i8 = q;
// CHECK-NEXT:     let __slate_printf_arg0 = ((unsafe {
// CHECK-NEXT:         p.offset_from(buffer.0.as_mut_ptr() as *mut i8)
// CHECK-NEXT:     }) as i64) as i32;
// CHECK-NEXT:     println!("{} {}", __slate_printf_arg0, (([[V_17]] as u64) & [[V_16]] == 0) as i32);
// CHECK-NEXT:     let [[V_18:__v[0-9]+]]: i32 = 3i32;
// CHECK-NEXT:     let [[V_19:__v[0-9]+]]: i32 = 3i32;
// CHECK-NEXT:     println!("{} {}", - 7 & ! [[V_18]], (- 7i32).wrapping_add([[V_19]]) & ! [[V_19]]);
// CHECK-NEXT:     let [[V_20:__v[0-9]+]]: i32 = 1i32;
// CHECK-NEXT:     let [[V_21:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:     i = [[V_21]] + 1;
// CHECK-NEXT:     println!("{}", (([[V_21]] & [[V_20]] == 0) as i32) + i);
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
