
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(linkage)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     #[linkage = "extern_weak"]
// LOWERING-NEXT:     static abs: Option<unsafe extern "C" fn(i32) -> i32>;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn weakref_target({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 35;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = weakref_target({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = weakref_target({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -53;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { abs.unwrap()({{_v[0-9]+}} as i32) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: #[linkage = "extern_weak"]
// REWRITES-DAG: static abs: Option<unsafe extern "C" fn(i32) -> i32>;
// REWRITES-NOT: fn weakref_alias

#include <stdio.h>
#include <stdlib.h>

int weakref_target(int value) { return value + 7; }

static int weakref_alias(int) __attribute__((weakref("weakref_target")));

static int weakref_external(int) __attribute__((weakref("abs")));

int main(void) {
  printf("%d %d %d\n", weakref_alias(35), weakref_target(41),
         weakref_external(-53));
  return 0;
}
