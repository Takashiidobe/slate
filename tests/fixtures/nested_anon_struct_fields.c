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
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-NEXT:     alias: {{_unnamed_at_[0-9A-Za-z_]+}},
// LOWERING-NEXT:     tag: {{_unnamed_at_[0-9A-Za-z_]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-X86_64-GNU-NEXT:     value: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     value: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// LOWERING-X86_64-GNU-NEXT:     handle: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     suffix: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     handle: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     suffix: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: union {{anon_[0-9]+}} {
// LOWERING-NEXT:     __slate_anon_0: {{anon_[0-9]+}},
// LOWERING-NEXT:     tag: {{anon_[0-9]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_0: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     __slate_anon_0: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct {{anon_[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:     handle: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     suffix: *mut i8,
// LOWERING-AARCH64-GNU-NEXT:     handle: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     suffix: *mut u8,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct event {
// LOWERING-NEXT:     r#type: i32,
// LOWERING-NEXT:     data: {{_unnamed_at_[0-9A-Za-z_]+}},
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut e: event = event {
// LOWERING-NEXT:         r#type: 0,
// LOWERING-NEXT:         data: unsafe {
// LOWERING-NEXT:             std::mem::zeroed::<{{_unnamed_at_[0-9A-Za-z_]+}}>()
// LOWERING-NEXT:         },
// LOWERING-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let mut h: [i8; 2] = [0; 2];
// LOWERING-X86_64-GNU-NEXT:     let mut s: [i8; 2] = [0; 2];
// LOWERING-AARCH64-GNU-NEXT:     let mut h: [u8; 2] = [0; 2];
// LOWERING-AARCH64-GNU-NEXT:     let mut s: [u8; 2] = [0; 2];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     e.r#type = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 2] = [72, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 2] = [72, 0];
// LOWERING-NEXT:     h = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 2] = [83, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 2] = [83, 0];
// LOWERING-NEXT:     s = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = h.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = h.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         e.data.tag.handle = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = s.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = s.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         e.data.tag.suffix = {{__v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %s%s\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %s%s\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = e.r#type;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { e.data.tag.handle };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { e.data.tag.suffix };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { e.data.tag.handle };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { e.data.tag.suffix };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-NEXT:     alias: {{_unnamed_at_[0-9A-Za-z_]+}},
// REWRITES-NEXT:     tag: {{_unnamed_at_[0-9A-Za-z_]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-X86_64-GNU-NEXT:     value: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     value: *mut u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// REWRITES-X86_64-GNU-NEXT:     handle: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     suffix: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     handle: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     suffix: *mut u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: union {{anon_[0-9]+}} {
// REWRITES-NEXT:     __slate_anon_0: {{anon_[0-9]+}},
// REWRITES-NEXT:     tag: {{anon_[0-9]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_0: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     __slate_anon_0: *mut u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct {{anon_[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:     handle: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     suffix: *mut i8,
// REWRITES-AARCH64-GNU-NEXT:     handle: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     suffix: *mut u8,
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct event {
// REWRITES-NEXT:     r#type: i32,
// REWRITES-NEXT:     data: {{_unnamed_at_[0-9A-Za-z_]+}},
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut e: event = event {
// REWRITES-NEXT:         r#type: 0,
// REWRITES-NEXT:         data: unsafe {
// REWRITES-NEXT:             std::mem::zeroed::<{{_unnamed_at_[0-9A-Za-z_]+}}>()
// REWRITES-NEXT:         },
// REWRITES-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let mut h: [i8; 2] = [0; 2];
// REWRITES-X86_64-GNU-NEXT:     let mut s: [i8; 2] = [0; 2];
// REWRITES-AARCH64-GNU-NEXT:     let mut h: [u8; 2] = [0; 2];
// REWRITES-AARCH64-GNU-NEXT:     let mut s: [u8; 2] = [0; 2];
// REWRITES-NEXT:     e.r#type = 1;
// REWRITES-NEXT:     h = [72, 0];
// REWRITES-NEXT:     s = [83, 0];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = h.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = h.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         e.data.tag.handle = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = s.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = s.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         e.data.tag.suffix = {{__v[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = e.r#type;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { e.data.tag.handle };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { e.data.tag.suffix };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { e.data.tag.handle };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { e.data.tag.suffix };
// REWRITES-NEXT:     unsafe { printf(c"%d %s%s\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
