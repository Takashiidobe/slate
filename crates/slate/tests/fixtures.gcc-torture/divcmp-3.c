extern void abort(void);

int test1(char x) { return x / 100 == 3; }

int test1u(unsigned char x) { return x / 100 == 3; }

int test2(char x) { return x / 100 != 3; }

int test2u(unsigned char x) { return x / 100 != 3; }

int test3(char x) { return x / 100 < 3; }

int test3u(unsigned char x) { return x / 100 < 3; }

int test4(char x) { return x / 100 <= 3; }

int test4u(unsigned char x) { return x / 100 <= 3; }

int test5(char x) { return x / 100 > 3; }

int test5u(unsigned char x) { return x / 100 > 3; }

int test6(char x) { return x / 100 >= 3; }

int test6u(unsigned char x) { return x / 100 >= 3; }

int main() {
  int c;

  for (c = -128; c < 256; c++) {
    if (test1(c) != 0)
      abort();
    if (test1u(c) != 0)
      abort();
    if (test2(c) != 1)
      abort();
    if (test2u(c) != 1)
      abort();
    if (test3(c) != 1)
      abort();
    if (test3u(c) != 1)
      abort();
    if (test4(c) != 1)
      abort();
    if (test4u(c) != 1)
      abort();
    if (test5(c) != 0)
      abort();
    if (test5u(c) != 0)
      abort();
    if (test6(c) != 0)
      abort();
    if (test6u(c) != 0)
      abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn test1(mut x: i8) -> i32 {
// CHECK-NEXT:     ((x as i32) / (100 as i32) == (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test1u(mut x: u8) -> i32 {
// CHECK-NEXT:     (((x as u32) as i32) / (100 as i32) == (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test2(mut x: i8) -> i32 {
// CHECK-NEXT:     ((x as i32) / (100 as i32) != (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test2u(mut x: u8) -> i32 {
// CHECK-NEXT:     (((x as u32) as i32) / (100 as i32) != (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test3(mut x: i8) -> i32 {
// CHECK-NEXT:     ((x as i32) / (100 as i32) < (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test3u(mut x: u8) -> i32 {
// CHECK-NEXT:     (((x as u32) as i32) / (100 as i32) < (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test4(mut x: i8) -> i32 {
// CHECK-NEXT:     ((x as i32) / (100 as i32) <= (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test4u(mut x: u8) -> i32 {
// CHECK-NEXT:     (((x as u32) as i32) / (100 as i32) <= (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test5(mut x: i8) -> i32 {
// CHECK-NEXT:     ((x as i32) / (100 as i32) > (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test5u(mut x: u8) -> i32 {
// CHECK-NEXT:     (((x as u32) as i32) / (100 as i32) > (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test6(mut x: i8) -> i32 {
// CHECK-NEXT:     ((x as i32) / (100 as i32) >= (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn test6u(mut x: u8) -> i32 {
// CHECK-NEXT:     (((x as u32) as i32) / (100 as i32) >= (3 as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut c: i32 = -(128 as i32);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while c < (256 as i32) {
// CHECK-NEXT:         if test1(c as i8) != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test1u((c as i8) as u8) != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test2(c as i8) != (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test2u((c as i8) as u8) != (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test3(c as i8) != (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test3u((c as i8) as u8) != (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test4(c as i8) != (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test4u((c as i8) as u8) != (1 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test5(c as i8) != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test5u((c as i8) as u8) != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test6(c as i8) != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         if test6u((c as i8) as u8) != (0 as i32) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = c;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         c = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
