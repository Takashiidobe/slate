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
// COMMON-LOWERING-NEXT: union {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT:     integer: i32,
// COMMON-LOWERING-NEXT:     real: f32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT:     x: i32,
// COMMON-LOWERING-NEXT:     y: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct container {
// COMMON-LOWERING-NEXT:     prefix: i32,
// COMMON-LOWERING-NEXT:     __slate_anon_1: {{anon_[0-9]+}},
// COMMON-LOWERING-NEXT:     __slate_anon_2: {{anon_[0-9]+}},
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut value: container = container {
// COMMON-LOWERING-NEXT:         prefix: 0,
// COMMON-LOWERING-NEXT:         __slate_anon_1: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:         __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: container = container {
// COMMON-LOWERING-NEXT:         prefix: 0,
// COMMON-LOWERING-NEXT:         __slate_anon_1: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-LOWERING-NEXT:         __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     value = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     value.prefix = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 31;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         value.__slate_anon_1.integer = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 37;
// COMMON-LOWERING-NEXT:     value.__slate_anon_2.x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 41;
// COMMON-LOWERING-NEXT:     value.__slate_anon_2.y = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = value.prefix;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { value.__slate_anon_1.integer };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = value.__slate_anon_2.x;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = value.__slate_anon_2.y;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = 16;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = 2.5;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         value.__slate_anon_1.real = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: f32 = unsafe { value.__slate_anon_1.real };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: union {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT:     integer: i32,
// COMMON-REWRITES-NEXT:     real: f32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT:     x: i32,
// COMMON-REWRITES-NEXT:     y: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct container {
// COMMON-REWRITES-NEXT:     prefix: i32,
// COMMON-REWRITES-NEXT:     __slate_anon_1: {{anon_[0-9]+}},
// COMMON-REWRITES-NEXT:     __slate_anon_2: {{anon_[0-9]+}},
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut value: container = container {
// COMMON-REWRITES-NEXT:         prefix: 0,
// COMMON-REWRITES-NEXT:         __slate_anon_1: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:         __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     value = container {
// COMMON-REWRITES-NEXT:         prefix: 0,
// COMMON-REWRITES-NEXT:         __slate_anon_1: unsafe { std::mem::zeroed::<{{anon_[0-9]+}}>() },
// COMMON-REWRITES-NEXT:         __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     value.prefix = 3;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         value.__slate_anon_1.integer = 31;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     value.__slate_anon_2.x = 37;
// COMMON-REWRITES-NEXT:     value.__slate_anon_2.y = 41;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = value.prefix;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { value.__slate_anon_1.integer };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = value.__slate_anon_2.x;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = value.__slate_anon_2.y;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d %zu\n".as_ptr(),
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             {{__v[0-9]+}},
// COMMON-REWRITES-NEXT:             16 as u64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         value.__slate_anon_1.real = 2.5;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             (unsafe { value.__slate_anon_1.real }) as i32,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
