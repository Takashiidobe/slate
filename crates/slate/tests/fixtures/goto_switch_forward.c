#include <stdio.h>

int main() {
  int x = 2;
  int r = 0;
  switch (x) {
  case 1:
    goto one;
  case 2:
    goto two;
  default:
    goto other;
  }
one:
  r = 10;
  goto done;
two:
  r = 20;
  goto done;
other:
  r = 30;
  goto done;
done:
  printf("%d\n", r);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut r: i32 = 0;
// LOWERING-NEXT: let mut {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:                 x = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 r = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 {{_v[0-9]+}} = x;
// LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 match {{_v[0-9]+}} {
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 4;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 5;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 6;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             3 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 4;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             4 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 9;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             5 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 10;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             6 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 11;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             7 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 8;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             8 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 9;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             9 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                 r = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 12;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             10 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                 r = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 12;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             11 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 30;
// LOWERING-NEXT:                 r = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 12;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             12 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = r;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 break '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: 1 => {
// REWRITES-DAG: r = 20;
// REWRITES-DAG: _ => {
// REWRITES-DAG: r = 30;
