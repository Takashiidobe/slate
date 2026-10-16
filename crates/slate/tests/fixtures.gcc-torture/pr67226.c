struct assembly_operand {
  int type, value, symtype, symflags, marker;
};

struct assembly_operand to_input, from_input;

void __attribute__((__noinline__, __noclone__))
assemblez_1(int internal_number, struct assembly_operand o1) {
  if (o1.type != from_input.type)
    __builtin_abort();
}

void __attribute__((__noinline__, __noclone__))
t0(struct assembly_operand to, struct assembly_operand from) {
  if (to.value == 0)
    assemblez_1(32, from);
  else
    __builtin_abort();
}

int main(void) {
  to_input.value    = 0;
  to_input.type     = 1;
  to_input.symtype  = 2;
  to_input.symflags = 3;
  to_input.marker   = 4;

  from_input.value    = 5;
  from_input.type     = 6;
  from_input.symtype  = 7;
  from_input.symflags = 8;
  from_input.marker   = 9;

  t0(to_input, from_input);

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct assembly_operand {
// CHECK-NEXT:     r#type: i32,
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     symtype: i32,
// CHECK-NEXT:     symflags: i32,
// CHECK-NEXT:     marker: i32,
// CHECK-NEXT: }
// CHECK-NEXT: static mut to_input: assembly_operand = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut from_input: assembly_operand = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn assemblez_1(mut internal_number: i32, mut o1: assembly_operand) {
// CHECK-NEXT:     if o1.r#type != unsafe { from_input.r#type } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn t0(mut to: assembly_operand, mut from: assembly_operand) {
// CHECK-NEXT:     if to.value == (0 as i32) {
// CHECK-NEXT:         assemblez_1(32 as i32, from);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         to_input.value = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         to_input.r#type = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         to_input.symtype = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         to_input.symflags = 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         to_input.marker = 4 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         from_input.value = 5 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         from_input.r#type = 6 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         from_input.symtype = 7 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         from_input.symflags = 8 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         from_input.marker = 9 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     t0(unsafe { to_input }, unsafe { from_input });
// CHECK-NEXT:     0 as i32
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
