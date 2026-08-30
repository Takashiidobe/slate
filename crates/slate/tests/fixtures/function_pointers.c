#include <stdio.h>

static int add_pair(int lhs, int rhs) { return lhs + rhs; }

static int mul_pair(int lhs, int rhs) { return lhs * rhs; }

static int apply_binary(int (*op)(int, int), int lhs, int rhs) {
  return op(lhs, rhs);
}

int main(void) {
  int (*chosen)(int, int) = add_pair;
  printf("%d\n", apply_binary(chosen, 4, 5));
  chosen = mul_pair;
  printf("%d\n", chosen(3, 6));
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn add_pair({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut lhs: i32 = 0;
// LOWERING-NEXT:     let mut rhs: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     lhs = {{arg[0-9]+}};
// LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = lhs;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = rhs;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn apply_binary({{arg[0-9]+}}: Option<unsafe extern "C" fn(i32, i32) -> i32>, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut op: Option<unsafe extern "C" fn(i32, i32) -> i32> = None;
// LOWERING-NEXT:     let mut lhs: i32 = 0;
// LOWERING-NEXT:     let mut rhs: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     op = {{arg[0-9]+}};
// LOWERING-NEXT:     lhs = {{arg[0-9]+}};
// LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32, i32) -> i32> = op;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = lhs;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = rhs;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C" fn mul_pair({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut lhs: i32 = 0;
// LOWERING-NEXT:     let mut rhs: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     lhs = {{arg[0-9]+}};
// LOWERING-NEXT:     rhs = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = lhs;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = rhs;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut chosen: Option<unsafe extern "C" fn(i32, i32) -> i32> = None;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     chosen = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32, i32) -> i32>>(add_pair as *const ()) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32, i32) -> i32> = chosen;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = apply_binary({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     chosen = unsafe { std::mem::transmute::<*const (), Option<unsafe extern "C" fn(i32, i32) -> i32>>(mul_pair as *const ()) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: Option<unsafe extern "C" fn(i32, i32) -> i32> = chosen;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: add_pair as *const ()
// REWRITES-DAG: Option<unsafe extern "C" fn(i32, i32) -> i32>>(add_pair as *const ())
// REWRITES-DAG: .unwrap()(
// REWRITES-DAG: lhs + rhs
// REWRITES-DAG: lhs * rhs
// REWRITES-NOT: return _v
// REWRITES-NOT: let mut __retval: i32 = lhs
