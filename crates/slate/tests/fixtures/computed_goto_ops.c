#include <stdio.h>

static int jump_probe(int x) {
  static void *targets[] = {&&zero, &&one, &&two};
  if (x < 0 || x > 2) {
    return -1;
  }
  goto *targets[x];

zero:
  return 10;
one:
  return 20;
two:
  return 30;
}

int main(void) {
  volatile int input = 2;
  printf("%d\n", jump_probe(input));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut jump_probe_targets: aligned::Aligned<aligned::A16, [*mut core::ffi::c_void; 3]> = aligned::Aligned([std::ptr::null_mut(); 3]);
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn jump_probe({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT: let mut {{_v[0-9]+}}: bool = false;
// LOWERING-NEXT: let mut {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 x = {{arg[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 3;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:                 {{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 4;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             3 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-NEXT:                 {{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 4;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             4 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             5 => {
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 6;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 7;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             6 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             7 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 8;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             8 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut core::ffi::c_void = 1usize as *mut core::ffi::c_void;
// LOWERING-NEXT:                 {{__state[0-9]+}} = [9, 10, 11][({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             9 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             10 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             11 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 30;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             12 => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut input: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = jump_probe({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut jump_probe_targets: aligned::Aligned<aligned::A16, [*mut core::ffi::c_void; 3]> = aligned::Aligned([std::ptr::null_mut(); 3]);
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn jump_probe({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut x: i32 = 0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut {{_v[0-9]+}}: bool = false;
// REWRITES-NEXT: let mut {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-NEXT: '{{__dispatch[0-9]+}}: loop {
// REWRITES-NEXT:         match {{__state[0-9]+}} {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         x = {{arg[0-9]+}};
// REWRITES-NEXT:                         {{__state[0-9]+}} = 1;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:                         if x < {{_v[0-9]+}} {
// REWRITES-NEXT:                                         {{__state[0-9]+}} = 2;
// REWRITES-NEXT:                         } else {
// REWRITES-NEXT:                                         {{__state[0-9]+}} = 3;
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             2 => {
// REWRITES-NEXT:                         {{_v[0-9]+}} = true;
// REWRITES-NEXT:                         {{__state[0-9]+}} = 4;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             3 => {
// REWRITES-NEXT:                         let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:                         {{_v[0-9]+}} = x > {{_v[0-9]+}};
// REWRITES-NEXT:                         {{__state[0-9]+}} = 4;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             4 => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 5;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             5 => {
// REWRITES-NEXT:                         if {{_v[0-9]+}} {
// REWRITES-NEXT:                                         {{__state[0-9]+}} = 6;
// REWRITES-NEXT:                         } else {
// REWRITES-NEXT:                                         {{__state[0-9]+}} = 7;
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             6 => {
// REWRITES-NEXT:                         __retval = -1;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             7 => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 8;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             8 => {
// REWRITES-NEXT:                         let {{_v[0-9]+}}: *mut core::ffi::c_void = 1usize as *mut core::ffi::c_void;
// REWRITES-NEXT:                         {{__state[0-9]+}} = [9, 10, 11][((x as i64) as usize)];
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             9 => {
// REWRITES-NEXT:                         __retval = 10;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             10 => {
// REWRITES-NEXT:                         __retval = 20;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             11 => {
// REWRITES-NEXT:                         __retval = 30;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             12 => {
// REWRITES-NEXT:                         unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut input: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(input), {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(input)) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = jump_probe({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
