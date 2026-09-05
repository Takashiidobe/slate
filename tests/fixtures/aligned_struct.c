#include <stddef.h>
#include <stdio.h>

struct __attribute__((aligned(16))) Aligned {
  char a;
  int  b;
};

static int next_value(void) { return 9; }

static int effectful_case(void) {
  struct Aligned effectful;
  effectful.a = next_value();
  effectful.b = 7;
  return effectful.a + effectful.b;
}

static int repeated_case(void) {
  struct Aligned repeated;
  repeated.a = 1;
  repeated.a = 2;
  repeated.b = 3;
  return repeated.a + repeated.b;
}

static int dependent_case(void) {
  struct Aligned dependent;
  dependent.b = 8;
  dependent.a = dependent.b;
  return dependent.a + dependent.b;
}

static int counter;

static void touch(void) { counter++; }

static int interrupted_case(void) {
  struct Aligned interrupted;
  interrupted.a = 4;
  touch();
  interrupted.b = 6;
  return interrupted.a + interrupted.b + counter;
}

int main(void) {
  struct Aligned s;
  s.a = 5;
  s.b = 0x1234;

  printf("%zu %zu\n", sizeof(struct Aligned), _Alignof(struct Aligned));
  printf("%zu %zu\n", offsetof(struct Aligned, a), offsetof(struct Aligned, b));
  printf("%d %x\n", s.a, s.b);
  printf("%d %d %d %d\n", effectful_case(), repeated_case(), dependent_case(),
         interrupted_case());
  return 0;
}

// REWRITES-DAG: std::mem::size_of::<Aligned>() as u64
// REWRITES-DAG: std::mem::align_of::<Aligned>() as u64
// REWRITES-DAG: std::mem::offset_of!(Aligned, a) as u64
// REWRITES-DAG: std::mem::offset_of!(Aligned, b) as u64
// REWRITES-NOT: let mut s: Aligned

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
// COMMON-LOWERING-NEXT: #[repr(C, align(16))]
// COMMON-LOWERING-NEXT: #[derive(Clone, Copy)]
// COMMON-LOWERING-NEXT: struct Aligned {
// COMMON-LOWERING-NEXT:     b: i32,
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: static mut counter: i32 = 0;
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut s: aligned::Aligned<aligned::A16, Aligned> = aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     s.a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4660;
// COMMON-LOWERING-NEXT:     s.b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::size_of::<Aligned>() as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::align_of::<Aligned>() as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(Aligned, a) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = std::mem::offset_of!(Aligned, b) as u64;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = effectful_case();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = repeated_case();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = dependent_case();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = interrupted_case();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn effectful_case() -> i32 {
// COMMON-LOWERING-NEXT:     let mut effectful: aligned::Aligned<aligned::A16, Aligned> =
// COMMON-LOWERING-NEXT:         aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = next_value();
// COMMON-LOWERING-NEXT:     effectful.a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     effectful.b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = effectful.b;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn repeated_case() -> i32 {
// COMMON-LOWERING-NEXT:     let mut repeated: aligned::Aligned<aligned::A16, Aligned> =
// COMMON-LOWERING-NEXT:         aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-LOWERING-NEXT:     repeated.a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     repeated.a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     repeated.b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = repeated.b;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn dependent_case() -> i32 {
// COMMON-LOWERING-NEXT:     let mut dependent: aligned::Aligned<aligned::A16, Aligned> =
// COMMON-LOWERING-NEXT:         aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     dependent.b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = dependent.b;
// COMMON-LOWERING-NEXT:     dependent.a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = dependent.b;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn interrupted_case() -> i32 {
// COMMON-LOWERING-NEXT:     let mut interrupted: aligned::Aligned<aligned::A16, Aligned> =
// COMMON-LOWERING-NEXT:         aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-LOWERING-NEXT:     interrupted.a = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     touch();
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:     interrupted.b = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = interrupted.b;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { counter };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn next_value() -> i32 {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn touch() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { counter };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:     unsafe {
// COMMON-LOWERING-NEXT:         counter = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     return;
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     a: i8,
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 5;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %x\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = s.a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = effectful.a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = repeated.a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = dependent.a;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = interrupted.a;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     a: u8,
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 5;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%zu %zu\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %x\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = s.a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = effectful.a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 1;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 2;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = repeated.a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = dependent.a;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 4;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = interrupted.a;
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
// COMMON-REWRITES-NEXT: #[repr(C, align(16))]
// COMMON-REWRITES-NEXT: #[derive(Clone, Copy)]
// COMMON-REWRITES-NEXT: struct Aligned {
// COMMON-REWRITES-NEXT:     b: i32,
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: static mut counter: i32 = 0;
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut s: aligned::Aligned<aligned::A16, Aligned> = aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-REWRITES-NEXT:     s.a = 5;
// COMMON-REWRITES-NEXT:     s.b = 4660;
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%zu %zu\n".as_ptr(),
// COMMON-REWRITES-NEXT:             std::mem::size_of::<Aligned>() as u64,
// COMMON-REWRITES-NEXT:             std::mem::align_of::<Aligned>() as u64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%zu %zu\n".as_ptr(),
// COMMON-REWRITES-NEXT:             std::mem::offset_of!(Aligned, a) as u64,
// COMMON-REWRITES-NEXT:             std::mem::offset_of!(Aligned, b) as u64,
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = s.b;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d %x\n".as_ptr(), s.a as i32, {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             effectful_case(),
// COMMON-REWRITES-NEXT:             repeated_case(),
// COMMON-REWRITES-NEXT:             dependent_case(),
// COMMON-REWRITES-NEXT:             interrupted_case(),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn effectful_case() -> i32 {
// COMMON-REWRITES-NEXT:     let mut effectful: aligned::Aligned<aligned::A16, Aligned> =
// COMMON-REWRITES-NEXT:         aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = next_value();
// COMMON-REWRITES-NEXT:     effectful.b = 7;
// COMMON-REWRITES-NEXT:     (effectful.a as i32) + effectful.b
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn repeated_case() -> i32 {
// COMMON-REWRITES-NEXT:     let mut repeated: aligned::Aligned<aligned::A16, Aligned> =
// COMMON-REWRITES-NEXT:         aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-REWRITES-NEXT:     repeated.a = 1;
// COMMON-REWRITES-NEXT:     repeated.a = 2;
// COMMON-REWRITES-NEXT:     repeated.b = 3;
// COMMON-REWRITES-NEXT:     (repeated.a as i32) + repeated.b
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn dependent_case() -> i32 {
// COMMON-REWRITES-NEXT:     let mut dependent: aligned::Aligned<aligned::A16, Aligned> =
// COMMON-REWRITES-NEXT:         aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-REWRITES-NEXT:     dependent.b = 8;
// COMMON-REWRITES-NEXT:     (dependent.a as i32) + dependent.b
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn interrupted_case() -> i32 {
// COMMON-REWRITES-NEXT:     let mut interrupted: aligned::Aligned<aligned::A16, Aligned> =
// COMMON-REWRITES-NEXT:         aligned::Aligned(Aligned { a: 0, b: 0 });
// COMMON-REWRITES-NEXT:     interrupted.a = 4;
// COMMON-REWRITES-NEXT:     touch();
// COMMON-REWRITES-NEXT:     interrupted.b = 6;
// COMMON-REWRITES-NEXT:     (interrupted.a as i32) + interrupted.b + unsafe { counter }
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn next_value() -> i32 {
// COMMON-REWRITES-NEXT:     9
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn touch() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         counter = (unsafe { counter }) + 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     return;
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-NEXT:     a: i8,
// REWRITES-X86_64-GNU-NEXT:     effectful.a = {{__v[0-9]+}} as i8;
// REWRITES-X86_64-GNU-NEXT:     dependent.a = dependent.b as i8;
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-NEXT:     a: u8,
// REWRITES-AARCH64-GNU-NEXT:     effectful.a = {{__v[0-9]+}} as u8;
// REWRITES-AARCH64-GNU-NEXT:     dependent.a = dependent.b as u8;
// SLATE-FILECHECK-END rewrites-aarch64-gnu
