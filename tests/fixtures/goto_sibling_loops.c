#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  int i     = 0;
  int k     = 0;
  int sum   = 0;
  int prod  = 1;
first:
  sum = sum + i;
  i   = i + 1;
  if (i < 5)
    goto first;
  k = 1;
second:
  prod = prod * k;
  k    = k + 1;
  if (k < 5)
    goto second;
  printf("%d %d\n", sum, prod);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut i: i32 = 0;
// LOWERING-DAG:     let mut k: i32 = 0;
// LOWERING-DAG:     let mut sum: i32 = 0;
// LOWERING-DAG:     let mut prod: i32 = 0;
// LOWERING-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-DAG:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-DAG:         match {{__state[0-9]+}} {
// LOWERING-DAG:             0 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 __retval = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 i = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 k = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 sum = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 prod = {{_v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             1 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = sum;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 sum = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 i = {{_v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 2;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             2 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                 if {{_v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 3;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 4;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             3 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             4 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 5;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             5 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 k = {{_v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 6;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             6 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = prod;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = k;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-DAG:                 prod = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = k;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 k = {{_v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 7;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             7 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = k;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                 if {{_v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 8;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 9;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             8 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 6;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             9 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 10;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             10 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = sum;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = prod;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 __retval = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                 std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG:             }
// LOWERING-DAG:             _ => {
// LOWERING-DAG:                 break '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut __retval: i32 = 0;
// REWRITES-DAG:     let mut i: i32 = 0;
// REWRITES-DAG:     let mut k: i32 = 0;
// REWRITES-DAG:     let mut sum: i32 = 0;
// REWRITES-DAG:     let mut prod: i32 = 0;
// REWRITES-DAG:     __retval = 0;
// REWRITES-DAG:     i = 0;
// REWRITES-DAG:     k = 0;
// REWRITES-DAG:     sum = 0;
// REWRITES-DAG:     prod = 1;
// REWRITES-DAG:     loop {
// REWRITES-DAG:         sum += i;
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:         if !(i < 5) {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     k = 1;
// REWRITES-DAG:     loop {
// REWRITES-DAG:         prod *= k;
// REWRITES-DAG:         k += 1;
// REWRITES-DAG:         if !(k < 5) {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), sum, prod) };
// REWRITES-DAG:     __retval = 0;
// REWRITES-DAG:     std::process::exit(__retval as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
