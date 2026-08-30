// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[cfg(target_arch = "x86_64")]
// REWRITES-NEXT: core::arch::global_asm!(".weak pragma_weak_alias\n.set pragma_weak_alias, pragma_weak_target", options(att_syntax, raw));
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn pragma_weak_alias(_0: i32) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[unsafe(no_mangle)]
// REWRITES-NEXT: pub extern "C" fn pragma_weak_target({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 7;
// REWRITES-NEXT: __retval = value + {{_v[0-9]+}};
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn pragma_actual({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: __retval = value * {{_v[0-9]+}};
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 29;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { pragma_weak_alias({{_v[0-9]+}} as i32) };
// REWRITES-NEXT: let {{_v[0-9]+}}: bool = (Some(pragma_weak_alias).unwrap() as *const u8) == (Some(pragma_weak_target).unwrap() as *const u8);
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 13;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = pragma_actual({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites

// LOWERING-DAG: .weak pragma_weak_alias\n.set pragma_weak_alias, pragma_weak_target
// LOWERING-DAG: fn pragma_weak_alias(_0: i32) -> i32;
// LOWERING-DAG: fn pragma_actual(arg0: i32) -> i32 {
// LOWERING-NOT: pragma_renamed

#include <stdio.h>

int pragma_weak_target(int value) { return value + 7; }

#pragma weak pragma_weak_alias = pragma_weak_target
extern int   pragma_weak_alias(int);

#pragma redefine_extname pragma_renamed pragma_actual
int pragma_renamed(int value) { return value * 3; }

int main(void) {
  printf("%d %d %d\n", pragma_weak_alias(29),
         pragma_weak_alias == pragma_weak_target, pragma_renamed(13));
  return 0;
}
