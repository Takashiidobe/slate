void abort(void);
void exit(int);

typedef struct {
  double epsilon;
} material_type;

material_type foo(double x) {
  material_type m;

  m.epsilon = 1.0 + x;
  return m;
}

int main(void) {
  int           i;
  material_type x;

  /* We must iterate enough times to overflow the FP stack on the
     x86.  */
  for (i = 0; i < 10; i++) {
    x = foo(1.0);
    if (x.epsilon != 1.0 + 1.0)
      abort();
  }

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     epsilon: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: f64) -> [[SLATERECORD_0]] {
// CHECK-NEXT:     let mut m: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     m.epsilon = 1.0f64 + x;
// CHECK-NEXT:     return m;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     // We must iterate enough times to overflow the FP stack on the
// CHECK-NEXT:     // x86.
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut x: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (10 as i32) {
// CHECK-NEXT:         x = foo(1.0f64);
// CHECK-NEXT:         if x.epsilon != 1.0f64 + 1.0f64 {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         i = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
