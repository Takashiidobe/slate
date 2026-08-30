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
// LOWERING-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-NEXT:     alias: {{_unnamed_at_[0-9A-Za-z_]+}},
// LOWERING-NEXT:     tag: {{_unnamed_at_[0-9A-Za-z_]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-NEXT:     value: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-NEXT:     handle: *mut i8,
// LOWERING-NEXT:     suffix: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     __slate_anon_0: {{anon_[0-9]+}},
// LOWERING-NEXT:     tag: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-NEXT:     __slate_anon_0: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-NEXT:     handle: *mut i8,
// LOWERING-NEXT:     suffix: *mut i8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct event {
// LOWERING-NEXT:     r#type: i32,
// LOWERING-NEXT:     data: {{_unnamed_at_[0-9A-Za-z_]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut e: event = event { r#type: 0, data: unsafe { std::mem::zeroed::<{{_unnamed_at_[0-9A-Za-z_]+}}>() } };
// LOWERING-NEXT:     let mut h: [i8; 2] = [0; 2];
// LOWERING-NEXT:     let mut s: [i8; 2] = [0; 2];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     e.r#type = {{_v[0-9]+}};
// LOWERING-NEXT:     h = [72, 0];
// LOWERING-NEXT:     s = [83, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = h.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         e.data.tag.handle = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = s.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         e.data.tag.suffix = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %s%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = e.r#type;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { e.data.tag.handle };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = unsafe { e.data.tag.suffix };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-NEXT:     alias: {{_unnamed_at_[0-9A-Za-z_]+}},
// REWRITES-NEXT:     tag: {{_unnamed_at_[0-9A-Za-z_]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-NEXT:     value: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-NEXT:     handle: *mut i8,
// REWRITES-NEXT:     suffix: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{anon_[0-9]+}} {
// REWRITES-NEXT:     __slate_anon_0: {{anon_[0-9]+}},
// REWRITES-NEXT:     tag: {{anon_[0-9]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_[0-9]+}} {
// REWRITES-NEXT:     __slate_anon_0: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_[0-9]+}} {
// REWRITES-NEXT:     handle: *mut i8,
// REWRITES-NEXT:     suffix: *mut i8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct event {
// REWRITES-NEXT:     r#type: i32,
// REWRITES-NEXT:     data: {{_unnamed_at_[0-9A-Za-z_]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut e: event = event { r#type: 0, data: unsafe { std::mem::zeroed::<{{_unnamed_at_[0-9A-Za-z_]+}}>() } };
// REWRITES-NEXT: let mut h: [i8; 2] = [0; 2];
// REWRITES-NEXT: let mut s: [i8; 2] = [0; 2];
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: e.r#type = 1;
// REWRITES-NEXT: h = [72, 0];
// REWRITES-NEXT: s = [83, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = h.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         e.data.tag.handle = {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = s.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         e.data.tag.suffix = {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %s%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = e.r#type;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { e.data.tag.handle };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = unsafe { e.data.tag.suffix };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
