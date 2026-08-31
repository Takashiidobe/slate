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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(16))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct Aligned {
// LOWERING-NEXT:     a: i8,
// LOWERING-NEXT:     b: i32,
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut counter: i32 = 0;
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn next_value() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 9;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn effectful_case() -> i32 {
// LOWERING-NEXT:     let mut effectful: aligned::Aligned<aligned::A16, Aligned> = aligned::Aligned(Aligned { a: 0, b: 0 });
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = next_value();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = {{_v[0-9]+}} as i8;
// LOWERING-NEXT:     effectful.a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     effectful.b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = effectful.a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = effectful.b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn repeated_case() -> i32 {
// LOWERING-NEXT:     let mut repeated: aligned::Aligned<aligned::A16, Aligned> = aligned::Aligned(Aligned { a: 0, b: 0 });
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 1;
// LOWERING-NEXT:     repeated.a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 2;
// LOWERING-NEXT:     repeated.a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     repeated.b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = repeated.a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = repeated.b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn dependent_case() -> i32 {
// LOWERING-NEXT:     let mut dependent: aligned::Aligned<aligned::A16, Aligned> = aligned::Aligned(Aligned { a: 0, b: 0 });
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     dependent.b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = dependent.b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = {{_v[0-9]+}} as i8;
// LOWERING-NEXT:     dependent.a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = dependent.a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = dependent.b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn touch() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { counter };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         counter = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn interrupted_case() -> i32 {
// LOWERING-NEXT:     let mut interrupted: aligned::Aligned<aligned::A16, Aligned> = aligned::Aligned(Aligned { a: 0, b: 0 });
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 4;
// LOWERING-NEXT:     interrupted.a = {{_v[0-9]+}};
// LOWERING-NEXT:     touch();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     interrupted.b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = interrupted.a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = interrupted.b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { counter };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut s: aligned::Aligned<aligned::A16, Aligned> = aligned::Aligned(Aligned { a: 0, b: 0 });
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = 5;
// LOWERING-NEXT:     s.a = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4660;
// LOWERING-NEXT:     s.b = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::size_of::<Aligned>() as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::align_of::<Aligned>() as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%zu %zu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::offset_of!(Aligned, a) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = std::mem::offset_of!(Aligned, b) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %x\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i8 = s.a;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = s.b;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = effectful_case();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = repeated_case();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = dependent_case();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = interrupted_case();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: std::mem::size_of::<Aligned>() as u64
// REWRITES-DAG: std::mem::align_of::<Aligned>() as u64
// REWRITES-DAG: std::mem::offset_of!(Aligned, a) as u64
// REWRITES-DAG: std::mem::offset_of!(Aligned, b) as u64
// REWRITES-NOT: let mut s: Aligned
