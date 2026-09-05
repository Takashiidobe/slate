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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut i: i32 = 0;
// LOWERING-DAG:     let mut j: i32 = 0;
// LOWERING-DAG:     let mut total: i32 = 0;
// LOWERING-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-DAG:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-DAG:         match {{__state[0-9]+}} {
// LOWERING-DAG:             0 => {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 __retval = {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 i = {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 j = {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 total = {{__v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             1 => {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 j = {{__v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 2;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             2 => {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = total;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = j;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                 total = {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = j;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                 j = {{__v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 3;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             3 => {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = j;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:                 if {{__v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 4;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 5;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             4 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 2;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             5 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 6;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             6 => {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:                 i = {{__v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 7;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             7 => {
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:                 if {{__v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 8;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 9;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             8 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             9 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 10;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             10 => {
// LOWERING-X86_64-GNU-DAG:                 let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-DAG:                 let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = total;
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 __retval = {{__v[0-9]+}};
// LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-DAG:                 std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-DAG:     let mut j: i32 = 0;
// REWRITES-DAG:     let mut total: i32 = 0;
// REWRITES-DAG:     __retval = 0;
// REWRITES-DAG:     i = 0;
// REWRITES-DAG:     j = 0;
// REWRITES-DAG:     total = 0;
// REWRITES-DAG:     loop {
// REWRITES-DAG:         j = 0;
// REWRITES-DAG:         loop {
// REWRITES-DAG:             total += i * j;
// REWRITES-DAG:             j += 1;
// REWRITES-DAG:             if !(j < 3) {
// REWRITES-DAG:                 break;
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:         i += 1;
// REWRITES-DAG:         if !(i < 4) {
// REWRITES-DAG:             break;
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { printf(c"%d\n".as_ptr(), total) };
// REWRITES-DAG:     __retval = 0;
// REWRITES-DAG:     std::process::exit(__retval as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
