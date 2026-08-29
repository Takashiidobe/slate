#include <stdio.h>

struct event {
  int type;
  union {
    struct {
      char *value;
    } alias;
    struct {
      char *handle;
      char *suffix;
    } tag;
  } data;
};

int main(void) {
  struct event e;
  e.type            = 1;
  char h[]          = "H";
  char s[]          = "S";
  e.data.tag.handle = h;
  e.data.tag.suffix = s;
  printf("%d %s%s\n", e.type, e.data.tag.handle, e.data.tag.suffix);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_5_3_ {
// LOWERING-NEXT:     alias: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_6_5_,
// LOWERING-NEXT:     tag: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_9_5_,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_6_5_ {
// LOWERING-NEXT:     value: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_9_5_ {
// LOWERING-NEXT:     handle: *mut i8,
// LOWERING-NEXT:     suffix: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union anon_0 {
// LOWERING-NEXT:     __slate_anon_0: anon_1,
// LOWERING-NEXT:     tag: anon_2,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct anon_1 {
// LOWERING-NEXT:     __slate_anon_0: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct anon_2 {
// LOWERING-NEXT:     handle: *mut i8,
// LOWERING-NEXT:     suffix: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct event {
// LOWERING-NEXT:     r#type: i32,
// LOWERING-NEXT:     data: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_5_3_,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut e: event = event { r#type: 0, data: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_5_3_ { alias: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_6_5_ { value: std::ptr::null_mut() } } };
// LOWERING-NEXT:     let mut h: [i8; 2] = [0; 2];
// LOWERING-NEXT:     let mut s: [i8; 2] = [0; 2];
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 1;
// LOWERING-NEXT:     e.r#type = _v1;
// LOWERING-NEXT:     h = [72, 0];
// LOWERING-NEXT:     s = [83, 0];
// LOWERING-NEXT:     let _v2: *mut i8 = h.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         e.data.tag.handle = _v2;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v3: *mut i8 = s.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         e.data.tag.suffix = _v3;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v4: *mut i8 = b"%d %s%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i32 = e.r#type;
// LOWERING-NEXT:     let _v6: *mut i8 = unsafe { e.data.tag.handle };
// LOWERING-NEXT:     let _v7: *mut i8 = unsafe { e.data.tag.suffix };
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v4 as *const i8, _v5, _v6, _v7) };
// LOWERING-NEXT:     let _v9: i32 = 0;
// LOWERING-NEXT:     __retval = _v9;
// LOWERING-NEXT:     let _v10: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v10 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_5_3_ {
// REWRITES-NEXT:     alias: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_6_5_,
// REWRITES-NEXT:     tag: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_9_5_,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_6_5_ {
// REWRITES-NEXT:     value: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_9_5_ {
// REWRITES-NEXT:     handle: *mut i8,
// REWRITES-NEXT:     suffix: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union anon_0 {
// REWRITES-NEXT:     __slate_anon_0: anon_1,
// REWRITES-NEXT:     tag: anon_2,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct anon_1 {
// REWRITES-NEXT:     __slate_anon_0: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct anon_2 {
// REWRITES-NEXT:     handle: *mut i8,
// REWRITES-NEXT:     suffix: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct event {
// REWRITES-NEXT:     r#type: i32,
// REWRITES-NEXT:     data: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_5_3_,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut e: event = event { r#type: 0, data: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_5_3_ { alias: _unnamed_at__home_takashi_Projects_slate_tests_fixtures_nested_{{anon_struct[0-9A-Za-z_]*}}_fields_c_6_5_ { value: std::ptr::null_mut() } } };
// REWRITES-NEXT: let mut h: [i8; 2] = [0; 2];
// REWRITES-NEXT: let mut s: [i8; 2] = [0; 2];
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: e.r#type = 1;
// REWRITES-NEXT: h = [72, 0];
// REWRITES-NEXT: s = [83, 0];
// REWRITES-NEXT: let _v2: *mut i8 = h.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         e.data.tag.handle = _v2;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v3: *mut i8 = s.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         e.data.tag.suffix = _v3;
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v4: *mut i8 = b"%d %s%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i32 = e.r#type;
// REWRITES-NEXT: let _v6: *mut i8 = unsafe { e.data.tag.handle };
// REWRITES-NEXT: let _v7: *mut i8 = unsafe { e.data.tag.suffix };
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v4 as *const i8, _v5, _v6, _v7) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
