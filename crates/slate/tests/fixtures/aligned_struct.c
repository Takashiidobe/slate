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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Aligned {
// CHECK-NEXT:     a: i8,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn next_value() -> i32 {
// CHECK-NEXT:     9
// CHECK-NEXT: }
// CHECK-NEXT: fn effectful_case() -> i32 {
// CHECK-NEXT:     let mut effectful: Aligned = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     effectful.a = next_value() as i8;
// CHECK-NEXT:     effectful.b = 7;
// CHECK-NEXT:     (effectful.a as i32) + effectful.b
// CHECK-NEXT: }
// CHECK-NEXT: fn repeated_case() -> i32 {
// CHECK-NEXT:     let mut repeated: Aligned = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     repeated.a = 1;
// CHECK-NEXT:     repeated.a = 2;
// CHECK-NEXT:     repeated.b = 3;
// CHECK-NEXT:     (repeated.a as i32) + repeated.b
// CHECK-NEXT: }
// CHECK-NEXT: fn dependent_case() -> i32 {
// CHECK-NEXT:     let mut dependent: Aligned = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     dependent.b = 8;
// CHECK-NEXT:     dependent.a = dependent.b as i8;
// CHECK-NEXT:     (dependent.a as i32) + dependent.b
// CHECK-NEXT: }
// CHECK-NEXT: static mut counter: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn touch() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         counter = counter + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn interrupted_case() -> i32 {
// CHECK-NEXT:     let mut interrupted: Aligned = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     interrupted.a = 4;
// CHECK-NEXT:     touch();
// CHECK-NEXT:     interrupted.b = 6;
// CHECK-NEXT:     (interrupted.a as i32) + interrupted.b + unsafe { counter }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: Aligned = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     s.a = 5;
// CHECK-NEXT:     s.b = 4660;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 16u64, 16u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu\n".as_ptr(), 0u64, 4u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%d %x\n".as_ptr(), s.a as i32, s.b) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", effectful_case(), repeated_case(), dependent_case(),
// CHECK-NEXT:         interrupted_case()
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
