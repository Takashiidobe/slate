#include <stdio.h>

static void source_probe(void) {
  printf("%s|%s|%d|%d\n", __builtin_FUNCTION(), __builtin_FILE(),
         __builtin_LINE(), __builtin_COLUMN());
}

int main(void) {
  source_probe();
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
// LOWERING-NEXT: fn source_probe() {
// LOWERING-NEXT:     let _v0: *mut i8 = b"%s|%s|%d|%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v1: *mut i8 = b"source_probe\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i8 = b"{{.*}}tests/fixtures/gnu_builtin_source.c\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: u32 = 5;
// LOWERING-NEXT:     let _v4: u32 = 28;
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v0 as *const i8, _v1, _v2, _v3, _v4) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     source_probe();
// LOWERING-NEXT:     let _v1: i32 = 0;
// LOWERING-NEXT:     __retval = _v1;
// LOWERING-NEXT:     let _v2: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v2 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn source_probe() {
// REWRITES-NEXT: let _v0: *mut i8 = b"%s|%s|%d|%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v1: *mut i8 = b"source_probe\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = b"{{.*}}tests/fixtures/gnu_builtin_source.c\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: u32 = 5;
// REWRITES-NEXT: let _v4: u32 = 28;
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v0 as *const i8, _v1, _v2, _v3, _v4) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: source_probe();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
