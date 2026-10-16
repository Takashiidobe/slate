void abort(void);
void exit(int);

int f() {
  int  j = 1;
  long i;
  for (i = -0x70000000L; i < 0x60000000L; i += 0x10000000L)
    j <<= 1;
  return j;
}

int main() {
  if (f() != 8192)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn f() -> i32 {
// CHECK-NEXT:     let mut j: i32 = 1 as i32;
// CHECK-NEXT:     let mut i: i64 = -(1879048192 as i64);
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (1610612736 as i64) {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] << (1 as i32);
// CHECK-NEXT:         j = [[V_2]];
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i64 = i;
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: i64 = [[V_3]] + (268435456 as i64);
// CHECK-NEXT:         i = [[V_4]];
// CHECK-NEXT:     }
// CHECK-NEXT:     j
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f() != (8192 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
