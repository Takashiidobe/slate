#include <stdio.h>

void abort(void);
void exit(int);

#ifdef DEBUG
#define abort() printf("error, line %d\n", __LINE__)
#endif

int count;

void a1() { ++count; }

void b(unsigned char data) {
  if (data & 0x80)
    a1();
  data <<= 1;

  if (data & 0x80)
    a1();
  data <<= 1;

  if (data & 0x80)
    a1();
}

int main(void) {
  count = 0;
  b(0);
  if (count != 0)
    abort();

  count = 0;
  b(0x80);
  if (count != 1)
    abort();

  count = 0;
  b(0x40);
  if (count != 1)
    abort();

  count = 0;
  b(0x20);
  if (count != 1)
    abort();

  count = 0;
  b(0xc0);
  if (count != 2)
    abort();

  count = 0;
  b(0xa0);
  if (count != 2)
    abort();

  count = 0;
  b(0x60);
  if (count != 2)
    abort();

  count = 0;
  b(0xe0);
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
// CHECK-NEXT: fn b(mut data: u8) {
// CHECK-NEXT:     if ((data as u32) as i32) & (128 as i32) != (0 as i32) {
// CHECK-NEXT:         a1();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: u8 = data;
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u8 = (((([[V_3]] as u32) as i32) << (1 as i32)) as i8) as u8;
// CHECK-NEXT:     data = [[V_4]];
// CHECK-NEXT:     if ((data as u32) as i32) & (128 as i32) != (0 as i32) {
// CHECK-NEXT:         a1();
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u8 = data;
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u8 = (((([[V_5]] as u32) as i32) << (1 as i32)) as i8) as u8;
// CHECK-NEXT:     data = [[V_6]];
// CHECK-NEXT:     if ((data as u32) as i32) & (128 as i32) != (0 as i32) {
// CHECK-NEXT:         a1();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((0 as i32) as i8) as u8);
// CHECK-NEXT:     if (unsafe { count }) != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((128 as i32) as i8) as u8);
// CHECK-NEXT:     if (unsafe { count }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((64 as i32) as i8) as u8);
// CHECK-NEXT:     if (unsafe { count }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((32 as i32) as i8) as u8);
// CHECK-NEXT:     if (unsafe { count }) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((192 as i32) as i8) as u8);
// CHECK-NEXT:     if (unsafe { count }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((160 as i32) as i8) as u8);
// CHECK-NEXT:     if (unsafe { count }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((96 as i32) as i8) as u8);
// CHECK-NEXT:     if (unsafe { count }) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         count = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     b(((224 as i32) as i8) as u8);
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
