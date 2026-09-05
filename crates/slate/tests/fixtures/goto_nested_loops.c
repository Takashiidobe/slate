#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  int i     = 0;
  int j     = 0;
  int total = 0;
outer:
  j = 0;
inner:
  total = total + (i * j);
  j     = j + 1;
  if (j < 3)
    goto inner;
  i = i + 1;
  if (i < 4)
    goto outer;
  printf("%d\n", total);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     let mut i: i32 = 0;
// COMMON-LOWERING-DAG:     let mut j: i32 = 0;
// COMMON-LOWERING-DAG:     let mut total: i32 = 0;
// COMMON-LOWERING-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     '{{__dispatch[0-9]+}}: loop {
// COMMON-LOWERING-DAG:         match {{__state[0-9]+}} {
// COMMON-LOWERING-DAG:             0 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 j = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             1 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 j = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 2;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             2 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 j = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 3;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             3 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = j;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 4;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 5;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             4 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 2;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             5 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 6;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             6 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 i = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 7;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             7 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 8;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 9;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             8 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             9 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 10;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             10 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
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
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:                 let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn main() {
// COMMON-REWRITES-DAG:     let mut __retval: i32 = 0;
// COMMON-REWRITES-DAG:     let mut i: i32 = 0;
// COMMON-REWRITES-DAG:     let mut j: i32 = 0;
// COMMON-REWRITES-DAG:     let mut total: i32 = 0;
// COMMON-REWRITES-DAG:     __retval = 0;
// COMMON-REWRITES-DAG:     i = 0;
// COMMON-REWRITES-DAG:     j = 0;
// COMMON-REWRITES-DAG:     total = 0;
// COMMON-REWRITES-DAG:     loop {
// COMMON-REWRITES-DAG:         j = 0;
// COMMON-REWRITES-DAG:         loop {
// COMMON-REWRITES-DAG:             total += i * j;
// COMMON-REWRITES-DAG:             j += 1;
// COMMON-REWRITES-DAG:             if !(j < 3) {
// COMMON-REWRITES-DAG:                 break;
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:         i += 1;
// COMMON-REWRITES-DAG:         if !(i < 4) {
// COMMON-REWRITES-DAG:             break;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     unsafe { printf(c"%d\n".as_ptr(), total) };
// COMMON-REWRITES-DAG:     __retval = 0;
// COMMON-REWRITES-DAG:     std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
