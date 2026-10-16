/* This exposed a bug in tree-ssa-ccp.c.  Since 'j' and 'i' are never
   defined, CCP was not traversing the edges out of the if(), which caused
   the PHI node for 'k' at the top of the while to only be visited once.
   This ended up causing CCP to think that 'k' was the constant '1'.  */
void abort(void);

int main(void) {
  int i, j, k;

  k = 0;
  while (k < 10) {
    k++;
    if (j > i)
      j = 5;
    else
      j = 3;
  }

  if (k != 10)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // This exposed a bug in tree-ssa-ccp.c.  Since 'j' and 'i' are never
// CHECK-NEXT: // defined, CCP was not traversing the edges out of the if(), which caused
// CHECK-NEXT: // the PHI node for 'k' at the top of the while to only be visited once.
// CHECK-NEXT: // This ended up causing CCP to think that 'k' was the constant '1'.
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut k: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while k < (10 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = k;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         k = [[V_1]];
// CHECK-NEXT:         if j > i {
// CHECK-NEXT:             j = 5 as i32;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             j = 3 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if k != (10 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
