/* PR c/4308
   This testcase failed because 0x8000000000000000 >> 0
   was incorrectly folded into 0xffffffff00000000.  */

extern void abort(void);
extern void exit(int);

long long foo(void) {
  long long C = 1ULL << 63, X;
  int       Y = 32;
  X           = C >> (Y & 31);
  return X;
}

int main(void) {
  if (foo() != 1ULL << 63)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/4308
// CHECK-NEXT: //! This testcase failed because 0x8000000000000000 >> 0
// CHECK-NEXT: //! was incorrectly folded into 0xffffffff00000000.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i64 {
// CHECK-NEXT:     let mut C: i64 = ((1 as u64) << (63 as i32)) as i64;
// CHECK-NEXT:     let mut X: i64 = 0 as i64;
// CHECK-NEXT:     let mut Y: i32 = 32 as i32;
// CHECK-NEXT:     X = C >> (Y & (31 as i32));
// CHECK-NEXT:     X
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (foo() as u64) != (1 as u64) << (63 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
