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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Accounting {
// LOWERING-NEXT:     direct: u64,
// LOWERING-NEXT:     indirect: u64,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut acc: Accounting = Accounting { direct: 0, indirect: 0 };
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: u64) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u64 = if {{_v[0-9]+}} { unsafe { std::ptr::addr_of_mut!(acc.direct) } } else { unsafe { std::ptr::addr_of_mut!(acc.indirect) } };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 3;
// LOWERING-NEXT:     add({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 5;
// LOWERING-NEXT:     add({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 7;
// LOWERING-NEXT:     add({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%llu %llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { acc.direct };
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = unsafe { acc.indirect };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Accounting {
// REWRITES-NEXT:     direct: u64,
// REWRITES-NEXT:     indirect: u64,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut acc: Accounting = Accounting { direct: 0, indirect: 0 };
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add({{arg[0-9]+}}: i32, {{arg[0-9]+}}: u64) {
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = {{arg[0-9]+}} != 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut u64 = if {{_v[0-9]+}} { unsafe { std::ptr::addr_of_mut!(acc.direct) } } else { unsafe { std::ptr::addr_of_mut!(acc.indirect) } };
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = (unsafe { *{{_v[0-9]+}} }) + {{arg[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 3;
// REWRITES-NEXT: add({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 5;
// REWRITES-NEXT: add({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 7;
// REWRITES-NEXT: add({{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%llu %llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = unsafe { acc.direct };
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = unsafe { acc.indirect };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
