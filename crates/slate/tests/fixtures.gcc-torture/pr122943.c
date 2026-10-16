/* PR tree-optimization/122943 */

__attribute__((noipa)) unsigned char foo(long long val) {
  unsigned char result = 0;
  switch (val) {
  case 0:
    result = 1;
    break;
  case 1:
    result = 2;
    break;
  case 2:
    result = 3;
    break;
  default:
    break;
  }
  return result;
}

__attribute__((noipa)) unsigned char bar(long long val) {
  unsigned char result = 1;
  switch (val) {
  case 0:
    result = 8;
    break;
  case 1:
    result = 31;
    break;
  case 2:
    result = 72;
    break;
  default:
    break;
  }
  return result;
}

#ifdef __SIZEOF_INT128__
__attribute__((noipa)) unsigned char baz(__int128 val) {
  unsigned char result = 0;
  switch (val) {
  case 0:
    result = 1;
    break;
  case 1:
    result = 2;
    break;
  case 2:
    result = 3;
    break;
  default:
    break;
  }
  return result;
}

__attribute__((noipa)) unsigned char qux(__int128 val) {
  unsigned char result = 1;
  switch (val) {
  case 0:
    result = 8;
    break;
  case 1:
    result = 31;
    break;
  case 2:
    result = 72;
    break;
  default:
    break;
  }
  return result;
}
#endif

int main() {
  if (foo(-1) != 0)
    __builtin_abort();
  if (foo(0) != 1)
    __builtin_abort();
  if (foo(1) != 2)
    __builtin_abort();
  if (foo(2) != 3)
    __builtin_abort();
  if (foo(3) != 0)
    __builtin_abort();
  if (foo(-__LONG_LONG_MAX__ - 1) != 0)
    __builtin_abort();
  if (foo(-__LONG_LONG_MAX__) != 0)
    __builtin_abort();
  if (foo(-__LONG_LONG_MAX__ + 1) != 0)
    __builtin_abort();
  if (bar(-1) != 1)
    __builtin_abort();
  if (bar(0) != 8)
    __builtin_abort();
  if (bar(1) != 31)
    __builtin_abort();
  if (bar(2) != 72)
    __builtin_abort();
  if (bar(3) != 1)
    __builtin_abort();
  if (bar(-__LONG_LONG_MAX__ - 1) != 1)
    __builtin_abort();
  if (bar(-__LONG_LONG_MAX__) != 1)
    __builtin_abort();
  if (bar(-__LONG_LONG_MAX__ + 1) != 1)
    __builtin_abort();
#ifdef __SIZEOF_INT128__
  if (baz(-1) != 0)
    __builtin_abort();
  if (baz(0) != 1)
    __builtin_abort();
  if (baz(1) != 2)
    __builtin_abort();
  if (baz(2) != 3)
    __builtin_abort();
  if (baz(3) != 0)
    __builtin_abort();
  if (baz(((__int128)1) << 64) != 0)
    __builtin_abort();
  if (baz((((__int128)1) << 64) + 1) != 0)
    __builtin_abort();
  if (baz((((__int128)1) << 64) + 2) != 0)
    __builtin_abort();
  if (qux(-1) != 1)
    __builtin_abort();
  if (qux(0) != 8)
    __builtin_abort();
  if (qux(1) != 31)
    __builtin_abort();
  if (qux(2) != 72)
    __builtin_abort();
  if (qux(3) != 1)
    __builtin_abort();
  if (qux(((__int128)1) << 64) != 1)
    __builtin_abort();
  if (qux((((__int128)1) << 64) + 1) != 1)
    __builtin_abort();
  if (qux((((__int128)1) << 64) + 2) != 1)
    __builtin_abort();
#endif
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/122943
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut val: i64) -> u8 {
// CHECK-NEXT:     let mut result: u8 = ((0 as i32) as i8) as u8;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match val {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 result = ((1 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 result = ((2 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 result = ((3 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut val: i64) -> u8 {
// CHECK-NEXT:     let mut result: u8 = ((1 as i32) as i8) as u8;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match val {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 result = ((8 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 result = ((31 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 result = ((72 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn baz(mut val: i128) -> u8 {
// CHECK-NEXT:     let mut result: u8 = ((0 as i32) as i8) as u8;
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match val {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 result = ((1 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 result = ((2 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 result = ((3 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn qux(mut val: i128) -> u8 {
// CHECK-NEXT:     let mut result: u8 = ((1 as i32) as i8) as u8;
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match val {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 result = ((8 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 result = ((31 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 result = ((72 as i32) as i8) as u8;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((foo(-(1 as i32) as i64) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((foo((0 as i32) as i64) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((foo((1 as i32) as i64) as u32) as i32) != (2 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((foo((2 as i32) as i64) as u32) as i32) != (3 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((foo((3 as i32) as i64) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((foo(-(9223372036854775807 as i64) - ((1 as i32) as i64)) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((foo(-(9223372036854775807 as i64)) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((foo(-(9223372036854775807 as i64) + ((1 as i32) as i64)) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((bar(-(1 as i32) as i64) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((bar((0 as i32) as i64) as u32) as i32) != (8 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((bar((1 as i32) as i64) as u32) as i32) != (31 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((bar((2 as i32) as i64) as u32) as i32) != (72 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((bar((3 as i32) as i64) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((bar(-(9223372036854775807 as i64) - ((1 as i32) as i64)) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((bar(-(9223372036854775807 as i64)) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((bar(-(9223372036854775807 as i64) + ((1 as i32) as i64)) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((baz(-(1 as i32) as i128) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((baz((0 as i32) as i128) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((baz((1 as i32) as i128) as u32) as i32) != (2 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((baz((2 as i32) as i128) as u32) as i32) != (3 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((baz((3 as i32) as i128) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((baz(((1 as i32) as i128) << (64 as i32)) as u32) as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((baz((((1 as i32) as i128) << (64 as i32)) + ((1 as i32) as i128)) as u32) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((baz((((1 as i32) as i128) << (64 as i32)) + ((2 as i32) as i128)) as u32) as i32)
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((qux(-(1 as i32) as i128) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((qux((0 as i32) as i128) as u32) as i32) != (8 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((qux((1 as i32) as i128) as u32) as i32) != (31 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((qux((2 as i32) as i128) as u32) as i32) != (72 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((qux((3 as i32) as i128) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((qux(((1 as i32) as i128) << (64 as i32)) as u32) as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((qux((((1 as i32) as i128) << (64 as i32)) + ((1 as i32) as i128)) as u32) as i32)
// CHECK-NEXT:         != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((qux((((1 as i32) as i128) << (64 as i32)) + ((2 as i32) as i128)) as u32) as i32)
// CHECK-NEXT:         != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
// SLATE-FILECHECK-END
