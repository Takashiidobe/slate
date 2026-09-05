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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Accounting {
// COMMON-LOWERING-NEXT:     direct: u64,
// COMMON-LOWERING-NEXT:     indirect: u64,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut acc: Accounting = Accounting {
// COMMON-LOWERING-NEXT:     direct: 0,
// COMMON-LOWERING-NEXT:     indirect: 0,
// COMMON-LOWERING-NEXT: };
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 3;
// COMMON-LOWERING-NEXT:     add({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 5;
// COMMON-LOWERING-NEXT:     add({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 7;
// COMMON-LOWERING-NEXT:     add({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { acc.direct };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { acc.indirect };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: u64) {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: *mut u64 = if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:         unsafe { std::ptr::addr_of_mut!(acc.direct) }
// COMMON-LOWERING-NEXT:     } else {
// COMMON-LOWERING-NEXT:         unsafe { std::ptr::addr_of_mut!(acc.indirect) }
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%llu %llu\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%llu %llu\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Accounting {
// COMMON-REWRITES-NEXT:     direct: u64,
// COMMON-REWRITES-NEXT:     indirect: u64,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut acc: Accounting = Accounting {
// COMMON-REWRITES-NEXT:     direct: 0,
// COMMON-REWRITES-NEXT:     indirect: 0,
// COMMON-REWRITES-NEXT: };
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     add(1, 3);
// COMMON-REWRITES-NEXT:     add(0, 5);
// COMMON-REWRITES-NEXT:     add(1, 7);
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { acc.direct };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: u64 = unsafe { acc.indirect };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%llu %llu\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: u64) {
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: *mut u64 = if {{arg[0-9]+}} != 0 {
// COMMON-REWRITES-NEXT:         unsafe { std::ptr::addr_of_mut!(acc.direct) }
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         unsafe { std::ptr::addr_of_mut!(acc.indirect) }
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         *{{__v[0-9]+}} = (unsafe { *{{__v[0-9]+}} }) + {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
