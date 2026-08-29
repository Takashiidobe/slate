#include <stdio.h>

enum Status { STATUS_OK, STATUS_FAIL };

typedef enum Status Processor(int x);

struct Dispatcher {
  Processor *run;
};

static int lastCode = -1;

static enum Status succeed(int x) {
  lastCode = x + 100;
  return STATUS_OK;
}

static enum Status fail(int x) {
  lastCode = x + 200;
  return STATUS_FAIL;
}

int main(void) {
  struct Dispatcher d;
  d.run = succeed;
  d.run(1);
  int a = lastCode;
  d.run = fail;
  d.run(2);
  int b = lastCode;
  printf("%d %d\n", a, b);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum Status {
// LOWERING-NEXT:     STATUS_OK = 0,
// LOWERING-NEXT:     STATUS_FAIL = 1,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Dispatcher {
// LOWERING-NEXT:     run: Option<unsafe extern "C" fn(i32) -> u32>,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut lastCode: i32 = -1;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn succeed(arg1: i32) -> u32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: u32 = 0;
// LOWERING-NEXT:     x = arg1;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 100;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         lastCode = _v2;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v3: u32 = Status::STATUS_OK as u32;
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: u32 = __retval;
// LOWERING-NEXT:     return _v4;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn fail(arg0: i32) -> u32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: u32 = 0;
// LOWERING-NEXT:     x = arg0;
// LOWERING-NEXT:     let _v0: i32 = x;
// LOWERING-NEXT:     let _v1: i32 = 200;
// LOWERING-NEXT:     let _v2: i32 = _v0 + _v1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         lastCode = _v2;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v3: u32 = Status::STATUS_FAIL as u32;
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: u32 = __retval;
// LOWERING-NEXT:     return _v4;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut d: Dispatcher = Dispatcher { run: None };
// LOWERING-NEXT:     let mut a: i32 = 0;
// LOWERING-NEXT:     let mut b: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     d.run = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> u32>>(succeed as *const ()) };
// LOWERING-NEXT:     let _v1: Option<unsafe extern "C" fn(i32) -> u32> = d.run;
// LOWERING-NEXT:     let _v2: i32 = 1;
// LOWERING-NEXT:     let _v3: u32 = unsafe { _v1.unwrap()(_v2) };
// LOWERING-NEXT:     let _v4: i32 = unsafe { lastCode };
// LOWERING-NEXT:     a = _v4;
// LOWERING-NEXT:     d.run = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> u32>>(fail as *const ()) };
// LOWERING-NEXT:     let _v5: Option<unsafe extern "C" fn(i32) -> u32> = d.run;
// LOWERING-NEXT:     let _v6: i32 = 2;
// LOWERING-NEXT:     let _v7: u32 = unsafe { _v5.unwrap()(_v6) };
// LOWERING-NEXT:     let _v8: i32 = unsafe { lastCode };
// LOWERING-NEXT:     b = _v8;
// LOWERING-NEXT:     let _v9: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: i32 = a;
// LOWERING-NEXT:     let _v11: i32 = b;
// LOWERING-NEXT:     let _v12: i32 = unsafe { printf(_v9 as *const i8, _v10, _v11) };
// LOWERING-NEXT:     let _v13: i32 = 0;
// LOWERING-NEXT:     __retval = _v13;
// LOWERING-NEXT:     let _v14: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v14 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum Status {
// REWRITES-NEXT:     STATUS_OK = 0,
// REWRITES-NEXT:     STATUS_FAIL = 1,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Dispatcher {
// REWRITES-NEXT:     run: Option<unsafe extern "C" fn(i32) -> u32>,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut lastCode: i32 = -1;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn succeed(arg1: i32) -> u32 {
// REWRITES-NEXT: let mut x: i32 = arg1;
// REWRITES-NEXT: let mut __retval: u32 = 0;
// REWRITES-NEXT: let _v1: i32 = 100;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         lastCode = x + _v1;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v3: u32 = Status::STATUS_OK as u32;
// REWRITES-NEXT: __retval = _v3;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C" fn fail(arg0: i32) -> u32 {
// REWRITES-NEXT: let mut x: i32 = arg0;
// REWRITES-NEXT: let mut __retval: u32 = 0;
// REWRITES-NEXT: let _v1: i32 = 200;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         lastCode = x + _v1;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v3: u32 = Status::STATUS_FAIL as u32;
// REWRITES-NEXT: __retval = _v3;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut d: Dispatcher = Dispatcher { run: None };
// REWRITES-NEXT: let mut a: i32 = 0;
// REWRITES-NEXT: let mut b: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: d.run = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> u32>>(succeed as *const ()) };
// REWRITES-NEXT: let _v2: i32 = 1;
// REWRITES-NEXT: let _v3: u32 = unsafe { d.run.unwrap()(_v2) };
// REWRITES-NEXT: a = unsafe { lastCode };
// REWRITES-NEXT: d.run = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32) -> u32>>(fail as *const ()) };
// REWRITES-NEXT: let _v6: i32 = 2;
// REWRITES-NEXT: let _v7: u32 = unsafe { d.run.unwrap()(_v6) };
// REWRITES-NEXT: b = unsafe { lastCode };
// REWRITES-NEXT: let _v9: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v12: i32 = unsafe { printf(_v9 as *const i8, a, b) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
