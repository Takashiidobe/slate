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
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Pair {
// LOWERING-NEXT:     left: i32,
// LOWERING-NEXT:     right: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut counter: i32 = 4;
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: static mut numbers: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([1, 2, 0, 0]);
// LOWERING-AARCH64-GNU-NEXT: static mut numbers: [i32; 4] = [1, 2, 0, 0];
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut pair: Pair = Pair { left: 3, right: 5 };
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut zeroed: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = adjust({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { counter };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { zeroed };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*numbers)[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { numbers[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn adjust({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { counter };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         counter = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { counter };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { zeroed };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         zeroed = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { zeroed };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*numbers)[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { numbers[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         (*numbers)[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:         numbers[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*numbers)[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { numbers[({{__v[0-9]+}} as usize)] };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pair.right };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         pair.right = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pair.left };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pair.right };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Pair {
// REWRITES-NEXT:     left: i32,
// REWRITES-NEXT:     right: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut counter: i32 = 4;
// REWRITES-EMPTY:
// REWRITES-X86_64-GNU-NEXT: static mut numbers: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([1, 2, 0, 0]);
// REWRITES-AARCH64-GNU-NEXT: static mut numbers: [i32; 4] = [1, 2, 0, 0];
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut pair: Pair = Pair { left: 3, right: 5 };
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut zeroed: i32 = 0;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), adjust(6)) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// REWRITES-NEXT:             unsafe { counter },
// REWRITES-NEXT:             unsafe { zeroed },
// REWRITES-X86_64-GNU-NEXT:             unsafe { (*numbers)[2] },
// REWRITES-AARCH64-GNU-NEXT:             unsafe { numbers[2] },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn adjust({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         counter = (unsafe { counter }) + {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         zeroed = (unsafe { zeroed }) + unsafe { counter };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// REWRITES-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         (*numbers)[({{__v[0-9]+}} as usize)] = (unsafe { zeroed }) - unsafe { (*numbers)[0] };
// REWRITES-AARCH64-GNU-NEXT:         numbers[({{__v[0-9]+}} as usize)] = (unsafe { zeroed }) - unsafe { numbers[0] };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         pair.right = (unsafe { pair.right }) + unsafe { (*numbers)[1] };
// REWRITES-AARCH64-GNU-NEXT:         pair.right = (unsafe { pair.right }) + unsafe { numbers[1] };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     (unsafe { pair.left }) + unsafe { pair.right }
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
