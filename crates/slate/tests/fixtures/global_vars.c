#include <stdio.h>

int counter = 4;
int zeroed;
int numbers[4] = {1, 2};

struct Pair {
  int left;
  int right;
};

struct Pair pair = {3, 5};

static int adjust(int by) {
  counter    += by;
  zeroed     += counter;
  numbers[2]  = zeroed - numbers[0];
  pair.right += numbers[1];
  return pair.left + pair.right;
}

int main(void) {
  printf("%d\n", adjust(6));
  printf("%d %d %d\n", counter, zeroed, numbers[2]);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Pair {
// LOWERING-NEXT:     left: i32,
// LOWERING-NEXT:     right: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut counter: i32 = 4;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut numbers: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([1, 2, 0, 0]);
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut pair: Pair = Pair { left: 3, right: 5 };
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut zeroed: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn adjust({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { counter };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         counter = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { counter };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { zeroed };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         zeroed = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { zeroed };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*numbers)[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*numbers)[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*numbers)[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { pair.right };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         pair.right = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { pair.left };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { pair.right };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = adjust({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { counter };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { zeroed };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { (*numbers)[({{_v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Pair {
// REWRITES-NEXT:     left: i32,
// REWRITES-NEXT:     right: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut counter: i32 = 4;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut numbers: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([1, 2, 0, 0]);
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut pair: Pair = Pair { left: 3, right: 5 };
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut zeroed: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn adjust({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         counter = (unsafe { counter }) + {{arg[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         zeroed = (unsafe { zeroed }) + unsafe { counter };
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*numbers)[({{_v[0-9]+}} as usize)] = (unsafe { zeroed }) - unsafe { (*numbers)[({{_v[0-9]+}} as usize)] };
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         pair.right = (unsafe { pair.right }) + unsafe { (*numbers)[({{_v[0-9]+}} as usize)] };
// REWRITES-NEXT: }
// REWRITES-NEXT: return (unsafe { pair.left }) + unsafe { pair.right };
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 6;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = adjust({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, unsafe { counter }, unsafe { zeroed }, unsafe { (*numbers)[({{_v[0-9]+}} as usize)] }) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
