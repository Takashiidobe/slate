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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     integer: i32,
// LOWERING-NEXT:     real: f32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-NEXT:     x: i32,
// LOWERING-NEXT:     y: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct container {
// LOWERING-NEXT:     prefix: i32,
// LOWERING-NEXT:     __slate_anon_1: {{anon_[0-9]+}},
// LOWERING-NEXT:     __slate_anon_2: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut value: container = container { prefix: 0, __slate_anon_1: {{anon_[0-9]+}} { integer: 0 }, __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 } };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     value = container { prefix: 0, __slate_anon_1: {{anon_[0-9]+}} { integer: 0 }, __slate_anon_2: {{anon_[0-9]+}} { x: 0, y: 0 } };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     value.prefix = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 31;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         value.__slate_anon_1.integer = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 37;
// LOWERING-NEXT:     value.__slate_anon_2.x = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     value.__slate_anon_2.y = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = value.prefix;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { value.__slate_anon_1.integer };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = value.__slate_anon_2.x;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = value.__slate_anon_2.y;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 2.5;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         value.__slate_anon_1.real = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { value.__slate_anon_1.real };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: union anon_0 {
// REWRITES-DAG: integer: i32,
// REWRITES-DAG: real: f32,
// REWRITES-DAG: struct container {
// REWRITES-DAG: __slate_anon_1: anon_0,
// REWRITES-DAG: value.__slate_anon_1.integer
