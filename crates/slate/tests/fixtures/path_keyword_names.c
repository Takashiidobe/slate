#include <stdio.h>

struct writer {
  const char *name;
  int phase;
};

struct context {
  struct writer super;
  int self;
  int crate;
  int Self;
};

static int super = 3;

static int combine(int self, int crate) {
  int Self = self * crate;
  return Self + super;
}

int main(void) {
  struct context ctx = {{"out", 2}, 5, 7, 11};
  ctx.super.phase += ctx.self;
  printf("%s %d %d\n", ctx.super.name, ctx.super.phase, ctx.crate + ctx.Self);
  printf("%d\n", combine(ctx.self, ctx.crate));
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct writer {
// CHECK-NEXT:     name: *const i8,
// CHECK-NEXT:     phase: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct context {
// CHECK-NEXT:     super_: writer,
// CHECK-NEXT:     self_: i32,
// CHECK-NEXT:     crate_: i32,
// CHECK-NEXT:     Self_: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut super_: i32 = 3 as i32;
// CHECK-NEXT: fn combine(self_: i32, crate_: i32) -> i32 {
// CHECK-NEXT:     let Self_: i32 = self_ * crate_;
// CHECK-NEXT:     Self_ + unsafe { super_ }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut ctx: context = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: context = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].super_ = {
// CHECK-NEXT:             let mut [[T_1:__t[0-9]+]]: writer = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_1]].name = c"out".as_ptr();
// CHECK-NEXT:             [[T_1]].phase = 2 as i32;
// CHECK-NEXT:             [[T_1]]
// CHECK-NEXT:         };
// CHECK-NEXT:         [[T_0]].self_ = 5 as i32;
// CHECK-NEXT:         [[T_0]].crate_ = 7 as i32;
// CHECK-NEXT:         [[T_0]].Self_ = 11 as i32;
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     ctx.super_.phase += ctx.self_;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%s %d %d\n".as_ptr(),
// CHECK-NEXT:             ctx.super_.name,
// CHECK-NEXT:             ctx.super_.phase,
// CHECK-NEXT:             ctx.crate_ + ctx.Self_,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", combine(ctx.self_, ctx.crate_));
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
