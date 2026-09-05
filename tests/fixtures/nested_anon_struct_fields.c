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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-LOWERING-NEXT:     alias: {{_unnamed_at_[0-9A-Za-z_]+}},
// COMMON-LOWERING-NEXT:     tag: {{_unnamed_at_[0-9A-Za-z_]+}},
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: union {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT:     __slate_anon_0: {{anon_[0-9]+}},
// COMMON-LOWERING-NEXT:     tag: {{anon_[0-9]+}},
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct {{anon_[0-9]+}} {
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: #[repr(C)]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct event {
// COMMON-LOWERING-NEXT:     r#type: i32,
// COMMON-LOWERING-NEXT:     data: {{_unnamed_at_[0-9A-Za-z_]+}},
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut e: event = event {
// COMMON-LOWERING-NEXT:         r#type: 0,
// COMMON-LOWERING-NEXT:         data: unsafe {
// COMMON-LOWERING-NEXT:             std::mem::zeroed::<{{_unnamed_at_[0-9A-Za-z_]+}}>()
// COMMON-LOWERING-NEXT:         },
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     e.r#type = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     h = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     s = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         e.data.tag.handle = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         e.data.tag.suffix = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = e.r#type;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     value: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     handle: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     suffix: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     __slate_anon_0: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     handle: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     suffix: *mut i8,
// LOWERING-X86_64-GNU-NEXT:     let mut h: [i8; 2] = [0; 2];
// LOWERING-X86_64-GNU-NEXT:     let mut s: [i8; 2] = [0; 2];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 2] = [72, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 2] = [83, 0];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = h.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = s.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %s%s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { e.data.tag.handle };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { e.data.tag.suffix };
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     value: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     handle: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     suffix: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     __slate_anon_0: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     handle: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     suffix: *mut u8,
// LOWERING-AARCH64-GNU-NEXT:     let mut h: [u8; 2] = [0; 2];
// LOWERING-AARCH64-GNU-NEXT:     let mut s: [u8; 2] = [0; 2];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 2] = [72, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 2] = [83, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = h.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = s.as_mut_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %s%s\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { e.data.tag.handle };
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { e.data.tag.suffix };
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-REWRITES-NEXT:     alias: {{_unnamed_at_[0-9A-Za-z_]+}},
// COMMON-REWRITES-NEXT:     tag: {{_unnamed_at_[0-9A-Za-z_]+}},
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct {{_unnamed_at_[0-9A-Za-z_]+}} {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: union {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT:     __slate_anon_0: {{anon_[0-9]+}},
// COMMON-REWRITES-NEXT:     tag: {{anon_[0-9]+}},
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct {{anon_[0-9]+}} {
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: #[repr(C)]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct event {
// COMMON-REWRITES-NEXT:     r#type: i32,
// COMMON-REWRITES-NEXT:     data: {{_unnamed_at_[0-9A-Za-z_]+}},
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut e: event = event {
// COMMON-REWRITES-NEXT:         r#type: 0,
// COMMON-REWRITES-NEXT:         data: unsafe {
// COMMON-REWRITES-NEXT:             std::mem::zeroed::<{{_unnamed_at_[0-9A-Za-z_]+}}>()
// COMMON-REWRITES-NEXT:         },
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     e.r#type = 1;
// COMMON-REWRITES-NEXT:     h = [72, 0];
// COMMON-REWRITES-NEXT:     s = [83, 0];
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         e.data.tag.handle = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         e.data.tag.suffix = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = e.r#type;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %s%s\n".as_ptr(), {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     value: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     handle: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     suffix: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     __slate_anon_0: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     handle: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     suffix: *mut i8,
// REWRITES-X86_64-GNU-NEXT:     let mut h: [i8; 2] = [0; 2];
// REWRITES-X86_64-GNU-NEXT:     let mut s: [i8; 2] = [0; 2];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = h.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = s.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { e.data.tag.handle };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = unsafe { e.data.tag.suffix };
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     value: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     handle: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     suffix: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     __slate_anon_0: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     handle: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     suffix: *mut u8,
// REWRITES-AARCH64-GNU-NEXT:     let mut h: [u8; 2] = [0; 2];
// REWRITES-AARCH64-GNU-NEXT:     let mut s: [u8; 2] = [0; 2];
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = h.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = s.as_mut_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { e.data.tag.handle };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = unsafe { e.data.tag.suffix };
// SLATE-FILECHECK-END rewrites-aarch64-gnu
