#include <stdio.h>

struct Conditional {
  enum { IN_THEN, IN_ELIF = 4, IN_ELSE } ctx;
};

int main(void) {
  struct Conditional conditional = {IN_THEN};
  conditional.ctx                = IN_ELSE;
  printf("%d %d\n", (int)conditional.ctx, IN_ELIF);
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[allow(non_camel_case_types)]
// LOWERING-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// LOWERING-NEXT: enum _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_ {
// LOWERING-NEXT:     IN_THEN = 0,
// LOWERING-NEXT:     IN_ELIF = 4,
// LOWERING-NEXT:     IN_ELSE = 5,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Conditional {
// LOWERING-NEXT:     ctx: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut conditional: Conditional = Conditional { ctx: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_::IN_THEN };
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     conditional = Conditional { ctx: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_::IN_THEN };
// LOWERING-NEXT:     let _v1: u32 = 5;
// LOWERING-NEXT:     conditional.ctx = _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_::IN_ELSE;
// LOWERING-NEXT:     let _v2: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: u32 = conditional.ctx as u32;
// LOWERING-NEXT:     let _v4: i32 = _v3 as i32;
// LOWERING-NEXT:     let _v5: i32 = 4;
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v2 as *const i8, _v4, _v5) };
// LOWERING-NEXT:     let _v7: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v7 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[allow(non_camel_case_types)]
// REWRITES-NEXT: #[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
// REWRITES-NEXT: enum _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_ {
// REWRITES-NEXT:     IN_THEN = 0,
// REWRITES-NEXT:     IN_ELIF = 4,
// REWRITES-NEXT:     IN_ELSE = 5,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct Conditional {
// REWRITES-NEXT:     ctx: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut conditional: Conditional = Conditional { ctx: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_::IN_THEN };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: conditional = Conditional { ctx: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_::IN_THEN };
// REWRITES-NEXT: let _v1: u32 = 5;
// REWRITES-NEXT: conditional.ctx = _unnamed_at__home_takashi_Projects_slate_tests_fixtures_anonymous_enum_record_field_c_4_3_::IN_ELSE;
// REWRITES-NEXT: let _v2: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = 4;
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v2 as *const i8, (conditional.ctx as u32) as i32, _v5) };
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
