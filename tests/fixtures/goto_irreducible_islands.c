#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
int main() {
  volatile int pick_a = 1;
  volatile int pick_b = 0;
  int          x      = 0;
  int          y      = 0;

  if (pick_a)
    goto a2;
a1:
  x = x + 1;
  if (x < 3)
    goto a2;
  goto second;
a2:
  x = x + 2;
  if (x < 4)
    goto a1;

second:
  if (pick_b)
    goto b2;
b1:
  y = y + 3;
  if (y < 9)
    goto b2;
  goto done;
b2:
  y = y + 5;
  if (y < 11)
    goto b1;

done:
  printf("%d %d\n", x, y);
  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn main() {
// COMMON-LOWERING-DAG:     let mut __retval: i32 = 0;
// COMMON-LOWERING-DAG:     let mut pick_a: i32 = 0;
// COMMON-LOWERING-DAG:     let mut pick_b: i32 = 0;
// COMMON-LOWERING-DAG:     let mut x: i32 = 0;
// COMMON-LOWERING-DAG:     let mut y: i32 = 0;
// COMMON-LOWERING-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     '{{__dispatch[0-9]+}}: loop {
// COMMON-LOWERING-DAG:         match {{__state[0-9]+}} {
// COMMON-LOWERING-DAG:             0 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                 unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(pick_a), {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(pick_b), {{__v[0-9]+}}) };
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 x = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                 y = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             1 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_a)) };
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 2;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 3;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             2 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 10;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             3 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 4;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             4 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 5;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             5 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 x = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 6;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             6 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 7;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 8;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             7 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 10;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             8 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 9;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             9 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 15;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             10 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 x = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 11;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             11 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 12;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 13;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             12 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 5;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             13 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 14;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             14 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 15;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             15 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 16;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             16 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_b)) };
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 17;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 18;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             17 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 25;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             18 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 19;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             19 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 20;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             20 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = y;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 y = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 21;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             21 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = y;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 22;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 23;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             22 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 25;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             23 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 24;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             24 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 30;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             25 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = y;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 y = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 26;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             26 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = y;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 27;
// COMMON-LOWERING-DAG:                 } else {
// COMMON-LOWERING-DAG:                     {{__state[0-9]+}} = 28;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             27 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 20;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             28 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 29;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             29 => {
// COMMON-LOWERING-DAG:                 {{__state[0-9]+}} = 30;
// COMMON-LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:             30 => {
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:                 let {{__v[0-9]+}}: i32 = y;
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
// COMMON-REWRITES-DAG:     let mut pick_a: i32 = 0;
// COMMON-REWRITES-DAG:     let mut pick_b: i32 = 0;
// COMMON-REWRITES-DAG:     let mut x: i32 = 0;
// COMMON-REWRITES-DAG:     let mut y: i32 = 0;
// COMMON-REWRITES-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(pick_a), 1 as i32) };
// COMMON-REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(pick_b), 0 as i32) };
// COMMON-REWRITES-DAG:     x = 0;
// COMMON-REWRITES-DAG:     y = 0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_a)) };
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} != 0 {
// COMMON-REWRITES-DAG:         {{__state[0-9]+}} = 4;
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         {{__state[0-9]+}} = 2;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     loop {
// COMMON-REWRITES-DAG:         match {{__state[0-9]+}} {
// COMMON-REWRITES-DAG:             2 => {
// COMMON-REWRITES-DAG:                 x += 1;
// COMMON-REWRITES-DAG:                 if x < 3 {
// COMMON-REWRITES-DAG:                     {{__state[0-9]+}} = 4;
// COMMON-REWRITES-DAG:                 } else {
// COMMON-REWRITES-DAG:                     break;
// COMMON-REWRITES-DAG:                 }
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             _ => {
// COMMON-REWRITES-DAG:                 x += 2;
// COMMON-REWRITES-DAG:                 if x < 4 {
// COMMON-REWRITES-DAG:                     {{__state[0-9]+}} = 2;
// COMMON-REWRITES-DAG:                 } else {
// COMMON-REWRITES-DAG:                     break;
// COMMON-REWRITES-DAG:                 }
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_b)) };
// COMMON-REWRITES-DAG:     if {{__v[0-9]+}} != 0 {
// COMMON-REWRITES-DAG:         {{__state[0-9]+}} = 9;
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         {{__state[0-9]+}} = 7;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     loop {
// COMMON-REWRITES-DAG:         match {{__state[0-9]+}} {
// COMMON-REWRITES-DAG:             7 => {
// COMMON-REWRITES-DAG:                 y += 3;
// COMMON-REWRITES-DAG:                 if y < 9 {
// COMMON-REWRITES-DAG:                     {{__state[0-9]+}} = 9;
// COMMON-REWRITES-DAG:                 } else {
// COMMON-REWRITES-DAG:                     break;
// COMMON-REWRITES-DAG:                 }
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:             _ => {
// COMMON-REWRITES-DAG:                 y += 5;
// COMMON-REWRITES-DAG:                 if y < 11 {
// COMMON-REWRITES-DAG:                     {{__state[0-9]+}} = 7;
// COMMON-REWRITES-DAG:                 } else {
// COMMON-REWRITES-DAG:                     break;
// COMMON-REWRITES-DAG:                 }
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), x, y) };
// COMMON-REWRITES-DAG:     std::process::exit(__retval as i32);
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
