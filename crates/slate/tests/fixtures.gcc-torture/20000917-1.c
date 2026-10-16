/* This bug exists in gcc-2.95, egcs-1.1.2, gcc-2.7.2 and probably
   every other version as well.  */

void abort(void);
void exit(int);

typedef struct int3 {
  int a, b, c;
} int3;

int3 one(void) { return (int3){1, 1, 1}; }

int3 zero(void) { return (int3){0, 0, 0}; }

int main(void) {
  int3 a;

  /* gcc allocates a temporary for the inner expression statement
     to store the return value of `one'.

     gcc frees the temporaries for the inner expression statement.

     gcc realloates the same temporary slot to store the return
     value of `zero'.

     gcc expands the call to zero ahead of the expansion of the
     statement expressions.  The temporary gets the value of `zero'.

     gcc expands statement expressions and the stale temporary is
     clobbered with the value of `one'.  The bad value is copied from
     the temporary into *&a.  */

  *({
    ({
      one();
      &a;
    });
  }) = zero();
  if (a.a && a.b && a.c)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This bug exists in gcc-2.95, egcs-1.1.2, gcc-2.7.2 and probably
// CHECK-NEXT: //! every other version as well.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct int3 {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     c: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn one() -> int3 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: int3 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = int3 {
// CHECK-NEXT:                 a: 1 as i32,
// CHECK-NEXT:                 b: 1 as i32,
// CHECK-NEXT:                 c: 1 as i32,
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn zero() -> int3 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: int3 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_1]] = int3 {
// CHECK-NEXT:                 a: 0 as i32,
// CHECK-NEXT:                 b: 0 as i32,
// CHECK-NEXT:                 c: 0 as i32,
// CHECK-NEXT:             };
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: int3 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     // gcc allocates a temporary for the inner expression statement
// CHECK-NEXT:     // to store the return value of `one'.
// CHECK-NEXT:     //
// CHECK-NEXT:     // gcc frees the temporaries for the inner expression statement.
// CHECK-NEXT:     //
// CHECK-NEXT:     // gcc realloates the same temporary slot to store the return
// CHECK-NEXT:     // value of `zero'.
// CHECK-NEXT:     //
// CHECK-NEXT:     // gcc expands the call to zero ahead of the expansion of the
// CHECK-NEXT:     // statement expressions.  The temporary gets the value of `zero'.
// CHECK-NEXT:     //
// CHECK-NEXT:     // gcc expands statement expressions and the stale temporary is
// CHECK-NEXT:     // clobbered with the value of `one'.  The bad value is copied from
// CHECK-NEXT:     // the temporary into *&a.
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: *mut int3;
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut int3;
// CHECK-NEXT:     one();
// CHECK-NEXT:     [[V_2]] = std::ptr::addr_of_mut!(a);
// CHECK-NEXT:     [[V_1]] = [[V_2]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_1]] = zero();
// CHECK-NEXT:     }
// CHECK-NEXT:     if a.a != (0 as i32) && a.b != (0 as i32) && a.c != (0 as i32) {
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
