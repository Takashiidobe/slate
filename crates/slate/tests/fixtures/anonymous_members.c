#include <stdio.h>

struct container {
  int prefix;
  union {
    int   integer;
    float real;
  };
  struct {
    int x;
    int y;
  };
};

int main(void) {
  struct container value = {0};
  value.prefix           = 3;
  value.integer          = 31;
  value.x                = 37;
  value.y                = 41;
  printf("%d %d %d %d %zu\n", value.prefix, value.integer, value.x, value.y,
         sizeof(value));
  value.real = 2.5f;
  printf("%d\n", (int)value.real);
  return 0;
}

// REWRITES-DAG: union anon_0 {
// REWRITES-DAG: integer: i32,
// REWRITES-DAG: real: f32,
// REWRITES-DAG: struct container {
// REWRITES-DAG: __slate_anon_1: anon_0,
// REWRITES-DAG: value.__slate_anon_1.integer

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
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     integer: i32,
// LOWERING-NEXT:     real: f32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct container {
// LOWERING-NEXT:     prefix: i32,
// LOWERING-NEXT:     __slate_anon_1: {{anon_[0-9]+}},
// LOWERING-NEXT:     __slate_anon_2: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut value: container = container {
// LOWERING-NEXT:         prefix: 0,
// LOWERING-NEXT:         __slate_anon_1: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:         __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: container = container {
// LOWERING-NEXT:         prefix: 0,
// LOWERING-NEXT:         __slate_anon_1: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// LOWERING-NEXT:         __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 },
// LOWERING-NEXT:     };
// LOWERING-NEXT:     value = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     value.prefix = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 31;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         value.__slate_anon_1.integer = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 37;
// LOWERING-NEXT:     value.__slate_anon_2.x = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     value.__slate_anon_2.y = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %zu\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %zu\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = value.prefix;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { value.__slate_anon_1.integer };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = value.__slate_anon_2.x;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = value.__slate_anon_2.y;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 2.5;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         value.__slate_anon_1.real = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { value.__slate_anon_1.real };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-NEXT: union {{anon_[0-9]+}} {
// REWRITES-NEXT:     integer: i32,
// REWRITES-NEXT:     real: f32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_[0-9]+}} {
// REWRITES-NEXT:     x: i32,
// REWRITES-NEXT:     y: i32,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct container {
// REWRITES-NEXT:     prefix: i32,
// REWRITES-NEXT:     __slate_anon_1: {{anon_[0-9]+}},
// REWRITES-NEXT:     __slate_anon_2: {{anon_[0-9]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut value: container = container {
// REWRITES-NEXT:         prefix: 0,
// REWRITES-NEXT:         __slate_anon_1: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:         __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     value = container {
// REWRITES-NEXT:         prefix: 0,
// REWRITES-NEXT:         __slate_anon_1: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// REWRITES-NEXT:         __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 },
// REWRITES-NEXT:     };
// REWRITES-NEXT:     value.prefix = 3;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         value.__slate_anon_1.integer = 31;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     value.__slate_anon_2.x = 37;
// REWRITES-NEXT:     value.__slate_anon_2.y = 41;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = value.prefix;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { value.__slate_anon_1.integer };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = value.__slate_anon_2.x;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = value.__slate_anon_2.y;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %zu\n".as_ptr(),
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             16 as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         value.__slate_anon_1.real = 2.5;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d\n".as_ptr(),
// REWRITES-NEXT:             (unsafe { value.__slate_anon_1.real }) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
