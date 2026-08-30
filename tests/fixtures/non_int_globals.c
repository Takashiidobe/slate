#include <stdio.h>

static char          small = 12;
static unsigned char byte  = 200;
static float         ratio = 1.5f;
static double        total = 2.25;

static char add_char(char a, char b) { return a + b; }

static float scale(float value, float factor) { return value * factor; }

static double add_double(double a, double b) { return a + b; }

int main(void) {
  small = add_char(small, 3);
  byte  = byte + 1;
  ratio = scale(ratio, 2.0f);
  total = add_double(total, ratio);
  printf("%d\n", small);
  printf("%u\n", byte);
  printf("%f\n", ratio);
  printf("%f\n", total);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut byte: u8 = 200;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut ratio: f32 = 1.5;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut small: i8 = 12;
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut total: f64 = 2.25;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_char({{arg[0-9]+}}: i8, {{arg[0-9]+}}: i8) -> i8 {
// LOWERING-NEXT:     let mut a: i8 = 0;
// LOWERING-NEXT:     let mut b: i8 = 0;
// LOWERING-NEXT:     let mut __retval: i8 = 0;
// LOWERING-NEXT:     a = {{arg[0-9]+}};
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = {{_v[0-9]+}} as i8;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn scale({{arg[0-9]+}}: f32, {{arg[0-9]+}}: f32) -> f32 {
// LOWERING-NEXT:     let mut value: f32 = 0.0;
// LOWERING-NEXT:     let mut factor: f32 = 0.0;
// LOWERING-NEXT:     let mut __retval: f32 = 0.0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     factor = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = value;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = factor;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn add_double({{arg[0-9]+}}: f64, {{arg[0-9]+}}: f64) -> f64 {
// LOWERING-NEXT:     let mut a: f64 = 0.0;
// LOWERING-NEXT:     let mut b: f64 = 0.0;
// LOWERING-NEXT:     let mut __retval: f64 = 0.0;
// LOWERING-NEXT:     a = {{arg[0-9]+}};
// LOWERING-NEXT:     b = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { small };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = add_char({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         small = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { byte };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         byte = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { ratio };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = 2.0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = scale({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         ratio = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { total };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { ratio };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = add_double({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         total = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = unsafe { small };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = unsafe { byte };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { ratio };
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} as f64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { total };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut byte: u8 = 200;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut ratio: f32 = 1.5;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut small: i8 = 12;
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut total: f64 = 2.25;
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_char({{arg[0-9]+}}: i8, {{arg[0-9]+}}: i8) -> i8 {
// REWRITES-NEXT: let mut a: i8 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut b: i8 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i8 = 0;
// REWRITES-NEXT: __retval = ((a as i32) + (b as i32)) as i8;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn scale({{arg[0-9]+}}: f32, {{arg[0-9]+}}: f32) -> f32 {
// REWRITES-NEXT: let mut value: f32 = 0.0;
// REWRITES-NEXT: let mut factor: f32 = 0.0;
// REWRITES-NEXT: let mut __retval: f32 = 0.0;
// REWRITES-NEXT: value = {{arg[0-9]+}};
// REWRITES-NEXT: factor = {{arg[0-9]+}};
// REWRITES-NEXT: __retval = value * factor;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn add_double({{arg[0-9]+}}: f64, {{arg[0-9]+}}: f64) -> f64 {
// REWRITES-NEXT: let mut a: f64 = 0.0;
// REWRITES-NEXT: let mut b: f64 = 0.0;
// REWRITES-NEXT: let mut __retval: f64 = 0.0;
// REWRITES-NEXT: a = {{arg[0-9]+}};
// REWRITES-NEXT: b = {{arg[0-9]+}};
// REWRITES-NEXT: __retval = a + b;
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i8 = 3;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         small = add_char(unsafe { small }, {{_v[0-9]+}});
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         byte = (((unsafe { byte }) as i32) + {{_v[0-9]+}}) as u8;
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: f32 = 2.0;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         ratio = scale(unsafe { ratio }, {{_v[0-9]+}});
// REWRITES-NEXT: }
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         total = add_double(unsafe { total }, (unsafe { ratio }) as f64);
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, (unsafe { small }) as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%u\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, (unsafe { byte }) as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, (unsafe { ratio }) as f64) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%f\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, unsafe { total }) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
