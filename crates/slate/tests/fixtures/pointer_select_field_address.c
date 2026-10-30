#include <stdio.h>

typedef unsigned long long BigCount;

struct Accounting {
  BigCount direct;
  BigCount indirect;
};

struct Accounting acc = {0, 0};

static void add(int isDirect, BigCount amount) {
  BigCount *const target  = isDirect ? &acc.direct : &acc.indirect;
  *target                += amount;
}

int main(void) {
  add(1, 3);
  add(0, 5);
  add(1, 7);
  printf("%llu %llu\n", acc.direct, acc.indirect);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Accounting {
// CHECK-NEXT:     direct: u64,
// CHECK-NEXT:     indirect: u64,
// CHECK-NEXT: }
// CHECK-NEXT: static mut acc: Accounting = Accounting {
// CHECK-NEXT:     direct: 0u64,
// CHECK-NEXT:     indirect: 0u64,
// CHECK-NEXT: };
// CHECK-NEXT: fn add(isDirect: i32, amount: u64) {
// CHECK-NEXT:     let target: *mut u64 = if isDirect != 0 {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(acc.direct) }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { std::ptr::addr_of_mut!(acc.indirect) }
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: u64 = unsafe { *target };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *target = [[V_0]].wrapping_add(amount);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     add(1i32, 3u64);
// CHECK-NEXT:     add(0i32, 5u64);
// CHECK-NEXT:     add(1i32, 7u64);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu %llu\n".as_ptr(), acc.direct, acc.indirect) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
