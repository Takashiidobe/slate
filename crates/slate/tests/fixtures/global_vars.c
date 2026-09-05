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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Pair {
// COMMON-LOWERING-NEXT:     left: i32,
// COMMON-LOWERING-NEXT:     right: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut counter: i32 = 4;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut pair: Pair = Pair { left: 3, right: 5 };
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut zeroed: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = adjust({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { counter };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { zeroed };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn adjust({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { counter };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         counter = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { counter };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { zeroed };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         zeroed = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { zeroed };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pair.right };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         pair.right = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pair.left };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { pair.right };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT: static mut numbers: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([1, 2, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*numbers)[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*numbers)[({{__v[0-9]+}} as usize)] };
// LOWERING-X86_64-GNU-NEXT:         (*numbers)[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { (*numbers)[({{__v[0-9]+}} as usize)] };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT: static mut numbers: [i32; 4] = [1, 2, 0, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { numbers[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { numbers[({{__v[0-9]+}} as usize)] };
// LOWERING-AARCH64-GNU-NEXT:         numbers[({{__v[0-9]+}} as usize)] = {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { numbers[({{__v[0-9]+}} as usize)] };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Pair {
// COMMON-REWRITES-NEXT:     left: i32,
// COMMON-REWRITES-NEXT:     right: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut counter: i32 = 4;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut pair: Pair = Pair { left: 3, right: 5 };
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut zeroed: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), adjust(6)) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             unsafe { counter },
// COMMON-REWRITES-NEXT:             unsafe { zeroed },
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn adjust({{arg[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         counter = (unsafe { counter }) + {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         zeroed = (unsafe { zeroed }) + unsafe { counter };
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     (unsafe { pair.left }) + unsafe { pair.right }
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT: static mut numbers: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([1, 2, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:             unsafe { (*numbers)[2] },
// REWRITES-X86_64-GNU-NEXT:         (*numbers)[({{__v[0-9]+}} as usize)] = (unsafe { zeroed }) - unsafe { (*numbers)[0] };
// REWRITES-X86_64-GNU-NEXT:         pair.right = (unsafe { pair.right }) + unsafe { (*numbers)[1] };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT: static mut numbers: [i32; 4] = [1, 2, 0, 0];
// REWRITES-AARCH64-GNU-NEXT:             unsafe { numbers[2] },
// REWRITES-AARCH64-GNU-NEXT:         numbers[({{__v[0-9]+}} as usize)] = (unsafe { zeroed }) - unsafe { numbers[0] };
// REWRITES-AARCH64-GNU-NEXT:         pair.right = (unsafe { pair.right }) + unsafe { numbers[1] };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
