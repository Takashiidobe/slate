#include <stdio.h>

struct Pair {
  int left;
  int right;
};

union Slot {
  int          value;
  unsigned int raw;
};

static int sizeof_values(void) {
  int values[3];
  return sizeof(int) + sizeof(values) + sizeof(struct Pair) +
         sizeof(union Slot);
}

static int sizeof_expr(void) {
  int value = 0;
  return sizeof value;
}

int main(void) {
  printf("%d\n", sizeof_values());
  printf("%d\n", sizeof_expr());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Pair {
// LOWERING-NEXT:     left: i32,
// LOWERING-NEXT:     right: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union Slot {
// LOWERING-NEXT:     value: i32,
// LOWERING-NEXT:     raw: u32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sizeof_values() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut values: [i32; 3] = [0; 3];
// LOWERING-NEXT:     let _v0: u64 = 4;
// LOWERING-NEXT:     let _v1: u64 = 12;
// LOWERING-NEXT:     let _v2: u64 = _v0 + _v1;
// LOWERING-NEXT:     let _v3: u64 = std::mem::size_of::<Pair>() as u64;
// LOWERING-NEXT:     let _v4: u64 = _v2 + _v3;
// LOWERING-NEXT:     let _v5: u64 = std::mem::size_of::<Slot>() as u64;
// LOWERING-NEXT:     let _v6: u64 = _v4 + _v5;
// LOWERING-NEXT:     let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:     __retval = _v7;
// LOWERING-NEXT:     let _v8: i32 = __retval;
// LOWERING-NEXT:     return _v8;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sizeof_expr() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     value = _v0;
// LOWERING-NEXT:     let _v1: i32 = 4;
// LOWERING-NEXT:     __retval = _v1;
// LOWERING-NEXT:     let _v2: i32 = __retval;
// LOWERING-NEXT:     return _v2;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = sizeof_values();
// LOWERING-NEXT:     let _v3: i32 = unsafe { printf(_v1 as *const i8, _v2) };
// LOWERING-NEXT:     let _v4: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i32 = sizeof_expr();
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v4 as *const i8, _v5) };
// LOWERING-NEXT:     let _v7: i32 = 0;
// LOWERING-NEXT:     __retval = _v7;
// LOWERING-NEXT:     let _v8: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v8 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Pair {
// REWRITES-NEXT:     left: i32,
// REWRITES-NEXT:     right: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union Slot {
// REWRITES-NEXT:     value: i32,
// REWRITES-NEXT:     raw: u32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sizeof_values() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut values: [i32; 3] = [0; 3];
// REWRITES-NEXT: let _v0: u64 = 4;
// REWRITES-NEXT: let _v1: u64 = 12;
// REWRITES-NEXT: let _v2: u64 = _v0 + _v1;
// REWRITES-NEXT: let _v3: u64 = std::mem::size_of::<Pair>() as u64;
// REWRITES-NEXT: let _v4: u64 = _v2 + _v3;
// REWRITES-NEXT: let _v5: u64 = std::mem::size_of::<Slot>() as u64;
// REWRITES-NEXT: __retval = (_v4 + _v5) as i32;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sizeof_expr() -> i32 {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut value: i32 = 0;
// REWRITES-NEXT: value = 0;
// REWRITES-NEXT: __retval = 4;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = sizeof_values();
// REWRITES-NEXT: let _v3: i32 = unsafe { printf(_v1 as *const i8, _v2) };
// REWRITES-NEXT: let _v4: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = sizeof_expr();
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v4 as *const i8, _v5) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
