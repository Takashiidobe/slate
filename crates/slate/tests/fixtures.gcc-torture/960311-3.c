#include <stdio.h>

void abort(void);
void exit(int);

#ifdef DEBUG
#define abort() printf("error, line %d\n", __LINE__)
#endif

int count;

void a1() { ++count; }

void b(unsigned long data) {
  if (data & 0x80000000)
    a1();
  data <<= 1;

  if (data & 0x80000000)
    a1();
  data <<= 1;

  if (data & 0x80000000)
    a1();
}

int main(void) {
  count = 0;
  b(0);
  if (count != 0)
    abort();

  count = 0;
  b(0x80000000);
  if (count != 1)
    abort();

  count = 0;
  b(0x40000000);
  if (count != 1)
    abort();

  count = 0;
  b(0x20000000);
  if (count != 1)
    abort();

  count = 0;
  b(0xc0000000);
  if (count != 2)
    abort();

  count = 0;
  b(0xa0000000);
  if (count != 2)
    abort();

  count = 0;
  b(0x60000000);
  if (count != 2)
    abort();

  count = 0;
  b(0xe0000000);
  if (count != 3)
    abort();

#ifdef DEBUG
  printf("Done.\n");
#endif
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut count: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn a1() {
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = unsafe { count };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn b(mut data: u64) {
// CHECK-NEXT:     if data & ((2147483648 as u32) as u64) != (0 as u64) {
// CHECK-NEXT:         a1();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u64 = data;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u64 = [[V_3]] << (1 as i32);
// CHECK-NEXT:     data = [[V_4]];
// CHECK-NEXT:     if data & ((2147483648 as u32) as u64) != (0 as u64) {
// CHECK-NEXT:         a1();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u64 = data;
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u64 = [[V_5]] << (1 as i32);
// CHECK-NEXT:     data = [[V_6]];
// CHECK-NEXT:     if data & ((2147483648 as u32) as u64) != (0 as u64) {
// CHECK-NEXT:         a1();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((0 as i32) as i64) as u64);
// CHECK-NEXT:     if (unsafe { count }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b((2147483648 as u32) as u64);
// CHECK-NEXT:     if (unsafe { count }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((1073741824 as i32) as i64) as u64);
// CHECK-NEXT:     if (unsafe { count }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((536870912 as i32) as i64) as u64);
// CHECK-NEXT:     if (unsafe { count }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b((3221225472 as u32) as u64);
// CHECK-NEXT:     if (unsafe { count }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b((2684354560 as u32) as u64);
// CHECK-NEXT:     if (unsafe { count }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((1610612736 as i32) as i64) as u64);
// CHECK-NEXT:     if (unsafe { count }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b((3758096384 as u32) as u64);
// CHECK-NEXT:     if (unsafe { count }) != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
