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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut __retval: i32 = 0;
// LOWERING-DAG:     let mut pick_a: i32 = 0;
// LOWERING-DAG:     let mut pick_b: i32 = 0;
// LOWERING-DAG:     let mut x: i32 = 0;
// LOWERING-DAG:     let mut y: i32 = 0;
// LOWERING-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-DAG:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-DAG:         match {{__state[0-9]+}} {
// LOWERING-DAG:             0 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 __retval = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(pick_a), {{_v[0-9]+}}) };
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(pick_b), {{_v[0-9]+}}) };
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 x = {{_v[0-9]+}};
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:                 y = {{_v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 1;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             1 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_a)) };
// LOWERING-DAG:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-DAG:                 if {{_v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 2;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 3;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             2 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 10;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             3 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 4;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             4 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 5;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             5 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 x = {{_v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 6;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             6 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                 if {{_v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 7;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 8;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             7 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 10;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             8 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 9;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             9 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 15;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             10 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 2;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 x = {{_v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 11;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             11 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 4;
// LOWERING-DAG:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                 if {{_v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 12;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 13;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             12 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 5;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             13 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 14;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             14 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 15;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             15 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 16;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             16 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_b)) };
// LOWERING-DAG:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-DAG:                 if {{_v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 17;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 18;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             17 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 25;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             18 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 19;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             19 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 20;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             20 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = y;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 3;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 y = {{_v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 21;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             21 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = y;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 9;
// LOWERING-DAG:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                 if {{_v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 22;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 23;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             22 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 25;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             23 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 24;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             24 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 30;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             25 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = y;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 5;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:                 y = {{_v[0-9]+}};
// LOWERING-DAG:                 {{__state[0-9]+}} = 26;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             26 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = y;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = 11;
// LOWERING-DAG:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-DAG:                 if {{_v[0-9]+}} {
// LOWERING-DAG:                     {{__state[0-9]+}} = 27;
// LOWERING-DAG:                 } else {
// LOWERING-DAG:                     {{__state[0-9]+}} = 28;
// LOWERING-DAG:                 }
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             27 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 20;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             28 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 29;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             29 => {
// LOWERING-DAG:                 {{__state[0-9]+}} = 30;
// LOWERING-DAG:                 continue '{{__dispatch[0-9]+}};
// LOWERING-DAG:             }
// LOWERING-DAG:             30 => {
// LOWERING-DAG:                 let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-DAG:                 let {{_v[0-9]+}}: i32 = y;
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
// REWRITES-DAG:     let mut pick_a: i32 = 0;
// REWRITES-DAG:     let mut pick_b: i32 = 0;
// REWRITES-DAG:     let mut x: i32 = 0;
// REWRITES-DAG:     let mut y: i32 = 0;
// REWRITES-DAG:     let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(pick_a), 1 as i32) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(pick_b), 0 as i32) };
// REWRITES-DAG:     x = 0;
// REWRITES-DAG:     y = 0;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_a)) };
// REWRITES-DAG:     if {{_v[0-9]+}} != 0 {
// REWRITES-DAG:         {{__state[0-9]+}} = 4;
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__state[0-9]+}} = 2;
// REWRITES-DAG:     }
// REWRITES-DAG:     loop {
// REWRITES-DAG:         match {{__state[0-9]+}} {
// REWRITES-DAG:             2 => {
// REWRITES-DAG:                 x += 1;
// REWRITES-DAG:                 if x < 3 {
// REWRITES-DAG:                     {{__state[0-9]+}} = 4;
// REWRITES-DAG:                 } else {
// REWRITES-DAG:                     break;
// REWRITES-DAG:                 }
// REWRITES-DAG:             }
// REWRITES-DAG:             _ => {
// REWRITES-DAG:                 x += 2;
// REWRITES-DAG:                 if x < 4 {
// REWRITES-DAG:                     {{__state[0-9]+}} = 2;
// REWRITES-DAG:                 } else {
// REWRITES-DAG:                     break;
// REWRITES-DAG:                 }
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(pick_b)) };
// REWRITES-DAG:     if {{_v[0-9]+}} != 0 {
// REWRITES-DAG:         {{__state[0-9]+}} = 9;
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__state[0-9]+}} = 7;
// REWRITES-DAG:     }
// REWRITES-DAG:     loop {
// REWRITES-DAG:         match {{__state[0-9]+}} {
// REWRITES-DAG:             7 => {
// REWRITES-DAG:                 y += 3;
// REWRITES-DAG:                 if y < 9 {
// REWRITES-DAG:                     {{__state[0-9]+}} = 9;
// REWRITES-DAG:                 } else {
// REWRITES-DAG:                     break;
// REWRITES-DAG:                 }
// REWRITES-DAG:             }
// REWRITES-DAG:             _ => {
// REWRITES-DAG:                 y += 5;
// REWRITES-DAG:                 if y < 11 {
// REWRITES-DAG:                     {{__state[0-9]+}} = 7;
// REWRITES-DAG:                 } else {
// REWRITES-DAG:                     break;
// REWRITES-DAG:                 }
// REWRITES-DAG:             }
// REWRITES-DAG:         }
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { printf(c"%d %d\n".as_ptr(), x, y) };
// REWRITES-DAG:     std::process::exit(__retval as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
