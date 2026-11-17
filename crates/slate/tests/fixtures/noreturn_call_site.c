#include <stdio.h>
#include <stdlib.h>

static int stage;

__attribute__((noreturn)) static void finish(int code) {
  printf("finish %d stage %d\n", code, stage);
  fflush(stdout);
  exit(code);
}

_Noreturn void stop(int code);

void stop(int code) { finish(code + 1); }

void (*finisher)(int) = finish;

static void step(int value) {
  stage = value;
  if (value == 3) {
    stop(value);
    stage = -1;
  }
}

int main(void) {
  for (int i = 0; i < 5; i++) {
    step(i);
    printf("stage %d\n", stage);
  }
  finisher(9);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush([[ARG_0:arg[0-9]+]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut stage: i32 = unsafe { core::mem::zeroed() };
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.finish")]
// CHECK-NEXT: extern "C-unwind" fn finish(code: i32) {
// CHECK-NEXT:     println!("finish {} stage {}", code, unsafe { stage });
// CHECK-NEXT:     unsafe { fflush(slate_support::stdout()) };
// CHECK-NEXT:     unsafe { std::process::exit(code as i32) };
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::exit;
// CHECK-NEXT: fn stop(code: i32) {
// CHECK-NEXT:     finish(code + 1);
// CHECK-NEXT:     unsafe { core::hint::unreachable_unchecked() };
// CHECK-NEXT: }
// CHECK-NEXT: static mut finisher: Option<unsafe extern "C-unwind" fn(i32)> = Some(finish);
// CHECK-NEXT: fn step(value: i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         stage = value;
// CHECK-NEXT:     }
// CHECK-NEXT:     if value == 3 {
// CHECK-NEXT:         stop(value);
// CHECK-NEXT:         unsafe { core::hint::unreachable_unchecked() };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             stage = -1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0i32..5i32 {
// CHECK-NEXT:         step(i);
// CHECK-NEXT:         println!("stage {}", unsafe { stage });
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { unsafe { finisher }.unwrap()(9i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
