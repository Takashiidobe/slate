#include <stdio.h>

static int *identity_mut(int *value) { return value; }

static int *forward_mut(int *value) { return identity_mut(value); }

static const int *identity_const(const int *value) { return value; }

static int *choose_value(int *first, int *second, int choose_first) {
  if (choose_first)
    return first;
  return second;
}

int main(void) {
  int first = 20;
  int second = 22;
  int *alias = forward_mut(&first);
  *alias += 2;
  const int *read_alias = identity_const(&second);
  int *ambiguous = choose_value(&first, &second, 1);
  printf("%d %d %d\n", first, *read_alias, *ambiguous);
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
// LOWERING-NEXT: fn identity_mut({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// LOWERING-NEXT:     return {{arg[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn forward_mut({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = identity_mut({{arg[0-9]+}});
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn identity_const({{arg[0-9]+}}: *mut i32) -> *mut i32 {
// LOWERING-NEXT:     return {{arg[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn choose_value({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> *mut i32 {
// LOWERING-NEXT:     let mut first: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut choose_first: i32 = 0;
// LOWERING-NEXT:     let mut __retval: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     first = {{arg[0-9]+}};
// LOWERING-NEXT:     choose_first = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = choose_first;
// LOWERING-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:         if {{_v[0-9]+}} {
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = first;
// LOWERING-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = __retval;
// LOWERING-NEXT:             return {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     __retval = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut first: i32 = 0;
// LOWERING-NEXT:     let mut second: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:     first = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 22;
// LOWERING-NEXT:     second = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = forward_mut(std::ptr::addr_of_mut!(first));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = identity_const(std::ptr::addr_of_mut!(second));
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = choose_value(std::ptr::addr_of_mut!(first), std::ptr::addr_of_mut!(second), {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = first;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: fn identity_mut(arg{{[0-9]+}}: &mut i32) -> *mut i32
// REWRITES-DAG: fn forward_mut(arg{{[0-9]+}}: &mut i32) -> *mut i32
// REWRITES-DAG: fn identity_const(arg{{[0-9]+}}: &i32) -> *mut i32
// REWRITES-DAG: fn choose_value(arg{{[0-9]+}}: *mut i32, arg{{[0-9]+}}: *mut i32, arg{{[0-9]+}}: i32) -> *mut i32
