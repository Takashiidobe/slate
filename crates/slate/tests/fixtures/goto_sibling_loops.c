#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  int i    = 0;
  int k    = 0;
  int sum  = 0;
  int prod = 1;
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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     let mut i: i32 = 0;
// COMMON-LOWERING-DAG:     let mut k: i32 = 0;
// COMMON-LOWERING-DAG:     let mut sum: i32 = 0;
// COMMON-LOWERING-DAG:     let mut prod: i32 = 0;
// COMMON-LOWERING-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     '{{__dispatch[0-9]+}}: loop {
// COMMON-LOWERING-DAG:         match {{__state[0-9]+}} {
// COMMON-LOWERING-DAG:             0 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 k = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 sum = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                 prod = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             1 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 sum = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 2;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             2 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 3;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 4;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             3 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             4 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 5;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             5 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                 k = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 6;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             6 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = prod;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 prod = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 k = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 7;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             7 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = k;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 8;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 9;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             8 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 6;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             9 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 10;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             10 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = sum;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = prod;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-DAG:                 std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             _ => {
// COMMON-LOWERING-DAG:                 break '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:                 let {{__v[0-9]+}}: *mut u8 = b"%d %d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     let mut i: i32 = 0;
// COMMON-REWRITES-DAG:     let mut k: i32 = 0;
// COMMON-REWRITES-DAG:     let mut sum: i32 = 0;
// COMMON-REWRITES-DAG:     let mut prod: i32 = 0;
// COMMON-REWRITES-DAG:     __retval = 0;
// COMMON-REWRITES-DAG:     i = 0;
// COMMON-REWRITES-DAG:     k = 0;
// COMMON-REWRITES-DAG:     sum = 0;
// COMMON-REWRITES-DAG:     prod = 1;
// COMMON-REWRITES-DAG:     loop {
// COMMON-REWRITES-DAG:         sum += i;
// COMMON-REWRITES-DAG:         i += 1;
// COMMON-REWRITES-DAG:         if !(i < 5) {
// COMMON-REWRITES-DAG:             break;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     k = 1;
// COMMON-REWRITES-DAG:     loop {
// COMMON-REWRITES-DAG:         prod *= k;
// COMMON-REWRITES-DAG:         k += 1;
// COMMON-REWRITES-DAG:         if !(k < 5) {
// COMMON-REWRITES-DAG:             break;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), sum, prod) };
// COMMON-REWRITES-DAG:     __retval = 0;
// COMMON-REWRITES-DAG:     std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
