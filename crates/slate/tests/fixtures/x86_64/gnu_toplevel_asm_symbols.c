#include <stdio.h>

static int counter = 40;

__attribute__((used)) static int twice(int value) { return value * 2; }

static int add(int left, int right) { return left + right; }

__asm__(".text\n"
        ".globl toplevel_bump\n"
        ".type toplevel_bump,@function\n"
        "toplevel_bump:\n"
        "  incl counter(%rip) # {braces}\n"
        "  movl counter(%rip), %eax\n"
        "  add $1, %eax\n"
        "  ret\n"
        ".size toplevel_bump,.-toplevel_bump\n");

__asm__(".text\n"
        ".globl toplevel_twice\n"
        ".type toplevel_twice,@function\n"
        "toplevel_twice:\n"
        "  jmp twice\n"
        ".size toplevel_twice,.-toplevel_twice\n");

extern int toplevel_bump(void);
extern int toplevel_twice(int value);

int main(void) {
  int bumped = toplevel_bump();
  printf("%d %d %d %d\n", bumped, counter, toplevel_twice(21), add(2, 3));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn toplevel_bump() -> i32;
// CHECK-NEXT:     fn toplevel_twice(value: i32) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut counter: i32 = 40;
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".text\n.globl toplevel_bump\n.type toplevel_bump,@function\ntoplevel_bump:\n  incl {0}(%rip) # {{[{][{]}}braces}}\n  movl {0}(%rip), %eax\n  add $1, %eax\n  ret\n.size toplevel_bump,.-toplevel_bump\n\n.text\n.globl toplevel_twice\n.type toplevel_twice,@function\ntoplevel_twice:\n  jmp {1}\n.size toplevel_twice,.-toplevel_twice\n",
// CHECK-NEXT:     sym counter, sym twice, options(att_syntax)
// CHECK-NEXT: );
// CHECK-NEXT: extern "C-unwind" fn twice(value: i32) -> i32 {
// CHECK-NEXT:     value * 2
// CHECK-NEXT: }
// CHECK-NEXT: fn add(left: i32, right: i32) -> i32 {
// CHECK-NEXT:     left + right
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let bumped: i32 = unsafe { toplevel_bump() };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", bumped, unsafe { counter }, unsafe { toplevel_twice(21i32) },
// CHECK-NEXT:         add(2i32, 3i32)
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
