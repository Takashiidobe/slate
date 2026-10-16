void exit(int);

static int rule_text_needs_stack_pop = 0;
static int input_stack_pos           = 1;

int f(void) {
  rule_text_needs_stack_pop = 1;

  if (input_stack_pos <= 0)
    return 1;
  else
    return 0;
}

int main(void) {
  f();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut rule_text_needs_stack_pop: i32 = 0 as i32;
// CHECK-NEXT: static mut input_stack_pos: i32 = 1 as i32;
// CHECK-NEXT: fn f() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         rule_text_needs_stack_pop = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { input_stack_pos }) <= (0 as i32) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     f();
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
// SLATE-FILECHECK-END
